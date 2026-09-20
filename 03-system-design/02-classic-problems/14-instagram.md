# Design Instagram (Photo Sharing + Feed)

Tests media handling, fan-out, and timeline generation at scale.

---

## 1. Requirements

### Functional
- Upload photos / videos with captions.
- Follow / unfollow users.
- Home feed: posts from followed users.
- Likes, comments.
- Stories (24 h ephemeral).
- Explore / search.

### Non-Functional
- High read throughput (feed loads).
- Low latency for feed.
- Media served from CDN.

---

## 2. Capacity Estimation

Assume 500M DAU, 100M photos/day, 5:1 read:write.

| Metric | Value |
|---|---|
| Photos/day | 100M |
| Photo writes/sec avg | ~1,200, peak ~5,000 |
| Feed reads/sec | ~6,000 avg |
| Avg followers / user | 200 |
| Total follow edges | 100B |

---

## 3. High-Level Design

```
 Client ─► API Gateway
 │
 ├── User Service
 ├── Media Service (upload)
 ├── Feed Service (timeline)
 ├── Story Service
 └── Search Service

 Storage:
 - Photos / videos: S3 + CDN
 - Metadata: Postgres + Cassandra
 - Feed cache: Redis (per user)
 - Search: Elasticsearch
 - Stories: Redis TTL + S3
```

---

## 4. Upload Flow

1. Client requests presigned URL (`POST /upload/init`).
2. Client uploads image bytes directly to S3.
3. Client calls `POST /posts` with the S3 URL + caption.
4. Post Service writes metadata to DB.
5. Fanout Service enqueues event "post.created".
6. Fanout pushes post_id into each follower's feed cache (or pull for celebrities).

---

## 5. Feed Read Flow

1. Client `GET /feed?cursor=...`.
2. Feed Service reads Redis sorted set (pushed posts) + merges celebrity pulled posts.
3. Returns ranked list of posts with image URLs (CDN).
4. Pagination by cursor.

### Ranking
- Reverse chrono by default.
- ML re-rank based on engagement + affinity.

---

## 6. Stories

- 24-hour ephemeral content.
- **Storage:** media in S3; metadata in Redis with TTL 24 h.
- **View tracking:** see who watched, expires with the story.
- **Fanout:** push to followers via the same feed infrastructure, filtered by `is_story=true`.

---

## 7. Likes / Comments

- `likes(post_id, user_id)` — composite key for uniqueness.
- Counts stored on post row; updated async (Redis INCR + flush).
- Comments in a separate table, paginated.

---

## 8. Explore / Search

- Elasticsearch indexes users, hashtags, captions.
- Explore ranker: candidate generation (similar users' likes) + ML ranker.

---

## 9. Database Schema

```
Table: posts
 post_id BIGINT PK (Snowflake)
 user_id BIGINT
 caption TEXT
 media_urls JSON
 media_type ENUM(photo, video, carousel)
 created_at TIMESTAMP
 like_count, comment_count BIGINT
 visibility ENUM(public, followers, private)

Table: follows
 follower_id BIGINT
 followee_id BIGINT
 PRIMARY KEY (follower_id, followee_id)

Table: likes
 post_id BIGINT
 user_id BIGINT
 PRIMARY KEY (post_id, user_id)
 created_at

Table: comments
 comment_id BIGINT PK
 post_id BIGINT
 user_id BIGINT
 text TEXT
 created_at
```

---

## 10. Optimizations

- **CDN everywhere** for media.
- **Image variants** generated on upload (thumb, medium, large).
- **Lazy generation** for some sizes.
- **Feed cache TTL** (e.g., 1 day) — partial refresh on new posts.
- **Hot post replication** in cache.
- **Idempotency keys** on writes (uploads can retry).

---

## 11. Failure Modes

| Failure | Mitigation |
|---|---|
| S3 slow | CDN absorbs; degraded quality OK. |
| Fanout lag | Users see a few seconds of staleness. |
| Celebrity spike | Hybrid push/pull; merge at read. |
| DB down | Read-only mode (cached feed). |

---

## 12. API Design

```
POST /api/v1/upload/init
 Body: { size, mime }
 → 200 { upload_url, media_id }

PUT <upload_url>
POST /api/v1/posts
 Body: { media_ids, caption }

GET /api/v1/feed?cursor&limit
POST /api/v1/posts/{id}/like
POST /api/v1/posts/{id}/comments
GET /api/v1/users/{id}
GET /api/v1/search?q=
```

---

## 13. Follow-up Questions

**Q: How to handle reels (short videos)?**
Same media pipeline; transcode to multiple bitrates; HLS or MP4 with pre-roll ads.

**Q: How to detect and remove harmful content?**
ML classifier on upload (CSAM, hate speech). User reports feed back into the model.

**Q: How to support close-friends-only stories?**
ACL on the story; only fans of the post can see.

**Q: How to keep memory usage down on the feed cache?**
Cap list at 1000-2000 entries; LRU evict.

**Q: How to migrate from one DB to another?**
Dual-write with consistency check; eventually switch reads.

---

## 14. End-to-End Diagram

```
 Client
 │
 │ upload (presigned)
 ▼
 S3 → CDN edges
 │
 │ POST /posts
 ▼
 API Gateway ─► Post Service ─► Postgres + Cassandra
 │
 ▼ Kafka
 Fanout Service ─► Redis (per-user feed)
 │
 │
 Read path ─► Feed Service ─► Redis + Cassandra (celebrity pull)
 │
 ▼
 Client renders
```
