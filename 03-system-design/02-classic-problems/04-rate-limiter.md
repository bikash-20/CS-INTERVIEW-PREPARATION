# Design Rate Limiter

## Requirements

### Functional
- Limit requests per user/IP/API key.
- Configurable limits (e.g., 100 requests/minute).
- Return 429 (Too Many Requests) when exceeded.

### Non-Functional
- Low latency.
- High availability.
- Distributed (works across multiple servers).

---

## Where to Place

1. **Client-side** — easily bypassed, only for UX hints.
2. **Server-side (in app)** — simple but each server has its own counter.
3. **Middleware / API gateway** — preferred; central.
4. **Cloud:** AWS API Gateway, Cloudflare, Kong, Envoy.

---

## Algorithms

### 1. Token Bucket
- Bucket holds N tokens; refilled at rate R.
- Each request consumes 1 token.
- If empty → reject.
- **Pros:** Allows bursts up to bucket size.
- **Use:** Most popular; used by AWS, Stripe.

### 2. Leaky Bucket
- Requests added to queue; processed at fixed rate.
- Overflow → drop.
- **Pros:** Smooth output rate.
- **Use:** Network shapers.

### 3. Fixed Window Counter
- Count requests per fixed time window (e.g., minute).
- Reset at window boundary.
- **Pros:** Simple.
- **Cons:** Boundary spike — 2x allowed rate at boundaries.

### 4. Sliding Window Log
- Store timestamps of all requests.
- Count requests within last N seconds.
- **Pros:** Accurate.
- **Cons:** Memory-heavy.

### 5. Sliding Window Counter
- Hybrid: weighted combination of current + previous window.
- **Pros:** Good accuracy + low memory.

---

## High-Level Design

```
Client → Load Balancer → API Gateway (with rate limiter) → Services
 │
 ▼
 ┌──────────┐
 │ Redis │ (counters, sorted sets)
 └──────────┘
```

## Implementation

### Token Bucket in Redis (Lua for atomicity)

```lua
-- KEYS[1]: bucket key
-- ARGV[1]: capacity, ARGV[2]: refill rate (tokens/sec), ARGV[3]: now (sec), ARGV[4]: tokens to consume
local data = redis.call('HMGET', KEYS[1], 'tokens', 'ts')
local tokens = tonumber(data[1]) or tonumber(ARGV[1])
local last_ts = tonumber(data[2]) or tonumber(ARGV[3])
local delta = math.max(0, tonumber(ARGV[3]) - last_ts)
tokens = math.min(tonumber(ARGV[1]), tokens + delta * tonumber(ARGV[2]))

local allowed = 0
if tokens >= tonumber(ARGV[4]) then
 tokens = tokens - tonumber(ARGV[4])
 allowed = 1
end

redis.call('HMSET', KEYS[1], 'tokens', tokens, 'ts', ARGV[3])
redis.call('EXPIRE', KEYS[1], 3600)
return allowed
```

---

## Distributed Considerations

- **Race conditions:** Multiple servers updating counter. Use Lua scripts (atomic in Redis) or distributed locks.
- **Synchronization:** Each request hits Redis — single source of truth.
- **Performance:** Redis is in-memory → very fast (~100k ops/sec per node).

---

## What to Limit On

| Key | Use Case |
|-----|----------|
| User ID | Authenticated API |
| IP | Public endpoints |
| API Key | Third-party developers |
| Endpoint | Protect heavy operations |
| Combination | E.g., `(user_id, /upload)` |

---

## Response Headers

```
X-RateLimit-Limit: 100
X-RateLimit-Remaining: 23
X-RateLimit-Reset: 1633024800

# On 429:
Retry-After: 30
```

---

## Follow-up Questions

**Q: How to handle distributed clock skew?**
Use Redis server time as source of truth (pass `TIME` command into Lua).

**Q: How to limit per endpoint differently?**
Different rules per route; store rules in config / DB.

**Q: How to make limits dynamic per user (paid tiers)?**
Look up user plan at limiter check; apply rule.

**Q: How to handle CDN/WAF rate limiting vs app?**
Layer them: CDN blocks obvious abuse (L7 rules), app handles business limits.
