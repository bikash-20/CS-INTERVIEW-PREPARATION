# Design Twitter (Social Media Feed)

Tests your ability to design a fan-out service, handle the celebrity problem, and reason about hot paths at massive scale.

---

## 1. Requirements

### Functional
- Post tweets (text, media URLs).
- Follow / unfollow users.
- View home timeline (tweets from people I follow).
- Like, retweet, reply.
- Search users / tweets.
- Notifications.

### Non-Functional
- High availability — feeds must always load.
- Low latency — home timeline < 200 ms.
- Massive scale — 300M+ DAU.
- Eventual consistency is fine for the timeline (a few seconds lag OK).

---

## 2. Capacity Estimation

Assume:
- DAU = 300M
- Tweets/day = 300M
- Avg follows per user = 200
- Read:write = 100:1 for timeline reads

| Metric | Value |
|---|---|
| Tweets/sec avg | 300M / 86400 ≈ **3,500** |
| Tweets/sec peak | ≈ **10,000** |
| Timeline reads/sec | 3,500 × 100 ≈ **350,000** |
| Avg timeline size | ~800 tweets cached |
| Fanout per tweet (avg) | 200 followers |
| Total fanout writes/sec | 3,500 × 200 = **700,000 ops/sec** |
| Storage / day | 300M × ~300 B ≈ **90 GB** |
| Storage / year | ≈ **30 TB** |

---

## 3. High-Level Design

```
 ┌─────────┐
 │ Client │
 └────┬────┘
 │ HTTPS
 ▼
 ┌────────────┐
 │ API Gateway │ ─── Auth, rate limit, routing
 └────┬───────┘
 │
 ┌────────────┬─────────────┬───────────────┐
 ▼ ▼ ▼
 ┌──────────┐ ┌──────────────┐ ┌──────────┐
 │ Tweet │ │ Timeline │ │ User │
 │ Service │ │ Service │ │ Service │
 └─────┬────┘ └──────┬───────┘ └─────┬────┘
 │ │ │
 ┌────┴────┐ ┌──────┴──────┐ ┌──────┴───────┐
 │Cassandra│ │ Redis (lists)│ │User Graph DB │
 │ + S3 │ │ + Fanout Q │ │ (TAO-style) │
 └─────────┘ └─────────────┘ └──────────────┘
 │
 Kafka (events for search, analytics, notifications)
```

---

## 4. Core Data Models

### Tweet
```
tweet_id BIGINT PK (Snowflake, time-ordered)
user_id BIGINT
content TEXT
media_urls JSON
parent_tweet_id BIGINT NULL -- replies
retweet_of BIGINT NULL
created_at TIMESTAMP
like_count BIGINT
retweet_count BIGINT
reply_count BIGINT
```

### User
```
user_id BIGINT PK
username VARCHAR(50) UNIQUE
display_name VARCHAR
bio TEXT
follower_count BIGINT
following_count BIGINT
```

### Follow
```
follower_id BIGINT
followee_id BIGINT
PRIMARY KEY (follower_id, followee_id)
```

---

## 5. Timeline Generation — Three Approaches

### 1. Pull (Fan-out on Read)
On request, fetch tweets from each followee, merge, sort.
- **Pros:** Simple writes.
- **Cons:** Slow reads (especially for users who follow many people).

### 2. Push (Fan-out on Write) — used by Twitter for normal users
On tweet creation, push the tweet ID into each follower's timeline cache.
- **Pros:** Reads are a single Redis read.
- **Cons:** Slow writes for celebrities (millions of followers).

### 3. Hybrid (recommended)
- Push for normal users.
- Pull for celebrities (≥ N followers).
- On read: merge pushed + pulled lists.

---

## 6. Storage Choices

| Data | Store | Reason |
|---|---|---|
| Tweets | Cassandra / ScyllaDB | Wide-column, write-heavy, time-series friendly |
| User profile | MySQL / Postgres | Strong consistency, simple relations |
| User graph (follows) | TAO-style or Neo4j | Optimized for graph reads |
| Timeline cache | Redis sorted set | Time-ordered list, fast range reads |
| Media | S3 + CDN | Object storage |
| Search | Elasticsearch | Inverted index for full-text |
| Analytics events | Kafka → Snowflake | Real-time stream + warehouse |

---

