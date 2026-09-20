# Design Pastebin (paste.textbin.com style)

Tests your ability to design a small but globally distributed content store. Similar to URL shortener but with bigger payloads.

---

## 1. Requirements

### Functional
- Create a paste (text + optional title + syntax).
- Read a paste via a short URL.
- Optional expiration (burn-after-read, time-based).
- Optional password / private.

### Non-Functional
- High availability.
- Low latency reads.
- Reasonable durability.
- Out of scope: editing, accounts (v1).

---

## 2. Capacity Estimation

100M pastes/month, avg 10 KB each, 10:1 read:write.

| Metric | Value |
|---|---|
| Writes/sec (avg) | ~40 |
| Reads/sec (avg) | ~400 |
| Storage / month | 100M × 10 KB = **1 TB** |
| Storage / 5 years | ~60 TB |

---

## 3. High-Level Design

```
 Client ─► API ─► Paste Service ─► DB
 │
 └─ Read: API → Cache → DB
```

Same pattern as URL shortener, with bigger values.

---

## 4. Schema

```
Table: pastes
 paste_id VARCHAR(10) PK
 content TEXT
 syntax VARCHAR(50)
 created_at TIMESTAMP
 expires_at TIMESTAMP NULL
 burn_after_read BOOL
 password_hash VARCHAR NULL
 view_count BIGINT
```

`paste_id` = 10-char base62 (≈ 8 × 10^17 combos).

---

## 5. Storage

- SQL (Postgres) for metadata + content up to 1 MB.
- For larger pastes, store content in object storage (S3) and reference URL.

### Sharding
- Shard by hash of `paste_id`.
- Read replicas for global reads.

---

## 6. Expiration

- **Lazy:** check `expires_at` on read.
- **Background sweep:** daily job deletes expired rows.

Burn-after-read: mark a flag; delete on first successful read.

---

## 7. API

```
POST /api/v1/pastes
 Body: { content, syntax, expires_in?, password? }
 → 201 { "paste_id": "...", "url": "..." }

GET /{paste_id}
 → 200 { content, syntax, view_count }

DELETE /{paste_id} (with secret)
 → 204
```

---

## 8. Optimizations

- CDN for popular pastes.
- Compress content with gzip on the wire.
- Syntax highlighting client-side (lighter than server-side).
- Cache top pastes in Redis.

---

## 9. Failure Modes

- DB down → serve from cache; reject new pastes.
- Burn-after-read race → idempotent delete; rely on DB row lock.

---

## 10. Follow-up Questions

**Q: How to prevent abuse?**
CAPTCHA, rate limit by IP, block known spam phrases, ML classifier.

**Q: How to support syntax highlighting?**
Client-side library (Prism / highlight.js) — keeps server simple.

**Q: How to scale beyond 1 MB pastes?**
Store content in S3, save URL in DB; cap DB row at 1 MB.

**Q: How to support collaboration / live edits?**
Use operational transforms or CRDTs (Yjs, Automerge) over WebSocket — significant complexity bump; out of v1 scope.

---

## 11. End-to-End Diagram

```
 Client ─► API Server ─► Postgres (metadata + content)
 │ │
 │ │ miss
 ▼ ▼
 Redis cache ───────► Postgres
```
