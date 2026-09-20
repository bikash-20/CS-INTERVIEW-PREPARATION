# Design URL Shortener (TinyURL / bit.ly)

The classic "first system design" question. Tests whether you can drive a structured conversation from requirements to scale.

---

## 1. Requirements

### Functional
- Given a long URL, return a unique short URL.
- Visiting a short URL redirects to the original long URL.
- Optional: custom alias, expiration, analytics (clicks), user accounts.

### Non-Functional
- High **availability** — the redirect is on the hot path.
- Low **latency** — redirect should be < 100 ms.
- Highly **scalable** — billions of URLs over time.
- **Durability** — once created, a short URL must keep working for years.

### Out of scope (v1)
- Analytics dashboards, abuse detection (v2).

---

## 2. Capacity Estimation

Assume 100M new URLs / month, 100:1 read:write ratio.

| Metric | Value |
|---|---|
| Writes/sec (avg) | 100M / 30d / 86400 ≈ **40 URLs/sec** |
| Writes/sec (peak, 5×) | ≈ **200 URLs/sec** |
| Reads/sec (avg) | 40 × 100 = **4,000 reads/sec** |
| Reads/sec (peak) | ≈ **20,000 reads/sec** |
| Storage / year | 100M × 12 × 500 B ≈ **600 GB** |
| Storage / 5 years | ≈ **3 TB** |
| Short URL length | 7 chars in base62 → 62^7 ≈ **3.5 trillion** combos |

That's plenty of room.

---

## 3. High-Level Design

```
 Client ─► Load Balancer ─► API Servers
 │
 ├─ Write path: API → ID Service → DB
 │ │─ Cache (Redis, recent keys)
 │
 └─ Read path: API → Cache → (miss) → DB → 301/302 Redirect
```

Two paths matter:
- **Write path** — generate a unique short key, persist `short → long` mapping.
- **Read path** — given a short key, return the long URL with HTTP 301/302.

---

## 4. Short Key Generation

### Option A — Hashing the long URL
- MD5 / SHA-256, take first 7 base62 chars.
- **Cons:** collisions; need to check or retry.
- **Pros:** stateless, no coordination.

### Option B — Auto-increment counter (preferred)
- Dedicated counter service (Redis INCR, Zookeeper, or Twitter Snowflake).
- Convert counter value to base62.
- **Pros:** unique by construction.
- **Cons:** counter is a critical SPOF → mitigate with multiple counters or Snowflake.

### Option C — Pre-generated keys
- A background worker pre-generates a pool of random keys; checks uniqueness lazily.
- Smooths the write path.

### Base62 encoding
`0-9 a-z A-Z` → 62 characters. Length 7 gives 3.5 trillion combinations.

---

## 5. Storage

### Primary store — DB
| Choice | Pros | Cons |
|---|---|---|
| SQL (Postgres) | Simple, transactional | Vertical scale ceiling |
| DynamoDB / Cassandra | Horizontal scale, fast reads | Eventual consistency by default |

Schema:
```
Table: url_mapping
 short_key VARCHAR(7) PRIMARY KEY
 long_url TEXT NOT NULL
 created_at TIMESTAMP
 expires_at TIMESTAMP NULL
 user_id BIGINT NULL
 click_count BIGINT DEFAULT 0
```

### Cache — Redis / Memcached
- Cache recent / hot short → long mappings.
- TTL or LRU eviction.
- Cache key = `short_key`, value = long URL.

### Sharding
- Shard by hash of `short_key`.
- 10-20 shards is enough for the write volume above.

### Replication
- Each shard: 1 leader + 2 followers for reads.

---

## 6. Read Path (Redirection)

1. Client requests `https://short.url/{key}`.
2. API looks up cache.
3. On hit → return 301/302 with `Location: <long_url>`.
4. On miss → query DB → populate cache → redirect.
5. Async fire-and-forget click event → analytics.

### 301 vs 302
- **301 (Permanent):** browser caches forever. Better performance, weaker analytics.
- **302 (Temporary):** browser always re-checks. Better analytics, slightly slower.

Most shorteners use **302**.

---

## 7. Write Path

1. Client POSTs long URL + (optional) custom alias to API.
2. API asks ID Service for a unique short key.
3. ID Service increments counter, base62 encodes.
4. API writes `(short, long)` to DB.
5. API returns short URL to client.

---

## 8. Optimizations

- **Cache hot URLs aggressively** — top 20% of URLs get 80% of clicks.
- **Bloom filter** in front of DB to fast-reject bad keys.
- **Multi-region read replicas** — redirect is global.
- **Precompute** likely-to-be-popular URLs (e.g., trending in news).
- **HTTP caching** at CDN for static `robots.txt`, landing page.
- **Rate limit** writes per IP / user.
- **Custom alias validation** — block reserved words, check uniqueness with DB unique index.

---

## 9. Analytics (out of scope for v1)

- Async: write a click event to Kafka.
- Batch consumer writes to warehouse (BigQuery, Snowflake).
- Dashboard: clicks per hour, geo, referrer.

---

## 10. API Design

```
POST /api/v1/urls
 Body: { "long_url": "...", "custom_alias": "blog" | null }
 → 201 { "short_url": "https://short.url/x4kB9p" }

GET /{short_key}
 → 302 Location: <long_url>

GET /api/v1/urls/{short_key}/stats
 → 200 { "clicks": 124, "created_at": "...", "long_url": "..." }

DELETE /api/v1/urls/{short_key}
 → 204
```

Headers: `Idempotency-Key: <uuid>` for safe retries on POST.

---

## 11. Failure Modes

| Failure | Mitigation |
|---|---|
| Cache down | DB fallback (slower but works). |
| DB down | Serve from cache; reject writes. |
| Counter service down | Fail writes; alarm. |
| ID collision | DB unique constraint + retry generation. |
| Hot key | Replicate across cache nodes, add jitter to TTL. |

---

## 12. Follow-up Questions

**Q: How to handle custom aliases?**
Treat as alternate key in DB. Lookup by alias or short_key.

**Q: How to expire URLs?**
Lazy expiration (check on read) + periodic cleanup job.

**Q: How to support analytics?**
Async event to Kafka → batch ETL → warehouse.

**Q: How to detect abuse / malware?**
- Integrate Google Safe Browsing or similar.
- Block known bad long URLs at write time.
- Rate limit + CAPTCHA on write endpoint.

**Q: Why not just hash the URL?**
Hash collisions waste keys and force retries. Auto-increment is simpler and unique by construction.

**Q: How to make redirects faster than the DB round trip?**
Cache hot keys in Redis or even in-process LRU on each API server.

**Q: How to keep storage small for 5+ years?**
- Compress `long_url` (gzip in DB or just rely on TEXT compression).
- Move expired rows to cold storage (S3 + Athena queries).
- Drop `click_count`; recompute from events.

**Q: Multi-region?**
Yes — primary in one region for writes, read replicas in many regions. Cache at CDN edges.

---

## 13. End-to-End Diagram

```
 ┌──────────┐
 │ Counter │ (Redis INCR or Snowflake)
 └────┬─────┘
 │ base62 ID
 ▼
┌────────┐ POST /urls ┌──────────┐ write ┌──────────────┐
│ Client │ ──────────►│ API GW │ ─────► │ url_mapping │
└────┬───┘ └────┬─────┘ │ (SQL/NoSQL) │
 │ GET /{key} │ │
 │ ▼ │
 │ ┌─────────┐ miss │
 │ │ Redis │ ◄──────────┘
 │ └─────────┘ hit
 │ │
 └──────────── 302/301 ──► Client (browser) → Long URL
```
