# Design Rate Limiter

A focused, useful system that comes up in many interviews as either the main problem or a sub-component.

---

## 1. Requirements

### Functional
- Limit requests per user / IP / API key / endpoint.
- Different limits per tier (free, paid).
- Return HTTP 429 with `Retry-After` when exceeded.
- Standard headers showing remaining quota.

### Non-Functional
- Low latency overhead (≤ 1 ms).
- High availability — limiter failure should fail open (allow traffic) rather than fail closed (deny everything).
- Distributed — multiple servers should see a consistent view.
- Accurate even under bursts.

---

## 2. Where to Place

| Layer | Pros | Cons |
|---|---|---|
| **Client** | UX hint | Easily bypassed |
| **Server in-app** | Simple | Each server has own counter |
| **API Gateway / middleware** | Central, recommended | One more hop |
| **Cloud WAF (Cloudflare, AWS API GW)** | Global, edge | Limited logic |

Best practice: edge + app. Edge blocks obvious abuse; app enforces business limits.

---

## 3. Algorithms

### Token Bucket
- Bucket holds N tokens; refills at rate R.
- Request consumes 1 token.
- **Allows bursts** up to bucket size.
- Most popular (Stripe, AWS).

### Leaky Bucket
- Requests added to queue; processed at fixed rate.
- **Smooth output** regardless of input burst.

### Fixed Window Counter
- Count per time window (e.g., per minute).
- Simple.
- **Boundary spike:** up to 2× the limit across two windows.

### Sliding Window Log
- Store all timestamps; count entries in the last N seconds.
- **Most accurate.**
- Memory-heavy.

### Sliding Window Counter
- Weighted blend of current + previous window.
- **Good accuracy + low memory** — practical sweet spot.

### Comparison
| Algorithm | Memory | Accuracy | Burst handling |
|---|---|---|---|
| Token bucket | O(1) per key | Approx | Allows burst |
| Leaky bucket | O(1) per key | Approx | Smooths |
| Fixed window | O(1) per key | Low | 2× at boundary |
| Sliding log | O(N) per key | Exact | Allows burst |
| Sliding counter | O(1) per key | Good | Tunable |

---

## 4. High-Level Design

```
 Client
 │
 ▼
 Load Balancer
 │
 ▼
 API Gateway (with rate limit middleware)
 │
 ▼
 Redis (atomic counters / sorted sets)
 │
 ▼
 Microservices
```

Two key data structures in Redis:
- **Token bucket state:** `key = hash(user, route)` → `tokens`, `last_refill_ts`.
- **Sliding window log:** `key = hash(user, route)` → sorted set of timestamps.

---

## 5. Token Bucket in Redis (Lua, atomic)

```lua
-- KEYS[1]: bucket key
-- ARGV[1]: capacity, ARGV[2]: refill rate (tokens/sec),
-- ARGV[3]: now (sec), ARGV[4]: tokens requested (usually 1)
local data = redis.call('HMGET', KEYS[1], 'tokens', 'ts')
local tokens = tonumber(data[1])
local last_ts = tonumber(data[2])
local capacity = tonumber(ARGV[1])
local rate = tonumber(ARGV[2])
local now = tonumber(ARGV[3])
local cost = tonumber(ARGV[4])

if tokens == nil then
 tokens = capacity
 last_ts = now
end

local delta = math.max(0, now - last_ts)
tokens = math.min(capacity, tokens + delta * rate)

local allowed = 0
if tokens >= cost then
 tokens = tokens - cost
 allowed = 1
end

redis.call('HMSET', KEYS[1], 'tokens', tokens, 'ts', now)
redis.call('EXPIRE', KEYS[1], 3600)
return allowed
```

Atomic in Redis → no race conditions across app servers.

---

## 6. Sliding Window Log in Redis

```lua
-- KEYS[1]: key, ARGV[1]: now_ms, ARGV[2]: window_ms, ARGV[3]: limit
redis.call('ZREMRANGEBYSCORE', KEYS[1], 0, tonumber(ARGV[1]) - tonumber(ARGV[2]))
local count = redis.call('ZCARD', KEYS[1])
if count >= tonumber(ARGV[3]) then
 return 0
end
redis.call('ZADD', KEYS[1], tonumber(ARGV[1]), tonumber(ARGV[1]))
redis.call('PEXPIRE', KEYS[1], tonumber(ARGV[2]))
return 1
```

---

## 7. Limit Dimensions

Pick a tuple, e.g., `(user_id, /api/v1/upload)`.

| Key | Use |
|---|---|
| IP | Public endpoints |
| User ID | Authenticated APIs |
| API key | Third-party developers |
| Endpoint | Protect heavy ops |
| Combination | E.g., `(tenant_id, route)` |

---

## 8. Headers

```
X-RateLimit-Limit: 100
X-RateLimit-Remaining: 23
X-RateLimit-Reset: 1633024800

# On 429
Retry-After: 30
```

Clients can throttle themselves proactively.

---

## 9. Distributed Considerations

- **Single source of truth:** central Redis. App servers are stateless.
- **Fail open:** if Redis is unreachable, allow request (with a warning metric). Better than blocking all users on a cache outage.
- **Replica reads don't work for writes** — you need the primary or Redis cluster.
- **Local cache with jittered TTL** for some limits (less accurate but reduces Redis load).

---

## 10. Dynamic Limits per Tier

```
limits:
 free: 60/min
 pro: 1000/min
 enterprise: custom
```

API gateway looks up the user's tier, applies the right limit. Stored in a config service, hot-reloaded.

---

## 11. Common Patterns

### Limit on tokens / cost
Heavy endpoints cost more than lightweight ones (e.g., search = 5 tokens, ping = 1).

### Burst budget
Allow short bursts over the limit, but watch total over a longer window.

### Multi-window
Limit at multiple granularities: 10/sec, 100/min, 10k/day.

---

## 12. API for Limits Service

```
POST /admin/limits
 Body: { "scope": "user:42:/api/upload", "limit": 100, "window_ms": 60000 }
 → 201

GET /admin/limits?scope=...
 → 200 { "limit": 100, "used": 23, "reset_at": 1633024800 }
```

---

## 13. Follow-up Questions

**Q: How to handle clock skew across servers?**
Use Redis server time as the source of truth: pass `TIME` command output into the Lua script.

**Q: How to test this at scale?**
Replay traffic; pre-populate Redis with full buckets; measure latency under load.

**Q: How to limit per-endpoint differently?**
Store rules in a config service or DB; look up at request time.

**Q: How to handle tier changes mid-session?**
Apply new tier on next request; allow current burst to drain.

**Q: What about WebSocket / long connections?**
Limit connection-establish rate, message rate, and bytes/sec separately.

**Q: How to limit at edge (Cloudflare, AWS API GW)?**
Use built-in throttling primitives; sync tier config to edge periodically.

**Q: What if a single user is doing legitimate heavy traffic?**
Per-endpoint limits catch abuse without blocking bulk operations.

**Q: How to make rules hot-reload?**
Push to Redis pub/sub; API gateway subscribes and updates in-process.

**Q: Why not use database?**
DB round-trip is too slow (5-10 ms vs Redis 0.5 ms). DB won't survive the read QPS.

---

## 14. End-to-End Diagram

```
 Client
 │
 ▼
 Edge / WAF ──── coarse limit (IP, geo, abuse)
 │
 ▼
 API Gateway ──── fine limit (user, endpoint, tier)
 │
 ▼ (Lua atomic)
 Redis cluster
 │
 ▼
 Microservices
```
