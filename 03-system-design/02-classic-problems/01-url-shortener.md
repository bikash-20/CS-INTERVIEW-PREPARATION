# Design URL Shortener (TinyURL / bit.ly)

## Requirements

### Functional
- Given a long URL, generate a unique short URL.
- Given a short URL, redirect to the original long URL.
- Optional: custom aliases, expiration, analytics.

### Non-Functional
- **High availability** — reads >> writes.
- **Low latency** — redirection must be fast.
- **Scalable** — billions of URLs.

### Capacity Estimation
- Write: 100M URLs/month → ~40 URLs/sec.
- Read: 100:1 read-write ratio → 4000 reads/sec.
- Storage: 100M/month * 12 * 5 years * 500 bytes ≈ 6 TB.
- Short URL length: 7 chars base62 ≈ 3.5 trillion combos.

---

## High-Level Design

```
Client → API Server → Write Path: [Hash Service] → [DB (long URL, short key)]
 [Cache]
 Client → API Server → Read Path: [Cache Lookup] → hit: return
 miss: [DB Lookup] → cache + return
```

## Component Design

### 1. Short URL Generation

**Option A: Hashing**
- MD5/SHA256 of long URL → take first N chars → base62.
- **Problem:** Collisions; need to handle or check.

**Option B: Counter (Preferred)**
- A dedicated `Counter Service` (e.g., Zookeeper/Redis INCR).
- Generate ID → base62 encode.
- Pros: No collisions, IDs are unique.
- Cons: Single point of failure (mitigated with multi-master counters or Twitter Snowflake).

**Base62:** `0-9 a-z A-Z` → 62 chars. For length 7: 62^7 ≈ 3.5 trillion.

### 2. Storage
- **Primary DB:** Store `short_key → long_url` mapping.
 - Could be NoSQL (DynamoDB, Cassandra) for scale.
- **Cache:** Redis/Memcached for hot URLs.
 - Cache miss → DB → populate cache.

### 3. Read Path (Redirection)
- Client hits `https://short.url/{key}`.
- Lookup cache → if hit, 301 redirect.
- Else lookup DB → cache + 301 redirect.
- 301 (permanent) vs 302 (temporary): 301 is cached by browser, 302 forces server lookup (better analytics).

---

## Schema

```
Table: url_mapping
 short_key VARCHAR(7) PRIMARY KEY
 long_url TEXT NOT NULL
 created_at TIMESTAMP
 expires_at TIMESTAMP NULL
 user_id INT NULL
 clicks BIGINT
```

---

## Scalability Considerations

- **Sharding:** Shard by `short_key` hash.
- **Cache:** LRU with TTL or based on click frequency.
- **Analytics:** Async via Kafka → data warehouse.
- **Rate limiting:** Token bucket per IP/user.
- **Custom aliases:** Validate uniqueness.

---

## Follow-up Questions

**Q: How to handle custom aliases?**
Store `custom_alias` as an alternate key in DB. Lookup by it too.

**Q: How to expire URLs?**
Lazy expiration (check on read) + periodic cleanup job.

**Q: How to support analytics?**
Async events to Kafka → batch process in Hadoop/Spark → dashboard.

**Q: How to handle hot URLs?**
Cache heavily. Multi-region replicas.

---

## Diagram
```
 ┌──────────────┐
 │ Counter │ (Snowflake / Redis INCR)
 └──────┬───────┘
 │ base62 ID
 ▼
┌────────┐ write ┌────────────┐ write ┌──────────┐
│ Client │ ───────────► │ API Server │ ──────────► │ Database │
└────────┘ └─────┬──────┘ └──────────┘
 │ │ read (with cache)
 │ 301 Redirect ▼
 └────────────────── ┌─────────┐
 │ Cache │ (Redis)
 └─────────┘
```
