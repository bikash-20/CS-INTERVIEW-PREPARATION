# System Design — Classic Problems Index

A catalog of the most frequently asked system design interview problems with detailed solutions.

## Index of Problems

| # | Problem | File | Category |
|---|---------|------|----------|
| 1 | URL Shortener (TinyURL / bit.ly) | [01-url-shortener.md](01-url-shortener.md) | Read-heavy, Cache |
| 2 | Twitter / Social Feed | [02-twitter.md](02-twitter.md) | Fan-out, Messaging |
| 3 | YouTube / Netflix | [03-youtube.md](03-youtube.md) | Media, CDN |
| 4 | Rate Limiter | [04-rate-limiter.md](04-rate-limiter.md) | Infra, Caching |
| 5 | Parking Lot (LLD) | [05-parking-lot.md](05-parking-lot.md) | LLD, OOP |
| 6 | Uber / Lyft | [06-uber.md](06-uber.md) | Geo, Real-time |
| 7 | WhatsApp / Messenger | [07-whatsapp.md](07-whatsapp.md) | Real-time, Chat |
| 8 | Dropbox / Google Drive | [08-dropbox.md](08-dropbox.md) | Sync, Storage |
| 9 | Key-Value Store (DynamoDB) | [09-key-value-store.md](09-key-value-store.md) | Distributed Systems |
| 10 | Notification System | [10-notification-system.md](10-notification-system.md) | Multi-channel, Queue |

## More Classic Problems (Self-Study)

| Problem | Key Concepts |
|---------|--------------|
| Instagram | Feed, image storage, CDN, search |
| Web Crawler | BFS, politeness, dedup, frontier |
| Search Autocomplete (Typeahead) | Trie, prefix lookup, caching |
| Hotel Booking (Airbnb) | Inventory, search, payment |
| Ticket Booking (BookMyShow) | Seat locking, concurrency, payment |
| Payment System (Stripe) | Idempotency, transactions, ledger |
| Distributed Message Queue (Kafka) | Partitions, ordering, replication |
| Distributed Cache | Consistent hashing, replication |
| Library Management System | LLD, classes |
| Chess Game | LLD, state machine |

---

## How To Approach A System Design Interview

### 1. Clarify Requirements (3-5 mins)
- Functional requirements — what must the system do?
- Non-functional — scale, latency, availability.
- Out of scope? (e.g., "we don't need analytics")

### 2. Capacity Estimation (3-5 mins)
- DAU, QPS (read/write), storage.
- Quick math on napkin.

### 3. High-Level Design (10 mins)
- Draw boxes & arrows: client → gateway → service → DB.
- Identify key components.

### 4. Database Design (5 mins)
- Schema for main entities.
- SQL vs NoSQL choice — justify.

### 5. Deep Dive (15 mins)
- Pick 2-3 hot paths.
- Discuss bottlenecks, scaling, caching, partitioning.

### 6. Wrap Up (5 mins)
- Monitoring, alerting.
- Future improvements.
- Failure modes & recovery.

---

## Common Building Blocks

You can mix and match these:
- **Load balancer** (L4 / L7)
- **CDN**
- **Caching layer** (Redis, Memcached)
- **Message queue** (Kafka, RabbitMQ)
- **Database** (SQL + NoSQL)
- **Search engine** (Elasticsearch)
- **Object storage** (S3)
- **Workflow** (workflow service)

---

## Pro Tips

 Start with the **simplest solution**, then iterate.
 Make the **right trade-offs** explicit (consistency vs availability, etc.).
 **Quantify** everything — QPS, storage, latency targets.
 Discuss **failure modes** — what if this service crashes?
 Use **back-of-envelope math** to back decisions.
 Don't jump into code or schema before high-level.
 Don't over-engineer — match requirements.
