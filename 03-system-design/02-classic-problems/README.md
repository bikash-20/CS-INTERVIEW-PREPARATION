# System Design — Classic Problems Index

A catalog of the most frequently asked system design interview problems with **detailed, interview-ready solutions**. Each file covers requirements, capacity math, high-level design, components, schema, scaling, failure modes, and 8-15 follow-up Q&As.

---

## How To Approach A System Design Interview

### 1. Clarify Requirements (3-5 mins)
- Functional requirements — what must the system do?
- Non-functional — scale, latency, availability.
- Out of scope? (e.g., "we don't need analytics")

### 2. Capacity Estimation (3-5 mins)
- DAU, QPS (read/write), storage.
- Quick back-of-envelope math.
- Use power-of-two numbers and the latency cheat sheet from `01-fundamentals/`.

### 3. High-Level Design (10 mins)
- Draw boxes & arrows: client → gateway → service → DB.
- Identify the read path and write path separately.
- Justify each choice briefly.

### 4. Database Design (5 mins)
- Schema for main entities.
- SQL vs NoSQL — say why.
- Indexing strategy.

### 5. Deep Dive (15 mins)
- Pick 2-3 hot paths or the bottleneck the interviewer calls out.
- Discuss caching, partitioning, queues, replication, ranking, fan-out.
- Talk about consistency / availability trade-offs explicitly.

### 6. Wrap Up (5 mins)
- Monitoring, alerting.
- Failure modes & recovery.
- Future improvements.

### Pro Tips
- Start simple, iterate.
- Quantify everything.
- Make trade-offs explicit ("we choose AP for availability").
- Discuss failure modes ("if Redis is down, fall back to DB").
- Don't jump into code or schema before the high-level.

---

## Common Building Blocks

You can mix and match these across problems:
- **Load balancer** (L4 / L7)
- **CDN** + edge compute
- **Cache** (in-process, Redis, Memcached)
- **Message queue** (Kafka, RabbitMQ, SQS)
- **Database** (SQL + NoSQL)
- **Search engine** (Elasticsearch)
- **Object storage** (S3)
- **Geospatial index** (H3, QuadTree, PostGIS)
- **Stream processor** (Flink, Kafka Streams)
- **Workflow / scheduler**

---

## Solved Classic Problems

| # | Problem | File | Category |
|---|---------|------|----------|
| 1 | URL Shortener (TinyURL / bit.ly) | [01-url-shortener.md](01-url-shortener.md) | Read-heavy, Cache |
| 2 | Twitter / Social Feed | [02-twitter.md](02-twitter.md) | Fan-out, Messaging |
| 3 | YouTube / Netflix | [03-youtube.md](03-youtube.md) | Media, CDN, Adaptive streaming |
| 4 | Rate Limiter | [04-rate-limiter.md](04-rate-limiter.md) | Infra, Caching, Lua |
| 5 | Parking Lot (LLD) | [05-parking-lot.md](05-parking-lot.md) | LLD, OOP, Concurrency |
| 6 | Uber / Lyft | [06-uber.md](06-uber.md) | Geo, Real-time dispatch |
| 7 | WhatsApp / Messenger | [07-whatsapp.md](07-whatsapp.md) | Real-time, Chat, E2EE |
| 8 | Dropbox / Google Drive | [08-dropbox.md](08-dropbox.md) | Sync, Chunking, Dedup |
| 9 | Key-Value Store (DynamoDB style) | [09-key-value-store.md](09-key-value-store.md) | Distributed Systems, LSM |
| 10 | Notification System | [10-notification-system.md](10-notification-system.md) | Multi-channel, Queues |
| 11 | Pastebin | [11-pastebin.md](11-pastebin.md) | Cache, Expiration |
| 12 | Web Crawler | [12-web-crawler.md](12-web-crawler.md) | BFS, Politeness, Dedup |
| 13 | Proximity Service (Yelp / Places) | [13-proximity-service.md](13-proximity-service.md) | Geo, Search, Ranking |
| 14 | Instagram | [14-instagram.md](14-instagram.md) | Feed, Media, Stories |
| 15 | Distributed Task Scheduler | [15-task-scheduler.md](15-task-scheduler.md) | DAGs, Retries, Coordination |

---

## More Classic Problems to Self-Study

| Problem | Key Concepts |
|---------|--------------|
| Search Autocomplete (Typeahead) | Trie, prefix lookup, caching, top-K |
| Hotel Booking (Airbnb) | Inventory, search, geo, payment |
| Ticket Booking (BookMyShow) | Seat locking, concurrency, payment |
| Payment System (Stripe) | Idempotency, ledger, two-phase |
| Distributed Message Queue (Kafka) | Partitions, ordering, replication |
| Distributed Cache | Consistent hashing, replication, gossip |
| Library Management System | LLD, classes |
| Chess Game | LLD, state machine |
| Snake & Ladder | LLD, dice, board |
| Stack Overflow | Q&A ranking, tags, full-text search |

---

## Coverage Summary

- **15 detailed problem solutions**, each with:
  - Functional + non-functional requirements
  - Capacity estimation
  - High-level design diagram
  - Database schema
  - Key algorithms (with code snippets where relevant)
  - Scaling and optimization strategy
  - Failure modes
  - 8-15 follow-up interview Q&A entries
- **Total: 5 LLD-friendly (Parking Lot + 4 in self-study) + 10 HLD with deep dives**.

Read the **fundamentals** first, then go through each problem file top-to-bottom. After solving them once, you should be able to confidently walk into any system design round.
