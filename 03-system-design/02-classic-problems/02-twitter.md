# Design Twitter (Social Media Feed)

## Requirements

### Functional
- Post tweets (text, media).
- Follow / unfollow users.
- Home timeline (tweets from followed users, reverse chronological or ranked).
- Like, retweet, reply.

### Non-Functional
- High availability.
- Low latency feed loading.
- Massive scale (300M+ DAU).

---

## Capacity Estimation

- DAU: 300M
- Tweets/day: 300M
- Read QPS: 300M * 100 reads / 86400s ≈ 350K reads/sec
- Write QPS: 300M / 86400 ≈ 3500 writes/sec

---

## High-Level Design

```
┌────────┐ ┌────────────┐ ┌──────────┐
│ Client │ ──────►│ API GW │────────►│ Tweet │
└────────┘ └─────┬──────┘ │ Service │
 ▲ │ └────┬─────┘
 │ Timeline │ │
 │ Response ▼ ▼
 │ ┌────────────┐ ┌──────────┐
 └────────────│ Timeline │◄────────│ User │
 │ Service │ │ Service │
 └─────┬──────┘ └──────────┘
 │
 ▼
 ┌──────────────────┐
 │ Tweet Cache (Redis)│
 └──────────────────┘
 │
 ▼
 ┌──────────────────────┐
 │ User Graph (Neo4j) │
 └──────────────────────┘
```

---

## Core Data Models

```sql
-- Tweet
tweet_id BIGINT PRIMARY KEY
user_id BIGINT
content TEXT
created_at TIMESTAMP
media_urls JSON
parent_tweet_id BIGINT NULL -- for replies
retweet_of BIGINT NULL

-- User
user_id BIGINT PRIMARY KEY
username VARCHAR
bio TEXT

-- Follow
follower_id BIGINT
followee_id BIGINT
PRIMARY KEY (follower_id, followee_id)
```

---

## Timeline Generation — Two Approaches

### Approach 1: Pull (Fan-out on Read)
- On request, fetch tweets from all followed users, merge, sort.
- **Pros:** Simple writes; no duplication.
- **Cons:** Slow reads for users following many people; expensive.

### Approach 2: Push (Fan-out on Write) — **Used by Twitter**
- When a user posts, push the tweet into every follower's timeline cache.
- **Pros:** Fast reads (just read the pre-built list).
- **Cons:** Slow writes for celebrity tweets (millions of followers).
- **Storage:** ~1500 tweets/user in cache.

### Hybrid
- Push for normal users; pull for celebrities (>10K followers).
- Merge at read time.

---

## Storage Choices

| Data | Storage |
|------|---------|
| Tweets | Cassandra / ScyllaDB (wide column, write-heavy, time-series) |
| User graph | Neo4j / TAO (Facebook's) |
| Timeline cache | Redis (Sorted Set by timestamp) |
| Media | Object store (S3) + CDN |
| Search | Elasticsearch |

---

## Key Optimizations

1. **Tweet ID = Snowflake** — sortable by time, no DB-generated IDs.
2. **Cache hottest timelines** in Redis.
3. **Materialized timeline** per user (write-fanout).
4. **Pre-generate timeline** for active users every few minutes.
5. **CDN for media** delivery.
6. **Rate limiting** per user for tweets / follows.

---

## Celebrity Problem
A user with 100M followers posts a tweet — fanning out to 100M lists is expensive.

**Solution:** Pull path for celebrities. Detect at write time and just store in celebrity's "tweet list". On read, merge.

---

## Follow-up Questions

**Q: How to handle retweets?**
Store retweet as a separate row pointing to original. Or just embed reference.

**Q: How to rank timeline (instead of reverse chronological)?**
ML model scores tweets based on engagement, recency, affinity. Score computed offline; stored per user-tweet.

**Q: How to handle media uploads?**
Direct upload to S3 via signed URL. Tweet service stores URL reference.

**Q: Search?**
Elasticsearch cluster; index tweets on creation.

---

## Diagram: Tweet Posting Flow

```
1. Client uploads media to S3 (presigned URL)
2. Client → API: POST /tweet (content + media URLs)
3. API → Tweet Service: persist to Cassandra
4. API → Fanout Service: publish event to Kafka
5. Fanout workers: for each follower, push tweet to Redis timeline
6. Return success to client
```
