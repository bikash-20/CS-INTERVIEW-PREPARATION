# Backend Interview Preparation — Comprehensive Guide

Complete, plain-English preparation for backend developer interviews. Covers **common backend concepts** (HTTP, REST, auth, DB, caching, queues, scaling), then three stack tracks with deep dives:

1. **Node.js + Express**
2. **Java + Spring Boot**
3. **Python + Django / FastAPI**

Plus coding challenges and a quick reference card. Every section has theory, interview Q&A, and runnable code.

---

## Table of Contents

- [Part 1 — Common Backend Concepts](#part-1--common-backend-concepts)
  - 1.1 [HTTP & REST](#11-http--rest)
  - 1.2 [API Design](#12-api-design)
  - 1.3 [Authentication & Authorization](#13-authentication--authorization)
  - 1.4 [Databases & Persistence](#14-databases--persistence)
  - 1.5 [Caching](#15-caching)
  - 1.6 [Message Queues & Async](#16-message-queues--async)
  - 1.7 [Scaling, Reliability, Observability](#17-scaling-reliability-observability)
  - 1.8 [Microservices & Distributed Systems](#18-microservices--distributed-systems)
- [Part 2 — Node.js + Express](#part-2--nodejs--express)
  - 2.1 [Core & Event Loop](#21-core--event-loop)
  - 2.2 [Express Basics](#22-express-basics)
  - 2.3 [Express Middleware & Auth](#23-express-middleware--auth)
  - 2.4 [Database, Validation, File Uploads](#24-database-validation-file-uploads)
  - 2.5 [Advanced (Clustering, Streams, Workers)](#25-advanced-clustering-streams-workers)
  - 2.6 [Testing, Logging, Monitoring](#26-testing-logging-monitoring)
- [Part 3 — Java + Spring Boot](#part-3--java--spring-boot)
  - 3.1 [Core Java for Interviews](#31-core-java-for-interviews)
  - 3.2 [Spring & Spring Boot Basics](#32-spring--spring-boot-basics)
  - 3.3 [Spring Data JPA & Transactions](#33-spring-data-jpa--transactions)
  - 3.4 [REST APIs & Validation](#34-rest-apis--validation)
  - 3.5 [Spring Security & JWT](#35-spring-security--jwt)
  - 3.6 [AOP, Caching, Async](#36-aop-caching-async)
  - 3.7 [Testing & Microservices](#37-testing--microservices)
- [Part 4 — Python + Django / FastAPI](#part-4--python--django--fastapi)
  - 4.1 [Python Essentials for Interviews](#41-python-essentials-for-interviews)
  - 4.2 [Django Fundamentals](#42-django-fundamentals)
  - 4.3 [Django REST Framework](#43-django-rest-framework)
  - 4.4 [FastAPI + Pydantic](#44-fastapi--pydantic)
  - 4.5 [Async Python & Celery](#45-async-python--celery)
- [Part 5 — Coding Challenges](#part-5--coding-challenges)
- [Quick Reference Card](#quick-reference-card)

---

## Part 1 — Common Backend Concepts

### 1.1 HTTP & REST

**Q1: What is REST?**
**A:** Representational State Transfer — an architectural style for networked apps using HTTP. Six constraints:
1. Client-server.
2. Stateless.
3. Cacheable responses.
4. Uniform interface.
5. Layered system.
6. (Optional) Code on demand.

**Q2: HTTP methods.**
- `GET` — read, idempotent, safe.
- `POST` — create, not idempotent.
- `PUT` — replace, idempotent.
- `PATCH` — partial update.
- `DELETE` — remove, idempotent.
- `OPTIONS`, `HEAD`, `TRACE`, `CONNECT`.

**Q3: Status code families.**
- `1xx` informational.
- `2xx` success (`200`, `201`, `202`, `204`).
- `3xx` redirect (`301`, `302`, `303`, `304`, `307`, `308`).
- `4xx` client error (`400`, `401`, `403`, `404`, `409`, `422`, `429`).
- `5xx` server error (`500`, `502`, `503`, `504`).

**Q4: Idempotency.**
A request that produces the same result no matter how many times it runs (after the first). `GET`, `PUT`, `DELETE` are idempotent by HTTP spec. `POST` is not — use `Idempotency-Key` for safe retries.

**Q5: REST best practices.**
- Plural nouns for collections (`/users`).
- Subresources for relations (`/users/42/orders`).
- Status codes properly.
- Pagination (`?limit=&offset=` or cursor).
- Versioning (`/v1/users` or header).
- HATEOAS optional (hypermedia links).

```http
POST /v1/orders            → 201 { id, status: "pending" }
GET  /v1/orders?limit=20   → 200 { items: [...], next: "..." }
PUT  /v1/orders/{id}       → 200
DELETE /v1/orders/{id}     → 204
```

**Q6: GET vs POST.**
`GET` body has no defined semantics and is logged in URLs — never put secrets there. `POST` for state-changing operations, with CSRF protection.

**Q7: What is CORS?**
Browser blocks JS from reading responses across origins unless the server sends CORS headers (`Access-Control-Allow-Origin`). Preflight `OPTIONS` for non-simple requests.

```http
Access-Control-Allow-Origin: https://mysite.com
Access-Control-Allow-Methods: GET,POST,PUT,DELETE
Access-Control-Allow-Headers: Content-Type, Authorization
Access-Control-Allow-Credentials: true
Access-Control-Max-Age: 600
```

**Q8: HTTPS / TLS handshake (short).**
1. Client Hello — supported ciphers, random.
2. Server Hello + cert.
3. Client verifies cert (CA chain).
4. Key exchange (ECDHE).
5. Both derive session key — all further traffic encrypted.

**Q9: HTTP/2 and HTTP/3?**
HTTP/2: binary framing, multiplexed streams, header compression. HTTP/3: uses QUIC (UDP) — faster handshake, immune to head-of-line blocking at transport.

**Q10: Long-polling vs SSE vs WebSocket.**
- **Long-poll** — client holds a request, server responds when there's data; client re-requests.
- **SSE** — server pushes over a single HTTP connection.
- **WebSocket** — bidirectional, separate protocol, persistent TCP.

---

### 1.2 API Design

**Q1: REST vs GraphQL vs gRPC.**
| | REST | GraphQL | gRPC |
|---|---|---|---|
| Style | Resources | Single endpoint, typed | Protobuf, HTTP/2 |
| Best for | Public APIs | Complex UIs | Service-to-service |
| Tooling | OpenAPI | Apollo, Hasura | protoc |
| Streaming | No | Subscriptions | Yes |

**Q2: Pagination — offset vs cursor.**
- **Offset** — `?page=2&size=20`. Simple; slow for deep pages; drifts under inserts.
- **Cursor** — `?cursor=eyJpZCI6MTJ9`. Stable, fast; no random access.

**Q3: OpenAPI / Swagger.**
Specification for HTTP APIs: paths, params, request/response schemas. Generate docs, server stubs, clients.

**Q4: Webhooks.**
Server-to-server callback. Sender retries with backoff; receiver must be **idempotent** and verify signatures (HMAC).

```http
POST /webhooks/github
X-Hub-Signature-256: sha256=...
Body: { "event": "...", "data": {...} }
```

**Q5: API versioning.**
- URL path (`/v1/users`) — simple, cacheable.
- Header (`Accept: application/vnd.myapi.v2+json`).
- Subdomain (`v2.api.com`).

**Q6: Rate limiting at the API level.**
Limit per API key / IP / user; use Redis counters; return 429 with `Retry-After`.

**Q7: Idempotency keys (in detail).**
Client sends `Idempotency-Key: <uuid>`. Server stores `(key → response)` for 24 h. Retry returns cached response.
- Critical for payments.
- Stores in Redis or DB unique index.

---

### 1.3 Authentication & Authorization

**Q1: Authn vs Authz.**
- **Authentication** — who are you?
- **Authorization** — what can you do?

**Q2: Password storage.**
NEVER plain. Use **bcrypt** or **argon2** with proper cost. Salt automatically. Never roll your own.

**Q3: JWT (JSON Web Token).**
Three parts: `header.payload.signature`. Self-contained, signed.

```js
const jwt = require("jsonwebtoken");
const token = jwt.sign({ userId: 1 }, secret, { expiresIn: "15m" });
const decoded = jwt.verify(token, secret);
```

Pros: stateless.
Cons: cannot revoke easily; size in headers; secret protection.

**Q4: Access token vs refresh token.**
- **Access token** — short-lived (15-60 min), used per request.
- **Refresh token** — long-lived (days/weeks), used only to mint new access tokens; stored in httpOnly cookie.

**Q5: OAuth 2.0 roles.**
- **Resource owner** — user.
- **Client** — app.
- **Authorization server** — issues tokens.
- **Resource server** — holds resources.

**Q6: OAuth 2.0 grant types.**
- **Authorization Code + PKCE** — for apps/SPA.
- **Client Credentials** — service-to-service.
- **Password** — legacy, internal only.
- **Implicit** — legacy, deprecated.

**Q7: OIDC.**
Identity layer on top of OAuth 2.0 — adds ID token (JWT with user profile) and standard userinfo endpoint.

**Q8: Session-based auth.**
Server keeps a session store (Redis or DB). Client gets a session cookie (httpOnly, secure, sameSite).

| | Cookies | Bearer JWT |
|---|---|---|
| Auto-sent | Yes | No |
| Revocable | Easy | Hard |
| CSRF | Risk | No risk (header) |
| XSS | httpOnly safe | localStorage risk |

**Q9: CSRF.**
State-changing requests from a logged-in user via an attacker page. Mitigations:
- SameSite cookies.
- CSRF tokens (hidden form input or header).
- Origin/Referer checks.

**Q10: RBAC vs ABAC vs ReBAC.**
- **RBAC** — roles (admin, user, viewer).
- **ABAC** — attributes (owner AND department === "X").
- **ReBAC** — relationships (Google Zanzibar, Auth0 FGA).

**Q11: Multi-tenant auth.**
- Tenant ID in JWT claim.
- Row-level security in DB.
- Per-tenant secrets / keys for sensitive tenants.

---

### 1.4 Databases & Persistence

**Q1: SQL vs NoSQL.**
| SQL | NoSQL |
|---|---|
| Fixed schema | Flexible |
| ACID | Often eventual |
| Vertical scale | Horizontal |
| SQL joins | Limited |
| Postgres, MySQL | Mongo, Cassandra, Redis, DynamoDB |

Pick SQL for transactions, joins, complex queries; NoSQL for scale, flexibility.

**Q2: ACID.**
- **Atomicity** — all-or-nothing.
- **Consistency** — valid state to valid state.
- **Isolation** — no interference.
- **Durability** — survives crash.

**Q3: Isolation levels.**
| Level | Dirty read | Non-repeatable | Phantom |
|---|---|---|---|
| Read uncommitted | Yes | Yes | Yes |
| Read committed | No | Yes | Yes |
| Repeatable read | No | No | Yes |
| Serializable | No | No | No |

**Q4: Connection pool.**
Reuse DB connections — opening one is expensive (TLS handshake, auth). Tune pool size (≈ # CPUs × 2).

**Q5: N+1 problem?**
ORM fetches a list (1 query), then each item's relation (N queries). Fix with `JOIN`/eager loading/`selectinload`/`include`.

**Q6: Indexes — what and when?**
Index columns used in `WHERE`, `JOIN`, `ORDER BY`. Composite index order matters: `(a, b, c)` covers `(a)`, `(a, b)`.

**Q7: When indexes hurt.**
- Write-heavy tables.
- Small tables.
- Low cardinality (e.g., gender).

**Q8: Transactions & locking.**
- `BEGIN`, `COMMIT`/`ROLLBACK`.
- `SELECT ... FOR UPDATE` for row locks.
- Deadlocks — DB detects via wait-for graph; kills victim.

**Q9: Replication.**
- Master-slave — read scaling.
- Multi-master — conflict resolution hard.
- Sync vs async.

**Q10: Sharding.**
Split rows by key (hash or range). Hard problems: cross-shard joins, hot keys, rebalancing.

**Q11: Choose a DB.**
| Need | Pick |
|---|---|
| Transactions, complex queries | Postgres / MySQL |
| Massive writes, simple reads | Cassandra, DynamoDB |
| Cache, sub-ms | Redis |
| Flexible schema | MongoDB |
| Search | Elasticsearch |
| Time-series | InfluxDB, Timescale |
| Graph traversal | Neo4j |

---

### 1.5 Caching

**Q1: Where to cache.**
Browser / CDN / reverse proxy (NGINX) / app LRU / Redis-Memcached / DB query cache.

**Q2: Strategies.**
- **Cache-aside** — app reads cache; on miss, DB; populate.
- **Write-through** — write to cache + DB synchronously.
- **Write-behind** — write to cache; async flush.
- **Refresh-ahead** — refresh before expiry.

**Q3: Eviction.**
LRU, LFU, TTL, ARC, W-TinyLFU.

**Q4: Cache stampede.**
Many concurrent cache misses → DB overload.
- Lock (only one fills).
- Probabilistic early refresh.
- Pre-warm.

**Q5: Cache invalidation.**
"Two hard things: cache invalidation, naming things, off-by-one errors."
- TTL.
- Event-driven invalidation.
- Versioned keys.

**Q6: Hot keys.**
One key gets massive traffic. Replicate across multiple cache nodes, jitter TTL, in-process L1.

**Q7: Cache penetration.**
Many requests for keys that don't exist. Mitigate with bloom filter in front of DB.

---

### 1.6 Message Queues & Async

**Q1: Why queues?**
- Decouple producer/consumer.
- Buffer spikes.
- Async work (email, video processing).
- Event-driven architecture.

**Q2: Delivery semantics.**
- **At-most-once** — fire-and-forget.
- **At-least-once** — retry until ack; may duplicate.
- **Exactly-once** — only with idempotent consumer + transactional writes.

**Q3: Common systems.**
- **Kafka** — log-based, replayable, partitioned.
- **RabbitMQ** — traditional broker, AMQP.
- **SQS** / **Pub/Sub** — managed.
- **NATS** — lightweight.

**Q4: Kafka basics.**
- Topics split into partitions (in-order per partition).
- Partition by key (hash) — same key always same partition.
- Consumers in groups share partitions.
- Offsets track position.
- Retention by time/size — replay old events.

**Q5: Idempotent consumer.**
Use the event's unique ID + dedup store to avoid processing twice.

**Q6: Dead letter queue (DLQ).**
Failed messages go to DLQ for inspection/replay. Prevents poison messages from blocking the queue.

**Q7: Backpressure.**
When consumer is slow, producer must slow down. Kafka: consumer pulls; RabbitMQ: prefetch limit.

---

### 1.7 Scaling, Reliability, Observability

**Q1: Vertical vs horizontal scaling.**
- Vertical — bigger box.
- Horizontal — more boxes; needs LB and sharding.

**Q2: Load balancing.**
Round robin, least connections, IP hash, consistent hashing. L4 (TCP) vs L7 (HTTP).

**Q3: Stateless services.**
Web servers should be stateless → trivially horizontal. Store session in Redis.

**Q4: Timeouts.**
Always set them. Per-dependency.

**Q5: Retries with backoff + jitter.**
Retry only transient (5xx, timeouts). Cap retries (3-5). Exponential backoff + jitter avoids thundering herd.

**Q6: Circuit breaker.**
After N failures, "open" — fail fast. After cool-down, half-open — try once.

**Q7: Bulkhead.**
Isolate resources per tenant — separate thread pools / connection pools.

**Q8: Graceful degradation.**
If a downstream is down, serve a simpler version. Cache + fallback response.

**Q9: CAP theorem.**
Under partition, choose CP (consistency) or AP (availability). Real systems pick explicitly.

**Q10: Observability.**
- **Metrics** — Prometheus, Datadog.
- **Logs** — ELK, Loki.
- **Traces** — Jaeger, Zipkin, OpenTelemetry.

**Q11: SLI / SLO / SLA.**
- SLI — metric.
- SLO — target.
- SLA — contract.

**Q12: Health checks.**
- **Liveness** — should I restart?
- **Readiness** — should I receive traffic?

**Q13: Logging best practices.**
- JSON structured.
- Include trace_id, request_id.
- Levels: debug, info, warn, error.
- Never log secrets / PII.

**Q14: Distributed tracing.**
OpenTelemetry instruments app; propagates trace context across services; backend joins spans.

**Q15: Chaos testing.**
Inject failures in production (latency, kill, network). Netflix Chaos Monkey. Always start with staging.

---

### 1.8 Microservices & Distributed Systems

**Q1: Microservices vs monolith.**
Microservices add network + ops overhead; use only when:
- Independent deploy needed.
- Different scaling profiles.
- Polyglot required.

**Q2: Service discovery.**
- DNS-based (Kubernetes).
- Registry (Consul, etcd).
- Client-side / server-side.

**Q3: API Gateway.**
Single entry for clients; routing, rate limit, auth, aggregation.

**Q4: Saga for distributed transactions.**
Long-lived transaction across services via compensating actions. Two flavors:
- **Choreography** — services exchange events.
- **Orchestration** — central saga coordinator.

**Q5: Outbox pattern.**
Write business + outbound event row in **same DB transaction**. A poller publishes to broker. Avoids losing events between DB and queue.

**Q6: Two-phase commit (2PC).**
Coordinator prepares both, then commits. Strong but blocking and rarely used in microservices.

**Q7: Idempotency.**
Same call → same result. Use `Idempotency-Key` header + store.

**Q8: Distributed locks.**
For cross-process / cross-host mutex. Redis (`SET NX PX`), Zookeeper, etcd.

**Q9: Fencing tokens.**
Monotonic tokens returned by lock service. Resource rejects stale writers.

**Q10: CQRS.**
Separate write model (commands) from read model (queries). Often paired with event sourcing.

**Q11: Event sourcing.**
Store state as a log of events; derive current state by replay.

**Q12: Webhooks & retries.**
Exponential backoff with jitter. Cap retries. Persistent queue.

**Q13: API security checklist.**
- TLS everywhere.
- Authentication on every request.
- Input validation.
- Output encoding.
- Rate limiting.
- Audit logging.
- WAF.

---

## Part 2 — Node.js + Express

### 2.1 Core & Event Loop

**Q1: Node.js architecture?**
Single-threaded JS on top of **libuv** (C++ async I/O). V8 for JS, libuv for the event loop + thread pool for blocking calls.

**Q2: Event loop phases.**
1. timers (`setTimeout`, `setInterval`)
2. pending callbacks
3. idle / prepare
4. poll (I/O)
5. check (`setImmediate`)
6. close callbacks
Microtasks (Promise, queueMicrotask) drain between each phase.

**Q3: `process.nextTick` vs `setImmediate` vs `setTimeout(0)`?**
- `process.nextTick` — runs **before** the event loop continues (very high priority; can starve I/O).
- `setImmediate` — runs in the **check** phase.
- `setTimeout(0, fn)` — runs in the **timers** phase.

**Q4: Blocking the event loop?**
Any synchronous CPU work blocks everyone. Use:
- `worker_threads` for CPU work.
- Streams for large I/O.
- Offload to native code via bindings.

**Q5: Streams?**
- Readable, Writable, Duplex, Transform.
- Backpressure: respect `highWaterMark`, listen for `drain`.
- `pipeline()` chains them and handles cleanup.

**Q6: `Buffer`?**
Node's binary data type (raw memory). Useful for files, network, crypto.

**Q7: CommonJS vs ESM?**
- **CommonJS** — `require`/`module.exports`. Default.
- **ESM** — `import`/`export`, async loading, tree-shake friendly.

```js
// commonjs
const fs = require("fs");
// esm
import fs from "node:fs";
```

**Q8: Global objects.**
`globalThis`, `process`, `Buffer`, `__dirname` (CJS), `console`, `setTimeout`, etc.

---

### 2.2 Express Basics

**Q1: Hello Express.**
```js
import express from "express";
const app = express();
app.use(express.json());
app.get("/health", (_req, res) => res.json({ ok: true }));
app.listen(3000);
```

**Q2: Middleware.**
A function with `(req, res, next)`. Runs in order.
- App-level: `app.use(logger)`.
- Router-level: `router.use(auth)`.
- Built-in: `express.json()`, `express.static()`.
- Third-party: cors, helmet, morgan.

**Q3: Order of middleware.**
Auth before handlers; error middleware last (4 args).

```js
app.use((err, req, res, next) => {
 console.error(err);
 res.status(500).json({ error: "Internal" });
});
```

**Q4: Route params.**
```js
app.get("/users/:id", (req, res) => res.json({ id: req.params.id }));
```

**Q5: Query strings.**
```js
req.query.page; // /users?page=2
```

**Q6: Async handlers.**
Express 5 supports async; Express 4 rejects on unhandled rejection.

```js
const wrap = (fn) => (req, res, next) =>
 Promise.resolve(fn(req, res, next)).catch(next);
app.get("/x", wrap(async (req, res) => { ... }));
```

**Q7: Static files.**
```js
app.use(express.static("public"));
```

**Q8: Routers.**
```js
const router = express.Router();
router.get("/", ...);
app.use("/api/users", router);
```

**Q9: `res` methods.**
`res.send()`, `res.json()`, `res.status(404).end()`, `res.redirect()`, `res.sendFile()`, `res.download()`, `res.cookie()`, `res.set()`.

**Q10: `helmet`, `cors`, `morgan`?**
- `helmet` — security headers.
- `cors` — CORS handling.
- `morgan` — request logging.

---

### 2.3 Express Middleware & Auth

**Q1: Custom logging middleware.**
```js
const logger = (req, res, next) => {
 const start = Date.now();
 res.on("finish", () => console.log(req.method, req.url, res.statusCode, Date.now() - start));
 next();
};
```

**Q2: JWT auth middleware.**
```js
import jwt from "jsonwebtoken";

export function auth(req, res, next) {
 const header = req.headers.authorization || "";
 const token = header.startsWith("Bearer ") ? header.slice(7) : null;
 if (!token) return res.status(401).end();
 try {
 req.user = jwt.verify(token, process.env.JWT_SECRET);
 next();
 } catch {
 res.status(401).end();
 }
}
```

**Q3: Role-based middleware.**
```js
const requireRole = (role) => (req, res, next) =>
 req.user?.role === role ? next() : res.status(403).end();
```

**Q4: Cookie session.**
```js
import session from "cookie-session";
app.use(session({ name: "s", keys: [process.env.SECRET], maxAge: 86400_000 }));
```

**Q5: Rate limiting middleware.**
```js
import rateLimit from "express-rate-limit";
app.use(rateLimit({ windowMs: 60_000, max: 100 }));
```

**Q6: Request validation with Zod.**
```js
import { z } from "zod";

const Body = z.object({ email: z.string().email(), age: z.number().int().min(0) });

app.post("/users", (req, res, next) => {
 const parsed = Body.safeParse(req.body);
 if (!parsed.success) return res.status(400).json(parsed.error.format());
 req.body = parsed.data;
 next();
});
```

**Q7: File upload with multer.**
```js
import multer from "multer";
const upload = multer({ dest: "uploads/" });
app.post("/upload", upload.single("file"), handler);
```

---

### 2.4 Database, Validation, File Uploads

**Q1: PostgreSQL with `pg`.**
```js
import { Pool } from "pg";
const pool = new Pool({ connectionString: process.env.DATABASE_URL });
const { rows } = await pool.query("SELECT * FROM users WHERE id = $1", [id]);
```

**Q2: ORM options.**
Prisma, TypeORM, Drizzle, Sequelize. Prisma is popular; Drizzle is fast and SQL-like.

**Q3: MongoDB with Mongoose.**
```js
import mongoose from "mongoose";
await mongoose.connect(process.env.MONGO_URL);
const User = mongoose.model("User", new mongoose.Schema({ name: String, email: String }));
```

**Q4: When to use an ORM?**
ORMs reduce boilerplate, add type safety, hide SQL. Trade-off: harder to optimize queries or run raw SQL. Use raw SQL when needed.

**Q5: Pagination.**
```js
const offset = (page - 1) * size;
SELECT * FROM users ORDER BY id LIMIT $1 OFFSET $2;
```

**Q6: Transactions in SQL.**
```js
const client = await pool.connect();
try {
 await client.query("BEGIN");
 await client.query("...");
 await client.query("COMMIT");
 } catch (e) { await client.query("ROLLBACK"); throw e; }
 finally { client.release(); }
```

**Q7: File streaming uploads to S3.**
Use busboy, or just hand a presigned URL to the client.

**Q8: Read S3 streams.**
```js
import { GetObjectCommand } from "@aws-sdk/client-s3";
const out = await s3.send(new GetObjectCommand({ Bucket, Key }));
out.Body.pipe(res);
```

**Q9: Email.**
`nodemailer` with SMTP, or providers like SES, SendGrid, Postmark.

---

### 2.5 Advanced (Clustering, Streams, Workers)

**Q1: Clustering.**
Use the `cluster` module to fork workers (one per CPU). Or run behind PM2 / systemd / k8s.

**Q2: Worker threads for CPU.**
```js
import { Worker } from "node:worker_threads";
new Worker("./cpu-task.js", { workerData: big });
```

**Q3: `child_process` for shelling out.**
```js
import { spawn } from "node:child_process";
const ls = spawn("ls", ["-la"]);
ls.stdout.on("data", d => console.log(d.toString()));
```

**Q4: Process management in production.**
PM2, systemd, Docker, Kubernetes. Handle SIGTERM for graceful shutdown.

**Q5: Graceful shutdown.**
```js
const server = app.listen(3000);
process.on("SIGTERM", async () => {
 server.close();
 await pool.end();
 process.exit(0);
});
```

**Q6: Streams — reading large files.**
```js
import { createReadStream } from "node:fs";
const stream = createReadStream("big.log");
stream.on("data", chunk => process(chunk));
```

**Q7: Piping streams.**
```js
import { pipeline } from "node:stream/promises";
await pipeline(readable, transform, writable);
```

**Q8: Handling backpressure.**
Listen for `drain` on the writable before resuming reads.

---

### 2.6 Testing, Logging, Monitoring

**Q1: Jest test example.**
```js
test("sum", () => expect(1 + 1).toBe(2));
```

**Q2: HTTP test with supertest.**
```js
import request from "supertest";
test("GET /users", async () => {
 const res = await request(app).get("/users");
 expect(res.status).toBe(200);
});
```

**Q3: Pino vs Winston.**
Pino is fast (JSON), Winston is more flexible. Use one; never `console.log` in production.

**Q4: Structured logging.**
```js
import pino from "pino";
const log = pino({ level: "info" });
log.info({ userId }, "user signed in");
```

**Q5: Monitor event loop lag.**
```js
const start = process.hrtime.bigint();
setImmediate(() => log.warn({ lag: Number(process.hrtime.bigint() - start) / 1e6 }));
```

**Q6: Health endpoints.**
- `/livez` — am I alive?
- `/readyz` — ready for traffic?

**Q7: Tracing with OpenTelemetry.**
`@opentelemetry/sdk-node` + auto instrumentations; export to OTLP / Jaeger.

---

## Part 3 — Java + Spring Boot

### 3.1 Core Java for Interviews

**Q1: `==` vs `.equals()` in Java.**
`==` checks reference identity. `.equals()` is value-based (depends on override).

**Q2: `final`, `finally`, `finalize`?**
- `final` — constant / no override.
- `finally` — cleanup in try/catch.
- `finalize` — deprecated; don't use.

**Q3: Pass-by-value?**
Java is pass-by-value. Object references are passed by value.

**Q4: Checked vs unchecked exceptions?**
- Checked — checked at compile; must handle (IOException).
- Unchecked — runtime (NullPointerException).

**Q5: `String` immutability?**
Yes. Hashes cached, safe for keys in HashMap. Use `StringBuilder` for many concats.

**Q6: `equals` and `hashCode` contract?**
Equal objects must have equal hashes. Hash-based collections break otherwise.

```java
@Override public boolean equals(Object o) {
 if (this == o) return true;
 if (!(o instanceof User u)) return false;
 return id == u.id && Objects.equals(name, u.name);
}
@Override public int hashCode() { return Objects.hash(id, name); }
```

**Q7: `HashMap` — internal?**
Array of buckets; each bucket is a linked list / tree (when bucket > 8 elements). Hash collisions degrade to O(log N).

**Q8: `ConcurrentHashMap`?**
Concurrent map with per-bucket locking (Java 7+) → better than `Hashtable`. Not for null keys/values.

**Q9: Threads & executors?**
`ExecutorService` pools threads. Choose:
- `newFixedThreadPool(n)` — bounded.
- `newCachedThreadPool` — elastic.
- `newScheduledThreadPool` — delayed.
- `newWorkStealingPool` — parallelism = # CPUs.

**Q10: `synchronized` vs `ReentrantLock`?**
`synchronized` — built-in, fair? No by default. Auto release on exception.
`ReentrantLock` — explicit `lock`/`unlock`, try-lock, condition vars, fairness.

**Q11: Java 21 — virtual threads (Project Loom)?**
Lightweight threads; millions viable. Great for I/O. Use `Executors.newVirtualThreadPerTaskExecutor()`.

**Q12: Records.**
Immutable data classes (Java 16+).

```java
public record User(String name, String email) {}
```

**Q13: Sealed classes (Java 17).**
Restrict subclasses.
```java
public sealed interface Shape permits Circle, Square {}
```

**Q14: Streams.**
```java
List<String> names = users.stream()
 .filter(u -> u.active())
 .map(User::name)
 .sorted()
 .toList();
```

**Q15: Optional.**
Wrap nullable; use `.map`, `.orElse`, `.orElseThrow`, `.filter`. Avoid `Optional` as field type.

---

### 3.2 Spring & Spring Boot Basics

**Q1: What is Spring?**
Application framework providing IoC (dependency injection), AOP, data access, MVC, security, etc.

**Q2: What is Spring Boot?**
Spring with auto-configuration and "convention over configuration". Standalone fat JAR with embedded Tomcat.

**Q3: Beans and DI.**
```java
@Service
public class UserService {
 private final UserRepo repo;
 public UserService(UserRepo repo) { this.repo = repo; } // constructor injection
}

@RestController
@RequestMapping("/users")
public class UserController {
 private final UserService svc;
 public UserController(UserService svc) { this.svc = svc; }
}
```

**Q4: Bean scopes.**
singleton (default), prototype, request, session, application.

**Q5: `@Configuration` and `@Bean`?**
```java
@Configuration
class AppConfig {
 @Bean
 public ObjectMapper mapper() { return new ObjectMapper(); }
}
```

**Q6: Stereotype annotations.**
`@Component` (generic), `@Service` (business), `@Repository` (DB, exceptions translated), `@Controller` / `@RestController` (web), `@Configuration`.

**Q7: Profiles.**
```java
@Service
@Profile("dev")
```
Run with `--spring.profiles.active=prod`.

**Q8: `@Autowired`?**
Optional in modern Spring — prefer constructor injection (no `@Autowired` needed).

**Q9: Bootstrapping.**
```java
@SpringBootApplication
public class App { public static void main(String[] a) { SpringApplication.run(App.class, a); }}
```

---

### 3.3 Spring Data JPA & Transactions

**Q1: JPA, Hibernate, Spring Data?**
- JPA — spec.
- Hibernate — implementation.
- Spring Data JPA — repo abstraction; reduces boilerplate.

**Q2: Entity.**
```java
@Entity
@Table(name = "users")
public class User {
 @Id @GeneratedValue(strategy = GenerationType.IDENTITY) private Long id;
 @Column(nullable = false, unique = true) private String email;
 private String name;
 // getters/setters/constructors
}
```

**Q3: Repository.**
```java
public interface UserRepo extends JpaRepository<User, Long> {
 Optional<User> findByEmail(String email);
}
```

**Q4: `@Transactional`?**
Wraps the method in a DB transaction.
- `propagation` — REQUIRED, REQUIRES_NEW, etc.
- `isolation`, `rollbackFor`, `readOnly`.

**Q5: N+1 fix.**
Use `@EntityGraph` or `JOIN FETCH` in JPQL.

```java
@EntityGraph(attributePaths = "orders")
List<User> findAll();
```

**Q6: Lazy vs eager loading.**
Default `@ManyToOne` is EAGER — often wrong. Make LAZY + fetch with join.

**Q7: Connection pool.**
Spring Boot defaults to HikariCP. Configure:
```yaml
spring.datasource.hikari.maximum-pool-size: 20
```

**Q8: Migrations.**
Flyway or Liquibase. SQL files in `src/main/resources/db/migration`.

```sql
-- V1__init.sql
CREATE TABLE users (
 id BIGINT PRIMARY KEY AUTO_INCREMENT,
 email VARCHAR(255) NOT NULL UNIQUE
);
```

---

### 3.4 REST APIs & Validation

**Q1: REST Controller.**
```java
@RestController
@RequestMapping("/api/users")
public class UserController {
 @GetMapping("/{id}")
 public ResponseEntity<User> get(@PathVariable Long id) {
 return userService.findById(id).map(ResponseEntity::ok).orElse(ResponseEntity.notFound().build());
 }
}
```

**Q2: Bean Validation.**
```java
public record CreateUserRequest(
 @NotBlank @Size(max = 100) String name,
 @NotBlank @Email String email
) {}
```
```java
@PostMapping
public ResponseEntity<User> create(@Valid @RequestBody CreateUserRequest req) { ... }
```

**Q3: Global exception handling.**
```java
@RestControllerAdvice
public class Errors {
 @ExceptionHandler(MethodArgumentNotValidException.class)
 public ResponseEntity<Map<String,String>> handle(MethodArgumentNotValidException ex) {
 return ResponseEntity.badRequest().body(Map.of("error", "validation"));
 }
}
```

**Q4: Pagination.**
```java
@GetMapping
public Page<User> list(Pageable p) { return repo.findAll(p); }
```

**Q5: DTO vs entity?**
Never return entities directly — leak DB schema. Use DTOs.

**Q6: OpenAPI / Swagger UI.**
Add `springdoc-openapi-starter-webmvc-ui`. Auto-generates docs from controllers.

---

### 3.5 Spring Security & JWT

**Q1: Spring Security basics.**
Adds a filter chain; you configure authentication, authorization, CORS, CSRF.

```java
@Configuration
@EnableWebSecurity
public class WebSecurityConfig {
 @Bean
 SecurityFilterChain chain(HttpSecurity http) throws Exception {
 return http.csrf(c -> c.disable())
 .authorizeHttpRequests(a -> a.requestMatchers("/public/**").permitAll().anyRequest().authenticated())
 .sessionManagement(s -> s.sessionCreationPolicy(SessionCreationPolicy.STATELESS))
 .build();
 }
}
```

**Q2: JWT authentication.**
- A filter reads `Authorization: Bearer ...`, validates the JWT, sets `SecurityContext`.
- On every request, security checks the principal and roles.

```java
@Component
public class JwtFilter extends OncePerRequestFilter {
 @Override
 protected void doFilterInternal(HttpServletRequest req, HttpServletResponse res, FilterChain chain) {
 String h = req.getHeader("Authorization");
 if (h != null && h.startsWith("Bearer ")) {
 // parse, set SecurityContextHolder
 }
 chain.doFilter(req, res);
 }
}
```

**Q3: Method security.**
```java
@PreAuthorize("hasRole('ADMIN')")
@DeleteMapping("/{id}")
public void delete(@PathVariable Long id) { ... }
```

**Q4: Password storage.**
Always `BCryptPasswordEncoder` (or Argon2).

```java
@Bean PasswordEncoder enc() { return new BCryptPasswordEncoder(); }
```

**Q5: OAuth2 Resource Server.**
Spring Security has built-in JWT resource-server support; just point at issuer URI.

```yaml
spring.security.oauth2.resourceserver.jwt.issuer-uri: https://auth.example.com
```

---

### 3.6 AOP, Caching, Async

**Q1: AOP — what?**
Cross-cutting concerns (logging, security checks) extracted into aspects.

```java
@Aspect @Component
public class Timing {
 @Around("@annotation(Timed)")
 public Object around(ProceedingJoinPoint pjp) throws Throwable {
 long t = System.nanoTime();
 try { return pjp.proceed(); }
 finally { log.info("{} {}ms", pjp.getSignature(), (System.nanoTime() - t) / 1e6); }
 }
}
```

**Q2: Spring Cache abstraction.**
```java
@Cacheable("users")
public User get(Long id) { return repo.findById(id).orElseThrow(); }
```
Enable with `@EnableCaching`. Use Redis backend for distributed.

**Q3: `@Async`.**
```java
@Async
public CompletableFuture<X> heavy() { ... }
```
Runs on a TaskExecutor pool. Caller gets a future.

**Q4: Retry with `@Retryable`.**
```java
@Retryable(value = TransientException.class, maxAttempts = 3, backoff = @Backoff(delay = 500, multiplier = 2))
```
Pair with `@EnableRetry`.

**Q5: Schedulers.**
```java
@EnableScheduling @Scheduled(cron = "0 0 * * * *")
```

**Q6: Reactive stack (WebFlux).**
For ultra-high concurrency, use `Mono`/`Flux`. Trade-off — JDBC drivers block; use R2DBC.

---

### 3.7 Testing & Microservices

**Q1: Unit testing.**
JUnit 5 + Mockito + AssertJ.

```java
@Test
void findsUser() {
 when(repo.findById(1L)).thenReturn(Optional.of(user));
 assertThat(svc.get(1L)).isEqualTo(user);
}
```

**Q2: `@SpringBootTest`.**
Loads full app context. Slow but realistic.

**Q3: `@WebMvcTest`.**
Loads only MVC layer.

**Q4: `@DataJpaTest`.**
Loads only JPA + embedded DB.

**Q5: Testcontainers.**
Real services in Docker for tests (Postgres, Kafka).

```java
@Container static PostgreSQLContainer<?> PG = new PostgreSQLContainer<>("postgres:16");
```

**Q6: REST integration tests.**
```java
@AutoConfigureMockMvc
class UserControllerIT {
 @Autowired MockMvc mvc;
 @Test void create() throws Exception {
 mvc.perform(post("/users").contentType(APPLICATION_JSON).content("{\"name\":\"a\"}"))
 .andExpect(status().isCreated());
 }
}
```

**Q7: Microservices with Spring Cloud.**
- **Eureka** — service registry.
- **Gateway** — single entry.
- **Config Server** — centralized config.
- **OpenFeign** — declarative HTTP clients.
- **Resilience4j** — circuit breaker / retry.

**Q8: Distributed tracing.**
Spring Sleuth + Zipkin, or OpenTelemetry auto-instrumentation.

**Q9: Saga with Spring?**
Spring State Machine + events; or use Axon / Temporal.

---

## Part 4 — Python + Django / FastAPI

### 4.1 Python Essentials for Interviews

**Q1: Mutable vs immutable?**
Immutable — int, float, str, tuple, frozenset.
Mutable — list, dict, set, bytearray.

**Q2: List comprehension.**
```python
[n * 2 for n in nums if n > 0]
```

**Q3: `*args`, `**kwargs`?**
```python
def f(*args, **kwargs): ...   # positional, keyword
```

**Q4: Decorators.**
```python
def log(fn):
 @wraps(fn)
 def wrap(*a, **kw):
 print(fn.__name__)
 return fn(*a, **kw)
 return wrap

@log
def add(x, y): return x + y
```

**Q5: Generators.**
```python
def gen():
 yield 1
 yield 2
g = gen()
next(g) # 1
```

**Q6: Context manager.**
```python
with open("f.txt") as f:
 data = f.read()
```

**Q7: GIL?**
Global Interpreter Lock — only one thread runs Python bytecode at a time. Use `multiprocessing` or async for concurrency.

**Q8: `is` vs `==`?**
`is` — same object (identity). `==` — same value. Use `is` for `None`, `==` for everything else.

**Q9: `__str__` vs `__repr__`?**
`__str__` — human-readable. `__repr__` — developer / unambiguous.

**Q10: Type hints + mypy.**
```python
def total(xs: list[int]) -> int: return sum(xs)
```

---

### 4.2 Django Fundamentals

**Q1: Django architecture.**
MTV: Model, Template, View. ORM in Model; HTTP handled in View; rendering in Template.

**Q2: Project layout.**
```
mysite/
 manage.py
 mysite/  (settings, urls, wsgi, asgi)
 app/
 models.py, views.py, urls.py, admin.py, migrations/
```

**Q3: Model.**
```python
class User(models.Model):
 email = models.EmailField(unique=True)
 name = models.CharField(max_length=100)
 created_at = models.DateTimeField(auto_now_add=True)
```

**Q4: Migrations.**
```bash
python manage.py makemigrations
python manage.py migrate
```

**Q5: Querying ORM.**
```python
User.objects.filter(email__endswith="@x.com").order_by("-created_at")[:20]
```

**Q6: Views — function vs class.**
- Function-based: simple, decorator-driven (`@login_required`).
- Class-based: DRY for common patterns (`ListView`, `CreateView`).

**Q7: URLs.**
```python
path("users/<int:id>/", views.user_detail)
```

**Q8: Templates.**
Django templates with `{{ var }}` and `{% for %}` / `{% if %}`.

**Q9: Static files.**
`{% load static %}` then `<img src="{% static 'logo.png' %}">`.

**Q10: Admin.**
```python
@admin.register(User)
class UserAdmin(admin.ModelAdmin):
 list_display = ("id", "email", "name")
```

**Q11: Forms.**
```python
class UserForm(forms.Form):
 email = forms.EmailField()
```

**Q12: Auth.**
`django.contrib.auth` — `User`, login/logout, `@login_required`, `request.user`.

**Q13: Settings.**
`DEBUG`, `ALLOWED_HOSTS`, `DATABASES`, `SECRET_KEY` (use env).

**Q14: Deployment.**
`gunicorn` or `uvicorn`; behind nginx; static via whitenoise; managed postgres.

---

### 4.3 Django REST Framework

**Q1: Serializer.**
```python
class UserSerializer(serializers.ModelSerializer):
 class Meta:
 model = User
 fields = ["id", "email", "name"]
```

**Q2: ViewSet.**
```python
class UserViewSet(viewsets.ModelViewSet):
 queryset = User.objects.all()
 serializer_class = UserSerializer
```

**Q3: Routing.**
```python
router = DefaultRouter()
router.register(r"users", UserViewSet)
urlpatterns += router.urls
```

**Q4: Permissions.**
```python
class IsOwner(BasePermission):
 def has_object_permission(self, request, view, obj):
 return obj.owner_id == request.user.id
```

**Q5: JWT with SimpleJWT.**
```python
REST_FRAMEWORK = {
 "DEFAULT_AUTHENTICATION_CLASSES": (
 "rest_framework_simplejwt.authentication.JWTAuthentication",
 ),
}
```

**Q6: Pagination.**
```python
REST_FRAMEWORK["DEFAULT_PAGINATION_CLASS"] = "rest_framework.pagination.PageNumberPagination"
REST_FRAMEWORK["PAGE_SIZE"] = 20
```

**Q7: Filtering.**
`django-filter` or query params in viewset.

**Q8: Throttling.**
```python
REST_FRAMEWORK["DEFAULT_THROTTLE_CLASSES"] = ("rest_framework.throttling.AnonRateThrottle",)
REST_FRAMEWORK["DEFAULT_THROTTLE_RATES"] = {"anon": "100/day"}
```

**Q9: APIView vs ViewSet.**
- `APIView` — explicit methods.
- `ViewSet` — bundles list/create/retrieve/update/destroy.

**Q10: Tests.**
```python
from rest_framework.test import APITestCase

class UserTests(APITestCase):
 def test_create(self):
 r = self.client.post("/users/", {"email": "a@b.c", "name": "a"}, format="json")
 self.assertEqual(r.status_code, 201)
```

---

### 4.4 FastAPI + Pydantic

**Q1: FastAPI hello.**
```python
from fastapi import FastAPI
app = FastAPI()

@app.get("/health")
def health(): return {"ok": True}
```

**Q2: Pydantic models.**
```python
from pydantic import BaseModel, EmailStr
class UserIn(BaseModel):
 name: str
 email: EmailStr
 age: int | None = None
```

Validation runs automatically; errors become 422.

**Q3: Path and query params.**
```python
from fastapi import Query

@app.get("/users/{id}")
def get(id: int, verbose: bool = Query(False)):
 return {"id": id, "verbose": verbose}
```

**Q4: Dependencies (DI).**
```python
from fastapi import Depends

def get_db():
 yield Session()
 return

@app.get("/items")
def items(db = Depends(get_db)): ...
```

**Q5: JWT in FastAPI.**
Use `fastapi-jwt-auth` or `python-jose`. OAuth2PasswordBearer for token URL.

**Q6: Database — SQLAlchemy / SQLModel / asyncpg.**
```python
class User(SQLModel, table=True):
 id: int | None = Field(default=None, primary_key=True)
 email: str
```

**Q7: Async endpoints.**
```python
@app.get("/items")
async def items(db: AsyncSession = Depends(get_db)):
 ...
```

**Q8: Background tasks.**
```python
from fastapi import BackgroundTasks

@app.post("/send")
def send(bg: BackgroundTasks, to: str):
 bg.add_task(send_email, to)
```

**Q9: OpenAPI / Swagger.**
Built-in. Visit `/docs` or `/redoc`.

**Q10: CORS.**
```python
from fastapi.middleware.cors import CORSMiddleware
app.add_middleware(CORSMiddleware, allow_origins=["*"], allow_methods=["*"], allow_headers=["*"])
```

---

### 4.5 Async Python & Celery

**Q1: `async def` / `await`?**
Cooperative concurrency with `asyncio`. Best for many I/O-bound tasks.

```python
async def main():
 async with httpx.AsyncClient() as c:
 r = await c.get("https://x")
```

**Q2: `asyncio.gather`?**
Run multiple coroutines concurrently.

```python
results = await asyncio.gather(fetch1(), fetch2(), fetch3())
```

**Q3: When to use Celery vs asyncio?**
- **Celery** — heavy background work, retries, scheduled tasks, many workers.
- **asyncio** — many small concurrent I/O in one process.

**Q4: Celery basics.**
```python
@celery.task
def send_email(to):
 ...

send_email.delay("a@b.c")
```

**Q5: Celery broker / backend.**
Broker — RabbitMQ / Redis. Backend — Redis / DB / S3 (for results).

**Q6: Django + Celery.**
Add celery to `INSTALLED_APPS`, autodiscover tasks, run workers via `celery -A app worker`.

**Q7: Periodic tasks.**
`celery beat` schedules; runs `delay()` on the schedule.

**Q8: Common gotchas.**
- Bound tasks: use `bind=True` to access `self`.
- Idempotency: assign `task_id`; dedupe.
- Result expiration: set a TTL or the result store fills up.

---

## Part 5 — Coding Challenges

### 1. Reverse a linked list
```python
def reverse(head):
 prev, cur = None, head
 while cur:
  nxt = cur.next
  cur.next = prev
  prev, cur = cur, nxt
 return prev
```

### 2. LRU Cache
```python
from collections import OrderedDict
class LRUCache:
 def __init__(self, cap):
  self.cap = cap
  self.d = OrderedDict()
 def get(self, k):
  if k not in self.d: return -1
  self.d.move_to_end(k)
  return self.d[k]
 def put(self, k, v):
  if k in self.d: self.d.move_to_end(k)
  self.d[k] = v
  if len(self.d) > self.cap: self.d.popitem(last=False)
```

### 3. Rate limiter (token bucket)
```js
class Limiter {
 constructor(cap, rate) { this.cap = cap; this.rate = rate; this.tokens = cap; this.last = Date.now(); }
 allow(now = Date.now()) {
  const elapsed = (now - this.last) / 1000;
  this.tokens = Math.min(this.cap, this.tokens + elapsed * this.rate);
  this.last = now;
  if (this.tokens >= 1) { this.tokens -= 1; return true; }
  return false;
 }
}
```

### 4. JWT auth (Express)
```js
function auth(req, res, next) {
 const t = req.headers.authorization?.split(" ")[1];
 try { req.user = jwt.verify(t, process.env.SECRET); next(); }
 catch { res.status(401).end(); }
}
```

### 5. Generate API key
```python
import secrets
def new_key(): return secrets.token_urlsafe(32)
```

### 6. Distributed counter (Redis Lua)
```lua
local v = redis.call("INCR", KEYS[1])
redis.call("EXPIRE", KEYS[1], 3600)
return v
```

### 7. Pagination SQL
```sql
SELECT id, name FROM users
WHERE id > $cursor
ORDER BY id
LIMIT $limit;
```

### 8. Idempotency middleware (Express)
```js
import { Redis } from "ioredis";
const redis = new Redis();
async function idempotent(req, res, next) {
 const k = req.header("Idempotency-Key");
 if (!k) return next();
 const got = await redis.get(`idem:${k}`);
 if (got) return res.status(409).json({ error: "duplicate" });
 await redis.set(`idem:${k}`, "1", "EX", 86400, "NX");
 next();
}
```

### 9. Background job handler
```python
import asyncio, aiohttp
async def fetch_all(urls):
 async with aiohttp.ClientSession() as s:
  async def one(u):
   r = await s.get(u)
   return await r.json()
  return await asyncio.gather(*(one(u) for u in urls))
```

### 10. Database migrations sketch
```sql
CREATE TABLE users (
 id BIGSERIAL PRIMARY KEY,
 email VARCHAR(255) NOT NULL UNIQUE,
 created_at TIMESTAMPTZ NOT NULL DEFAULT now()
);
CREATE INDEX users_email_idx ON users (email);
```

### 11. Pub/Sub fan-out (Spring)
```java
@Component
public class Notifier {
 @Autowired private ApplicationEventPublisher pub;
 public void notify(User u) { pub.publishEvent(new NotificationEvent(u)); }
}

@Component
public class PushListener {
 @EventListener
 public void on(NotificationEvent e) { push(e.user); }
}
```

### 12. Page cache
```python
from functools import lru_cache
import time

def cached(key_fn, ttl=60):
 def deco(fn):
  cache = {}
  def wrap(*args):
   k = key_fn(*args)
   now = time.time()
   hit = cache.get(k)
   if hit and now - hit[1] < ttl: return hit[0]
   v = fn(*args)
   cache[k] = (v, now)
   return v
  return wrap
 return deco
```

### 13. Circuit breaker
```python
import time
class Breaker:
 def __init__(self, threshold=5, cool=30):
  self.fail = 0
  self.threshold = threshold
  self.cool = cool
  self.opened_at = None
 def call(self, fn, *a, **k):
  if self.opened_at and time.time() - self.opened_at < self.cool:
   raise RuntimeError("circuit open")
  try:
   r = fn(*a, **k); self.fail = 0; return r
  except Exception as e:
   self.fail += 1
   if self.fail >= self.threshold: self.opened_at = time.time()
   raise
```

### 14. CORS handler (Express)
```js
app.use((req, res, next) => {
 const origin = req.headers.origin;
 if (["https://app.com", "https://admin.app.com"].includes(origin)) {
  res.setHeader("Access-Control-Allow-Origin", origin);
  res.setHeader("Access-Control-Allow-Credentials", "true");
  res.setHeader("Vary", "Origin");
 }
 res.setHeader("Access-Control-Allow-Methods", "GET,POST,PUT,DELETE,OPTIONS");
 res.setHeader("Access-Control-Allow-Headers", "Content-Type, Authorization");
 if (req.method === "OPTIONS") return res.sendStatus(204);
 next();
});
```

### 15. Outbox publisher (Java)
```java
@Scheduled(fixedDelay = 1000)
public void publishPending() {
 List<Outbox> rows = repo.findByStatus("PENDING");
 rows.forEach(o -> {
  kafka.send(o.topic(), o.payload());
  o.setStatus("SENT");
 });
}
```

---

## Quick Reference Card

| Concept | One-liner |
|---|---|
| HTTP method | Verb that defines intent (`GET`, `POST`, etc.) |
| Idempotent | Same result on multiple calls |
| Idempotency-Key | Unique ID for safe retries |
| CORS | Browser security for cross-origin |
| TLS / HTTPS | Encryption in transit |
| CDN | Edge cache for static content |
| JWT | Stateless signed token |
| OAuth 2.0 | Delegated authorization |
| OIDC | Identity layer on OAuth 2.0 |
| RBAC / ABAC / ReBAC | Authorization models |
| HMAC | Hash-based message authentication |
| UUID v4 | Random unique identifier |
| Snowflake ID | Time-ordered distributed ID |
| Idempotent producer | Kafka producer that doesn't duplicate |
| Saga | Distributed transaction pattern |
| Outbox pattern | Atomic DB + event write |
| CQRS | Split reads and writes |
| Event sourcing | State as event log |
| Consistent hashing | Resilient key distribution |
| Rate limiting | Throttle calls per identity |
| Circuit breaker | Stop failing calls when downstream is sick |
| Bulkhead | Isolate resources per tenant |
| Health checks | liveness vs readiness |
| Tracing | Spans + trace IDs across services |
| OpenTelemetry | Vendor-neutral instrumentation |
| ACID | DB transaction guarantees |
| CAP | Consistency / Availability / Partition |

---

Good luck with your backend interviews!
