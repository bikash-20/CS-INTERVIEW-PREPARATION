# System Design Fundamentals — Comprehensive

The core concepts that come up in **every** system design interview. Read this once, then come back to it before each round. Every section has **theory, key trade-offs, examples, and the most-asked follow-up questions**.

---

## Table of Contents

1. [How to Approach a System Design Interview](#1-how-to-approach-a-system-design-interview)
2. [Requirements & Estimation](#2-requirements--estimation)
3. [Scalability — Vertical vs Horizontal](#3-scalability--vertical-vs-horizontal)
4. [Load Balancing](#4-load-balancing)
5. [Caching](#5-caching)
6. [Proxies — Forward, Reverse, L4, L7](#6-proxies--forward-reverse-l4-l7)
7. [Storage Types — Block, File, Object](#7-storage-types--block-file-object)
8. [Databases — SQL vs NoSQL](#8-databases--sql-vs-nosql)
9. [Database Scaling — Replication & Sharding](#9-database-scaling--replication--sharding)
10. [Consistent Hashing](#10-consistent-hashing)
11. [CAP, PACELC, and Consistency Models](#11-cap-pacelc-and-consistency-models)
12. [Quorum, Raft, Paxos](#12-quorum-raft-paxos)
13. [Distributed Locks, Leader Election, Idempotency](#13-distributed-locks-leader-election-idempotency)
14. [Message Queues & Event-Driven Architecture](#14-message-queues--event-driven-architecture)
15. [Pub/Sub, Fan-out, Backpressure](#15-pubsub-fan-out-backpressure)
16. [CDN — Content Delivery Network](#16-cdn--content-delivery-network)
17. [API Design — REST, GraphQL, gRPC, Webhooks](#17-api-design--rest-graphql-grpc-webhooks)
18. [Microservices vs Monolith vs Modular Monolith](#18-microservices-vs-monolith-vs-modular-monolith)
19. [Bloom Filters, HyperLogLog, Count-Min Sketch](#19-bloom-filters-hyperloglog-count-min-sketch)
20. [Rate Limiting](#20-rate-limiting)
21. [Pagination — Offset vs Cursor](#21-pagination--offset-vs-cursor)
22. [Geo-DNS, Anycast, Multi-Region](#22-geo-dns-anycast-multi-region)
23. [Consistency Patterns — Cache Aside, Write Through, Read Through, Write Behind](#23-consistency-patterns--cache-aside-write-through-read-through-write-behind)
24. [Data Modeling for Scale](#24-data-modeling-for-scale)
25. [Security at Scale](#25-security-at-scale)
26. [Observability — Metrics, Logs, Traces, SLOs](#26-observability--metrics-logs-traces-slos)
27. [Resilience Patterns](#27-resilience-patterns)
28. [Storage Engines, Indexing, LSM vs B-Tree](#28-storage-engines-indexing-lsm-vs-b-tree)
29. [Search Systems](#29-search-systems)
30. [Streaming & Realtime](#30-streaming--realtime)
31. [Design Cheatsheet — Picking the Right Tool](#31-design-cheatsheet--picking-the-right-tool)

---

## 1. How to Approach a System Design Interview

### The 4-phase framework

Almost every senior interviewer expects you to walk through these phases in order.

**Phase 1 — Clarify requirements (3-5 min)**
- Functional: who are the users? what are the core actions?
- Non-functional: scale (QPS), latency, availability, durability.
- Out of scope: what is *not* in v1?

**Phase 2 — High-level design (5-10 min)**
- Draw boxes: client → LB → API → services → storage.
- Show the read and write paths separately.
- Call out the data flow, not the implementation detail.

**Phase 3 — Deep dive on the bottleneck (10-15 min)**
- The interviewer usually picks a piece ("how do you scale the feed?", "how does the read path work?").
- Drill into trade-offs: cache strategy, sharding, queue.

**Phase 4 — Wrap up (3-5 min)**
- Summarize.
- Mention monitoring, failure modes, future work.

### What strong candidates do
- Ask before designing.
- Drive the conversation: "Let me clarify X before drawing".
- Quantify: "100M URLs/month ≈ 40 writes/sec".
- Name the trade-offs explicitly.
- Mention failure: "if Redis is down, fall back to DB".

### What weak candidates do
- Jump to code without clarifying.
- Use buzzwords without explaining.
- Forget the read vs write path difference.
- Don't address failures.

---

## 2. Requirements & Estimation

### Functional vs Non-Functional
- **Functional** — what the system does (e.g., shorten URL, redirect).
- **Non-Functional** — quality attributes: latency, availability, scalability, durability, security, consistency.

### The "5 S's" of Non-Functional Requirements
| Letter | Quality | Example |
|--------|---------|---------|
| S | **Speed** | 95p latency < 200 ms |
| S | **Scale** | 100k QPS |
| S | **Safety** | 99.99% availability |
| S | **Stability** | degrades gracefully |
| S | **Security** | encrypted at rest & in transit |

### Back-of-the-envelope math
Always keep these numbers in mind:

| Quantity | Approx value |
|----------|--------------|
| 1 KB | 10^3 bytes |
| 1 MB | 10^6 bytes |
| 1 GB | 10^9 bytes |
| 1 TB | 10^12 bytes |
| 1 PB | 10^15 bytes |
| 1 day | ~10^5 seconds |
| 1 month | ~2.6 × 10^6 seconds |
| 1 year | ~3.15 × 10^7 seconds |
| Latency L1 cache | ~1 ns |
| Latency SSD random read | ~100 μs |
| Latency HDD seek | ~10 ms |
| Latency intra-AZ network | ~0.5 ms |
| Latency cross-country | ~50-100 ms |

### Power-of-two cheat
- 2^10 = ~1 thousand
- 2^20 = ~1 million
- 2^30 = ~1 billion
- 2^32 = ~4 billion
- 2^40 = ~1 trillion
- 2^64 = ~1.8 × 10^19

### Common interview questions to ask yourself
- How many **DAU** (daily active users)?
- Read:write ratio?
- Peak QPS vs average QPS?
- Average and p95/p99 latency budget?
- Storage for 5 years?
- Hot keys / heavy users?

### Example
Twitter: 200M DAU, 150M tweets/day.
- Average tweets/sec = 150M / 86400 ≈ 1700.
- Peak = 3-5× ≈ 5000-8000 writes/sec.
- Timeline reads are 100× writes.
- Storage: 200 bytes/tweet × 150M × 365 × 5 = ~55 TB/year.

---

## 3. Scalability — Vertical vs Horizontal

### Vertical scaling (scale up)
Add more CPU/RAM/disk to one box.
- **Pros:** Simple. No code change.
- **Cons:** Hardware ceiling, single point of failure, expensive at top end.
- **When:** databases (until you outgrow them), simple services.

### Horizontal scaling (scale out)
Add more boxes behind a load balancer.
- **Pros:** Theoretically unlimited, fault-tolerant, cheaper per unit.
- **Cons:** Coordination, distributed systems problems, network failures.
- **When:** web servers, stateless services.

### Stateless vs Stateful
- **Stateless** services (web API): trivially horizontal — just add more boxes.
- **Stateful** services (DBs, KV store): need replication, sharding, consensus.

### Auto-scaling
- **Reactive** — scale on a metric (CPU, queue depth).
- **Predictive** — pre-scale based on forecast.
- **Scheduled** — scale up at known peaks (e.g., 9am).

---

## 4. Load Balancing

### Layer
- **L4 (Transport):** routes on IP/port. Fast, low latency.
- **L7 (Application):** routes on URL, host, headers, cookies. More flexible.

### Algorithms
- **Round Robin** — even rotation.
- **Weighted Round Robin** — based on capacity.
- **Least Connections** — pick server with fewest open connections.
- **Least Response Time** — pick fastest server.
- **IP Hash** — sticky session (same client → same server).
- **Consistent Hash** — for caches; minimal disruption on add/remove.
- **Random** — surprisingly competitive at scale.

### Tools
- **Cloud:** AWS ALB (L7), NLB (L4), GCP LB, Azure LB.
- **Software:** NGINX, HAProxy, Envoy, Traefik, Istio.

### Health checks
- Active ping (HTTP `/healthz`, TCP).
- Remove unhealthy from rotation.
- Drain in-flight requests before removing.

### Global vs Local
- **GSLB (Global Server Load Balancing):** picks region based on geo/health.
- **Local LB:** picks server within a region.

---

## 5. Caching

### Where to cache
1. **Browser** — Cache-Control headers, ETag, Last-Modified.
2. **CDN** — static assets, sometimes API responses.
3. **Reverse proxy** — NGINX caching.
4. **Application** — in-process LRU.
5. **Distributed cache** — Redis, Memcached.
6. **DB** — query cache, materialized views, index-only scans.

### Eviction
- **LRU** (Least Recently Used) — most common.
- **LFU** (Least Frequently Used) — good for skewed access.
- **FIFO**, **TTL**, **ARC** (adaptive).
- **W-TinyLFU** (Caffeine default) — modern best-of-both.

### Problems
- **Cache stampede / thundering herd** — many concurrent misses.
  - Mitigation: locking, request coalescing, probabilistic early refresh.
- **Cold start** — empty cache after restart; pre-warm.
- **Hot keys** — one key gets massive traffic.
  - Mitigation: replicate the key across multiple cache nodes, in-process L1 cache, jitter the TTL.
- **Cache penetration** — many queries for non-existent keys.
  - Mitigation: bloom filter, return a sentinel.
- **Stale data** — cache holds old value after DB update.
  - Mitigation: TTL, write-through, event-driven invalidation.

---

## 6. Proxies — Forward, Reverse, L4, L7

### Forward proxy
Sits **in front of clients**. Used to reach the internet (corporate networks), hide client identity, cache egress.
Client → Forward Proxy → Internet.

### Reverse proxy
Sits **in front of servers**. Receives client requests, forwards them.
Client → Reverse Proxy → Servers.
Used for: TLS termination, caching, compression, load balancing, security, path-based routing.

Examples: NGINX, HAProxy, Envoy, AWS ALB, Cloudflare.

### L4 vs L7 proxy
| | L4 | L7 |
|---|---|---|
| Layer | TCP/UDP | HTTP |
| Speed | Fastest | Adds parsing |
| Routing | IP, port | URL, headers, cookies |
| Use | TCP services, gaming, video | HTTP APIs, web apps |

### Sidecar / Service mesh
A "smart" proxy (e.g., Envoy) deployed next to each service.
Adds mTLS, retries, observability, without changing app code.
Examples: Istio, Linkerd, Consul Connect.

---

## 7. Storage Types — Block, File, Object

| Type | Examples | Best for | Access |
|------|----------|----------|--------|
| **Block** | EBS, iSCSI, local SSD | Databases, OS disks | Raw volumes; one machine |
| **File** | EFS, NFS, HDFS | Shared files, logs | POSIX; multiple machines |
| **Object** | S3, GCS, Azure Blob | Images, videos, backups | HTTP API; unlimited scale |
| **Tape / Cold** | Glacier, Archive | Long-term archive | Slow, cheap |

### Pick by use case
- DB needs low-latency random I/O → block (EBS, local NVMe).
- Many services need shared files → NFS, EFS.
- Static assets, big blobs, backups → object (S3).

---

## 8. Databases — SQL vs NoSQL

### SQL (RDBMS)
Postgres, MySQL, SQL Server, Oracle.
- ACID, structured schema, joins, mature tooling.
- Vertical scaling has limits → master-slave, then sharding.

### NoSQL
- **Key-Value:** Redis, DynamoDB, Memcached.
  - Use: sessions, cache, leaderboards.
- **Document:** MongoDB, CouchDB.
  - Use: product catalogs, content management.
- **Column-Family:** Cassandra, HBase, Bigtable.
  - Use: time-series, write-heavy.
- **Graph:** Neo4j, JanusGraph, Neptune.
  - Use: social graph, recommendations.

### When to pick what
| Need | Pick |
|------|------|
| Transactions, complex joins | SQL |
| Massive write throughput, simple access | Cassandra / DynamoDB |
| Caching, sub-ms latency | Redis |
| Flexible schema, evolving entities | MongoDB |
| Search, faceted | Elasticsearch |
| Time-series | InfluxDB, Timescale, Cassandra |
| Graph traversal | Neo4j |

### NewSQL
Combines SQL with horizontal scale: CockroachDB, Spanner, YugabyteDB, TiDB.

---

## 9. Database Scaling — Replication & Sharding

### Replication
- **Master-Slave (Primary-Replica)** — writes to master, reads from replicas.
  - Pros: read scale, simple.
  - Cons: replication lag (eventual consistency), single master for writes.
- **Master-Master (Multi-Primary)** — writes to any node, conflict resolution needed.
  - Pros: write scale, no SPOF.
  - Cons: conflicts; rare in OLTP.
- **Synchronous** vs **Asynchronous** — sync = strong consistency, slower; async = fast, can lose writes on master crash.

### Sharding (Partitioning)
- **Horizontal** — split rows by hash/range of a key (e.g., `user_id % 256`).
- **Vertical** — split tables across DBs (e.g., `users` DB vs `orders` DB).
- **Directory-based** — lookup table mapping key → shard.

### Shard key choice
- High cardinality.
- Even distribution.
- Query pattern: most queries should hit one shard.
- Avoid "hot" shards (e.g., key = tenant_id for a huge tenant).

### Challenges
- Cross-shard joins/queries.
- Rebalancing when adding a shard.
- Hot shards.
- Sequence/auto-increment IDs across shards (use Snowflake, UUID, or dedicated sequence service).

### Replication factor
`N = 3` is typical: tolerates 1 failure.
`N = 5` for very important data.

---

## 10. Consistent Hashing

### Why
Modulo hashing (`key % N`) reshuffles **almost all keys** when N changes.
Consistent hashing reshuffles only **`K/N`** keys.

### How
- Hash keys and nodes onto a ring (`0 .. 2^32 - 1`).
- Each key belongs to the next clockwise node.
- Adding/removing a node only moves the keys between that node and its neighbor.

### Virtual nodes
Each real node owns many positions on the ring (e.g., 100-200 vnodes).
Improves balance: even with few real nodes, load is spread.

### Used by
- Distributed caches (Memcached, Redis Cluster).
- Dynamo-style KV stores (DynamoDB, Cassandra).
- CDN routing.

---

## 11. CAP, PACELC, and Consistency Models

### CAP
In a distributed store under partition, choose between:
- **C**onsistency — every read sees latest write.
- **A**vailability — every request gets a response.
- **P**artition tolerance — required (networks fail).

So real choice: **CP** or **AP**.

### PACELC
Extends CAP for the no-partition case: even when there is no partition, you trade between **Latency** and **Consistency**.
- DynamoDB / Cassandra: PA/EL (always available, eventual).
- Spanner / CockroachDB: PC/EC.

### Consistency models
- **Strong** — reads see latest committed write. Slow, expensive.
- **Linearizable** — strong + real-time ordering.
- **Sequential** — total order, but only per client.
- **Causal** — related events are ordered.
- **Read-your-writes** — user sees their own writes.
- **Eventual** — converges given time. Fast, scalable.
- **Monotonic reads** — once seen, never regress.

### Order from strongest to weakest
Linearizable > Sequential > Causal > Read-your-writes > Eventual.

---

## 12. Quorum, Raft, Paxos

### Quorum
For N replicas: `R + W > N` ensures reads see at least one updated copy.
- N=3, W=2, R=2: strong consistency, can survive 1 failure.

### Raft / Paxos
Consensus algorithms to keep a replicated log consistent across nodes.
- Leader elected.
- Leader replicates log entries to followers.
- Committed when majority acknowledges.
- Used by: etcd, Consul, CockroachDB, TiKV, Kafka (KRaft).

### Leader election
- Heartbeats keep the leader alive.
- If a follower misses heartbeats → becomes candidate → requests votes.
- Majority wins.

### Split brain
Two nodes both think they're leader. Mitigated by majority quorum and fencing tokens.

---

## 13. Distributed Locks, Leader Election, Idempotency

### Distributed lock
A mutex that works across processes/nodes.
- Redis: `SET key value NX PX <ttl>` (with token to release safely).
- Zookeeper: ephemeral sequential znodes.
- etcd: leases + revision numbers.
- Redlock: multi-Redis for stronger guarantees (controversial).

### Leader election
Only one node acts as the leader at a time.
- Zookeeper: ephemeral node with lowest sequence.
- etcd: lease + keepalive.
- Database row lock + heartbeat.

### Idempotency keys
Client supplies a unique key per operation. Server stores `(key → response)` for some TTL. Retry the same key returns the same response — safe to retry on network failure.

Use case: payments, order creation, anything with side effects.

### Fencing tokens
Monotonic tokens returned by the lock service. The resource checks the token to reject requests from a previous lock holder. Prevents stale writes from a paused process.

---

## 14. Message Queues & Event-Driven Architecture

### Why
- Decouple producer and consumer.
- Buffer spikes.
- Async work (email, video processing, ETL).
- Enable event-driven microservices.

### Concepts
- **Producer** — publishes events.
- **Broker** — holds events.
- **Consumer** — subscribes and processes.
- **Topic / Queue** — logical channel.
- **Offset** — position in a partition.

### Delivery semantics
- **At-most-once** — fire and forget. Lose on crash.
- **At-least-once** — retry until ack. Possible duplicates.
- **Exactly-once** — only with idempotent consumers + transactional writes.

### Ordering
- Per partition/key ordering.
- Global ordering is hard and slow.

### Popular systems
- **Kafka** — log-based, high throughput, replayable.
- **RabbitMQ** — traditional broker, AMQP.
- **AWS SQS / SNS** — managed, simple.
- **Google Pub/Sub**, **Azure Service Bus** — managed.

### Kafka in 60 seconds
- Topics split into partitions (ordered within a partition).
- Producers pick a partition by key (hash).
- Consumers in groups share partitions.
- Offsets tracked by consumer or broker.
- Retention by time/size — replay possible.

---

## 15. Pub/Sub, Fan-out, Backpressure

### Pub/Sub
- One event → many subscribers.
- Examples: Kafka topics with multiple consumer groups, SNS, Google Pub/Sub, Redis Pub/Sub.

### Fan-out
- **Write fan-out:** writing to many places (e.g., timeline cache for each follower).
- **Read fan-out:** pull from many places (e.g., aggregator reads from many shards).

### Backpressure
When a consumer is slow, the producer must slow down to avoid memory blowup.
- Kafka: consumer commits offset when done → naturally bounded.
- RabbitMQ: prefetch limit.
- gRPC streaming: flow control.

---

## 16. CDN — Content Delivery Network

### What
A geographically distributed network of edge servers that cache and serve content close to users.

### Use cases
- Static assets (images, JS, CSS, fonts).
- Video streaming (HLS/DASH).
- Static sites.
- Sometimes dynamic responses with edge compute (Cloudflare Workers, Lambda@Edge).

### How it works
1. Client requests `cdn.example.com/photo.jpg`.
2. Edge POP serves it if cached.
3. Otherwise, edge pulls from origin, caches, and serves.

### Cache key
Usually `host + path`. Can be customized (Vary headers, query strings).

### Eviction
TTL-based, LRU at the edge.

### Pitfalls
- Stale content after deploy → version in URL (`app.v2.js`).
- Sensitive content cached → set `Cache-Control: private, no-store`.
- Invalidation is hard → use content hashing in filename.

---

## 17. API Design — REST, GraphQL, gRPC, Webhooks

### REST
- Resource-oriented URLs (`/users/42`).
- HTTP methods as verbs (`GET`, `POST`, `PUT`, `PATCH`, `DELETE`).
- Stateless.
- Cacheable.
- Standard status codes.

### REST best practices
- Plural nouns for collections (`/users`, not `/user`).
- Use HTTP status codes properly (`200`, `201`, `204`, `400`, `401`, `403`, `404`, `409`, `429`, `500`).
- Use `?limit=20&offset=40` or cursor pagination.
- Versioning: `/v1/users` or header.
- Idempotency for POST (`Idempotency-Key`).

### GraphQL
- Single endpoint `/graphql`.
- Client specifies the shape of the response.
- Solves over-fetching and under-fetching.
- Tools: Apollo, Hasura, GraphQL Yoga.

### gRPC
- HTTP/2 + Protocol Buffers.
- Strongly typed contracts (`.proto`).
- Streaming support (unary, server, client, bidi).
- Best for internal microservice-to-microservice.

### Webhooks
- Server calls back a user-registered URL when an event happens.
- Used for: payment confirmation, GitHub events, CI/CD completion.
- Sender should retry with backoff; receiver should be **idempotent**.

### SOAP / XML
Legacy; rarely needed for new systems.

---

## 18. Microservices vs Monolith vs Modular Monolith

| Aspect | Monolith | Modular Monolith | Microservices |
|--------|----------|------------------|---------------|
| Deploy | One unit | One unit, modular boundaries | Many independent |
| Scale | Whole app | Whole app | Per-service |
| Tech | One stack | One stack | Polyglot |
| Failure | All-or-nothing | All-or-nothing | Isolated |
| Network overhead | None | None | Yes |
| Dev speed early | Fastest | Fast | Slowest |
| Dev speed at scale | Slow | Medium | Fast |

### When microservices
- Independent deploys needed for large teams.
- Different scaling profiles (some services need 100× more).
- Different tech stacks per service.

### When NOT
- Small teams (< 10 devs).
- Early product (uncertain boundaries).
- Use **modular monolith** first, split when boundaries are clear.

### Common microservice patterns
- API Gateway (Kong, Envoy, AWS API Gateway).
- Service discovery (Consul, etcd, K8s DNS).
- Circuit breaker (Hystrix, Resilience4j, Polly).
- Distributed tracing (Jaeger, Zipkin).
- Saga for distributed transactions.

---

## 19. Bloom Filters, HyperLogLog, Count-Min Sketch

### Bloom filter
Probabilistic set membership.
- False positives possible.
- False negatives impossible.
- Tiny space.
- Use: avoid cache miss, malicious URL detection, Cassandra SSTable lookup.

### HyperLogLog
Approximate **cardinality** (count distinct).
- 1.5 KB memory for billions of uniques with ~1% error.
- Use: unique visitors, distinct search terms.
- Redis has `PFADD`, `PFCOUNT`, `PFMERGE`.

### Count-Min Sketch
Approximate **frequency** of items in a stream.
- Use: top-K queries, count of how many times a user saw an ad.

### GeoHash / QuadTree
Encode lat/lng into a short string for fast proximity queries.

---

## 20. Rate Limiting

### Algorithms
- **Token bucket** — tokens refill at rate R, request consumes 1.
  - Allows bursts up to bucket size. Most popular. Stripe, AWS use this.
- **Leaky bucket** — process at fixed rate; queue overflows.
- **Fixed window counter** — count per time bucket; simple, can 2× at boundary.
- **Sliding window log** — store timestamps; very accurate; memory-heavy.
- **Sliding window counter** — weighted blend of current and previous window.

### Where
- **Client** — UX hints only.
- **Server** — every app instance; needs shared store.
- **API Gateway / WAF** — central, recommended.

### Limit on
- IP (public endpoints).
- User ID (authenticated).
- API key (third-party devs).
- Endpoint or combo.

### Distributed
- Central Redis with Lua script (atomic).
- Sliding window via Redis sorted sets.

### Headers
```
X-RateLimit-Limit: 100
X-RateLimit-Remaining: 23
X-RateLimit-Reset: 1633024800
Retry-After: 30
```

---

## 21. Pagination — Offset vs Cursor

### Offset
`GET /items?limit=20&offset=40`
- **Pros:** simple, can jump to any page.
- **Cons:** slow for deep pages, results shift if items are inserted/deleted.

### Cursor (keyset)
`GET /items?limit=20&after=<id>`
- **Pros:** fast even deep, stable under inserts.
- **Cons:** no random access, cursor opaque.

### Use
- Search engines, social feeds → cursor.
- Admin dashboards with "page 4231" → offset.

---

## 22. Geo-DNS, Anycast, Multi-Region

### Geo-DNS
DNS returns different IPs based on the client's region.
- Route EU users to EU cluster.
- Tools: Route 53, Cloudflare, NS1.

### Anycast
Many servers share the same IP. Routing sends each client to the **topologically closest** one.
- Used by DNS roots, CDN edges.

### Multi-region
- **Active-Passive** — primary region serves all writes; failover to standby.
- **Active-Active** — many regions serve reads + writes.
  - Hard: cross-region writes need conflict resolution.
- **Read-local, write-global** — read from local region, write to primary.

### Challenges
- Data residency (GDPR).
- Cross-region consistency.
- Higher latency for global services.

---

## 23. Consistency Patterns — Cache Aside, Write Through, Read Through, Write Behind

### Cache-Aside (Lazy Loading)
App reads cache; on miss, reads DB and populates.
```python
v = cache.get(key)
if v is None:
    v = db.get(key)
    cache.set(key, v, ttl=300)
return v
```
- App code controls cache.
- Risk: stale data after update.

### Read-Through
Cache library reads from DB on miss transparently (Caffeine, Redis with custom loader).
- Code is simpler.
- Same staleness risk.

### Write-Through
App writes to cache, cache writes synchronously to DB.
- Reads always fresh.
- Slower writes.

### Write-Behind (Write-Back)
App writes to cache; cache flushes async to DB.
- Fast writes.
- Risk of data loss if cache dies.

### Refresh-Ahead
Cache refreshes TTL'd entries before expiry (probabilistic).
- Prevents thundering herd.

---

## 24. Data Modeling for Scale

### Denormalization
Trade storage for speed. Common in NoSQL.
- Avoids joins at the cost of consistency.

### Materialized views
Pre-computed query results stored as tables.
- Updated by triggers or scheduled jobs.

### Indexes
- B-tree for range/equality.
- Hash for equality.
- Composite: order matters (`(a, b, c)` covers `(a)`, `(a,b)`, `(a,b,c)`).
- Covering index: contains all columns needed by the query.

### Partitioning (within one DB)
- By range (e.g., date).
- By hash.
- By list (e.g., region).

### Hot keys
Detect (top-N by QPS) and split: replicate the row, route by hash of tenant.

### Big rows / wide tables
- Split into multiple tables.
- Move blobs to object storage, keep pointer in DB.

---

## 25. Security at Scale

### Authentication
- **Passwords:** hash with bcrypt / argon2 (NEVER plain, NEVER MD5/SHA1).
- **MFA:** TOTP, WebAuthn, SMS.
- **SSO:** OIDC, SAML.

### Authorization
- **RBAC** — role-based (admin, user, viewer).
- **ABAC** — attribute-based (owner AND department=X).
- **ReBAC** — relationship-based (Google Zanzibar).

### Tokens
- **JWT** — self-contained, signed; cannot be revoked easily.
- **Opaque tokens** — stored server-side; revocable.
- **Refresh tokens** — long-lived; rotate.

### Encryption
- **At rest:** AES-256; KMS-managed keys.
- **In transit:** TLS 1.2+.
- **In use:** confidential computing (rare).

### Common attacks
- **SQLi** — use parameterized queries.
- **XSS** — escape output; CSP headers.
- **CSRF** — same-site cookies + tokens.
- **SSRF** — block private IPs in fetcher.
- **DDoS** — CDN + rate limit + WAF.
- **Credential stuffing** — MFA + bot detection.

### Secrets management
- AWS Secrets Manager / HashiCorp Vault.
- Never commit secrets; rotate; audit access.

### PII / Compliance
- GDPR, HIPAA, PCI-DSS.
- Right to delete, encryption, audit logs.

---

## 26. Observability — Metrics, Logs, Traces, SLOs

### The three pillars
- **Metrics** — aggregate numbers (CPU, QPS, latency).
- **Logs** — discrete events with context.
- **Traces** — request path across services.

### Metrics
- **Counter** — monotonic (requests total).
- **Gauge** — current value (queue depth).
- **Histogram** — distribution (latency).

### SLI / SLO / SLA
- **SLI** — the metric (e.g., p99 latency).
- **SLO** — the target (e.g., p99 < 200 ms over 30 days).
- **SLA** — the contract with customers (often ties to refunds).

### Error budget
`1 - SLO`. If you burn your budget early, you freeze deploys and fix things.

### Logging
- Structured JSON (`{"level":"info","trace_id":"..."}`).
- Sampling at high volume.
- PII redaction.

### Tracing
- OpenTelemetry — vendor-neutral SDK.
- Spans with trace IDs propagated across services.

### Tools
- **Metrics:** Prometheus, Datadog, CloudWatch.
- **Logs:** ELK, Loki, Splunk.
- **Traces:** Jaeger, Zipkin, Tempo, Datadog APM.
- **Dashboards:** Grafana.

### Alerting
- Alert on **symptoms**, not causes (e.g., `error_rate > 1%`, not `CPU > 80%`).
- Pages should be actionable; 1 page per week target.

---

## 27. Resilience Patterns

### Timeouts
- Always set them. Never make a request without one.

### Retries with exponential backoff + jitter
- Retry transient errors only (5xx, timeouts).
- Don't retry 4xx (except 408, 429).
- Cap retries at 3-5.

### Circuit breaker
- After N failures in a window, "open" the circuit → fail fast.
- After a cool-down, "half-open" → try one request → close or open.

### Bulkhead
- Isolate resources per tenant/endpoint (thread pool, connection pool).

### Rate limiting
See Section 20.

### Graceful degradation
Serve a simpler version when downstream is down (e.g., cached home page, no recommendations).

### Health checks
- Liveness: "am I alive?" — restart if no.
- Readiness: "can I serve traffic?" — remove from LB if no.

### Chaos engineering
Inject failures (latency, kill, network) in staging/prod (Netflix Chaos Monkey, Gremlin).

---

## 28. Storage Engines, Indexing, LSM vs B-Tree

### B-Tree
- Balanced tree on disk pages.
- Good for OLTP: point lookups, range scans, ordered scans.
- Used by: Postgres, MySQL InnoDB, Oracle.

### LSM (Log-Structured Merge)
- Writes go to in-memory memtable + append-only SSTable.
- Periodic compaction merges SSTables.
- Great for **write-heavy** workloads.
- Used by: RocksDB, LevelDB, Cassandra, HBase, Bigtable.

### Trade-offs
| | B-Tree | LSM |
|---|---|---|
| Write amp | Moderate | Higher (compaction) |
| Read amp | Lower | Higher (multiple SSTables) |
| Space amp | Lower | Higher (until compacted) |
| Range scan | Fast | OK |
| Writes | Slower | Faster |

### WAL (Write-Ahead Log)
Every write is appended to a log before being applied. Crash recovery replays the log.

---

## 29. Search Systems

### Inverted index
Map term → list of documents containing it.
- Behind Elasticsearch, Solr, OpenSearch, Lucene.

### Pieces
- **Analyzer** — tokenize + normalize text (lowercase, stemming, stop words).
- **Indexer** — builds the inverted index.
- **Query parser** — turns query into terms.
- **Ranker** — BM25, TF-IDF, learning-to-rank.

### Scaling search
- Shard by document or term.
- Replicas for read scaling.
- Refresh interval (Elasticsearch default 1 s).

### When
- Full-text search → Elasticsearch.
- Simple substring → LIKE (small data) or trigram index.
- Autocomplete → tries, FST (finite state transducer).

---

## 30. Streaming & Realtime

### Streaming platforms
- **Kafka** — durable, replayable log.
- **Pulsar** — geo-replicated, tiered storage.
- **Kinesis** — AWS managed.

### Stream processing
- **Kafka Streams** — Java library.
- **Flink** — distributed, exactly-once, event time.
- **Spark Streaming** — micro-batch.
- **Materialize / RisingWave** — streaming SQL.

### Realtime push to clients
- **WebSockets** — bidirectional.
- **SSE (Server-Sent Events)** — one-way server → client.
- **Long polling** — fallback.

### Use cases
- Live dashboards.
- Alerts (anomaly detection on stream).
- Triggers (a fraud event → SMS).

---

## 31. Design Cheatsheet — Picking the Right Tool

### Quick rules of thumb

| Need | Pick |
|------|------|
| Transactions, joins | Postgres, MySQL |
| Massive writes, simple reads | Cassandra, DynamoDB |
| Sub-ms cache | Redis |
| Full-text search | Elasticsearch |
| Time-series | InfluxDB, Timescale, Cassandra |
| Graph traversal | Neo4j |
| Object storage (files) | S3 + CloudFront |
| Async work | Kafka, SQS, RabbitMQ |
| Coordination | etcd, Zookeeper |
| Realtime push | WebSocket, SSE |
| Global low latency | CDN + Anycast |
| Idempotency | DB unique key + Idempotency-Key header |
| Rate limiting | Redis Lua + API Gateway |
| Search ranking | Elasticsearch + Learning-to-Rank |

### Interview rule of thumb
- "If the question says X billion and you can't avoid it, think about sharding, replication, queues, and caches."
- "If the question says milliseconds, think about Redis, CDN, and avoiding the database on the hot path."
- "If the question says availability, think multi-region, replication, and failure modes."
- "Always state your non-functional requirements (NFRs) before designing."

---

## The 30 Most-Asked Interview Concepts (Quick Recap)

1. Vertical vs horizontal scaling
2. Load balancing (L4 vs L7)
3. Caching strategies (cache-aside, write-through)
4. Cache stampede / hot keys / cold start
5. Replication (master-slave, multi-master)
6. Sharding (horizontal, vertical, directory)
7. Consistent hashing (virtual nodes)
8. CAP and PACELC
9. Consistency models (linearizable → eventual)
10. Quorum, Raft, Paxos
11. Distributed locks + fencing tokens
12. Idempotency keys
13. Message queues (Kafka vs RabbitMQ)
14. Pub/Sub and fan-out
15. CDN and edge compute
16. REST vs GraphQL vs gRPC
17. Webhooks
18. Microservices vs monolith
19. Bloom filter, HyperLogLog, Count-Min Sketch
20. Rate limiting (token bucket)
21. Pagination (offset vs cursor)
22. Geo-DNS, Anycast, multi-region
23. Storage engines (B-tree vs LSM)
24. Search (inverted index)
25. Security (OAuth, JWT, secrets)
26. Observability (SLI/SLO, OpenTelemetry)
27. Resilience (retries, circuit breaker, bulkhead)
28. Pagination, indexing, hot keys
29. Streaming and realtime
30. Picking the right tool — and explaining the trade-off

Master these and you will be able to walk into any system design round with confidence.