## 7. Tweet Posting Flow

```
1. Client uploads media to S3 via presigned URL.
2. Client POST /tweets with content + media refs.
3. Tweet Service:
 - Generates Snowflake tweet_id.
 - Writes to Cassandra.
4. Publishes "tweet.created" event to Kafka.
5. Fanout Service:
 - Reads event.
 - For each follower (excluding celebrities):
 * Pushes tweet_id onto their Redis timeline list (capped at 800).
6. Returns 201 to client.
```

---

## 8. Home Timeline Read Flow

```
1. Client GET /home_timeline.
2. API → Timeline Service.
3. Service reads Redis sorted set (pushed tweets).
4. Merges with tweets from celebrities I follow (pulled).
5. Filters out muted / blocked users.
6. Returns ranked list.
7. Client renders.
```

### Ranking
- Reverse chronological by default.
- ML ranking later: engagement, recency, affinity.

---

## 9. Celebrity Problem

A user with 100M followers posts a tweet → pushing to 100M Redis lists is expensive.

### Mitigation
- **Hybrid model:** if `follower_count > threshold (e.g., 10K)`, do **not** push to followers.
- On read for a user who follows such celebrities, pull their recent tweets and merge.

---

## 10. Optimizations

- **Snowflake ID** — sortable by time, no DB round-trip, no contention.
- **In-process LRU** for hot timelines.
- **Pre-generate timelines** for active users every few minutes.
- **CDN** for media (images, videos).
- **Rate limit** posts, follows, likes per user.
- **Search indexing** is async via Kafka.
- **Notification fan-out** also async.

---

## 11. API Design

```
POST /api/v1/tweets
 Body: { "content": "...", "media_ids": ["..."] }
 → 201 { "tweet_id": "1689..." }

GET /api/v1/home_timeline?limit=20&cursor=...
 → 200 { "tweets": [...], "next_cursor": "..." }

POST /api/v1/follows/{user_id}
POST /api/v1/tweets/{id}/like
POST /api/v1/tweets/{id}/retweet
```

All writes return quickly; downstream effects happen async.

---

## 12. Failure Modes

| Failure | Mitigation |
|---|---|
| Redis down | Serve from Cassandra (slower). |
| Fanout lag | Eventually catches up; user sees slight delay. |
| Celebrity spike | Hybrid mitigates; cap push fanout per tweet. |
| Snowflake service down | Fail writes; alarm; fallback to UUID. |
| S3 outage | Tweet still posts with text-only; client retries media upload. |

---

## 13. Follow-up Questions

**Q: How to handle retweets?**
Store as a row pointing to original + the retweeting user. Or embed a flag in timeline entries.

**Q: How to rank by relevance instead of time?**
Offline ML model computes a score per (user, tweet). Store in a separate index. Merge into timeline by score.

**Q: How to handle replies / threads?**
Store `parent_tweet_id` on each reply; index by it; load thread lazily.

**Q: How to support search?**
Async event to Kafka → Elasticsearch indexer → searchable in ~1s.

**Q: How to scale the follow graph?**
Sharded by `user_id`. Cache hot users' follower lists in Redis. Use a TAO-style cache (Facebook) for very high QPS.

**Q: How to implement "trending"?**
Streaming pipeline (Kafka → Flink) computes topic counters in 5-minute windows.

**Q: How to handle media uploads at scale?**
Direct upload to S3 with **presigned URLs** so the API server never sees the bytes.

**Q: How to send notifications?**
Dedicated notification service consumes events from Kafka; pushes via APNs / FCM / WebSocket.

**Q: How to prevent abuse / spam?**
- Rate limit per user/IP.
- ML classifier on content.
- Shadow ban / mute / block.

**Q: How to handle user blocking / muting?**
Filter at timeline read time. Maintain a per-user block list; check on each tweet before returning.

---

## 14. End-to-End Diagram

```
 Client
 │
 │ POST /tweets
 ▼
 API Gateway ─► Tweet Service ─► Cassandra (tweets)
 │ │
 │ publish tweet.created event
 ▼ ▼
 Kafka
 │
 ▼
 Fanout Workers ──► Redis (per-user timeline lists)
 ▲
 │
 Client ◄──────── Timeline Service (merge pushed + celebrities)
 │
 │ media URLs
 ▼
 S3 + CDN
```
