# System Design Fundamentals

A comprehensive collection of core concepts tested in system design interviews.

---

## 1. Scalability

### Vertical Scaling (Scale Up)
- Add more power to a single machine (CPU, RAM, SSD).
- **Pros:** Simple; no code change.
- **Cons:** Hardware limits; single point of failure; expensive.

### Horizontal Scaling (Scale Out)
- Add more machines.
- **Pros:** Theoretically unlimited; fault tolerant.
- **Cons:** More complex (load balancing, data consistency).

### When to Use
- Vertical: databases, simple apps, when N is small.
- Horizontal: web servers, big data, microservices.

---

## 2. Load Balancing

Distributes requests across multiple servers.

### Types
- **L4 (Transport):** Routes based on IP/port. Faster.
- **L7 (Application):** Routes based on URL/headers/cookies. Smarter.

### Algorithms
- Round Robin
- Least Connections
- Weighted Round Robin
- IP Hash (sticky)
- Consistent Hashing (for caching)

### Tools
- NGINX, HAProxy, Envoy (software)
- AWS ALB/NLB, GCP LB (cloud)

---

## 3. Caching

### Where to Cache
- **Client-side:** Browser cache, HTTP cache headers
- **CDN:** Static assets at edge
- **Application:** In-memory (e.g., Caffeine, Guava)
- **Distributed cache:** Redis, Memcached
- **Database:** Query cache, materialized views

### Strategies
- **Cache-aside (lazy):** App reads cache; on miss, reads DB and populates.
- **Write-through:** Write to cache & DB synchronously.
- **Write-behind:** Write to cache; async flush to DB.
- **Read-through:** Cache itself fetches from DB.
- **Cache stampede:** Many concurrent misses → thundering herd. Mitigation: locking, probabilistic early refresh.

### Eviction Policies
- LRU, LFU, FIFO, TTL-based, ARC.

---

## 4. Database Scaling

### Replication
- **Master-Slave:** Writes → master; reads → slaves. Read scaling.
- **Master-Master:** Multi-master writes; conflict resolution tricky.
- **Synchronous** vs **Asynchronous.**

### Sharding (Partitioning)
- **Horizontal:** Split rows by hash of key (`user_id % N`).
- **Vertical:** Split tables/columns across DBs.
- **Directory-based:** Lookup table.

### Challenges
- Cross-shard queries/joins.
- Rebalancing when adding shards.
- Hot shards (uneven distribution).

---

## 5. CAP Theorem

In a distributed data store, you can have **at most 2 of 3**:
- **C**onsistency — every read sees latest write.
- **A**vailability — every request gets a response.
- **P**artition tolerance — works despite network partitions.

Since P is required, you choose:
- **CP:** MongoDB, HBase, Redis (configurable).
- **AP:** Cassandra, DynamoDB, CouchDB.

---

## 6. Consistent Hashing

Used in distributed caches and databases. Maps keys to nodes in a way that:
- Adding/removing a node only affects `K/N` keys (vs all keys in simple modulo).
- Uses a hash ring.

```
 node A
 / \
 key1 node B
 \ /
 node C
 key2, key3
```

---

## 7. Message Queues

Async communication between services. Decouples producers and consumers.

### Concepts
- **Producer** → publishes message
- **Queue/Topic** → holds messages
- **Consumer** → subscribes & processes

### Patterns
- **Pub/Sub** (one-to-many)
- **Work queue** (one-to-one, load-balanced)

### Examples
- Kafka, RabbitMQ, AWS SQS, Google Pub/Sub.

### Use Cases
- Async tasks (email, video processing)
- Decoupling services
- Buffering bursts
- Event-driven architecture

---

## 8. CDN (Content Delivery Network)

- Caches static assets at edge locations worldwide.
- Reduces latency, offloads origin.
- Examples: Cloudflare, Akamai, CloudFront, Fastly.

---

## 9. Database Choices

### SQL (RDBMS)
PostgreSQL, MySQL, Oracle, SQL Server.
- ACID, structured, joins, mature tooling.

### NoSQL
- **Key-Value:** Redis, DynamoDB, Memcached
- **Document:** MongoDB, CouchDB
- **Column-Family:** Cassandra, HBase
- **Graph:** Neo4j, JanusGraph

### When to Pick What
- Need transactions, complex queries → SQL.
- Massive scale, flexible schema, simple access patterns → NoSQL.
- Real-time, low-latency reads → in-memory (Redis).
- Search, log analysis → Elasticsearch.

---

## 10. API Design

### REST
- Resource-based URLs
- HTTP methods (GET, POST, PUT, DELETE)
- Stateless, cacheable

### GraphQL
- Client specifies shape of response
- Single endpoint
- Reduces over-fetching/under-fetching

### gRPC
- Protocol buffers, HTTP/2
- Strongly typed contracts
- Best for internal microservice communication

---

## 11. Microservices vs Monolith

| Aspect | Monolith | Microservices |
|--------|----------|---------------|
| Deployment | Single unit | Independent services |
| Scaling | Whole app | Per service |
| Tech | One stack | Polyglot |
| Communication | Function calls | HTTP/gRPC/messaging |
| Complexity | Lower initially | Higher (network, ops) |
| Failure | All-or-nothing | Isolated |

---

## 12. Bloom Filter

Probabilistic data structure for set membership:
- **False positives possible** (says "yes" wrongly)
- **False negatives impossible** (if says "no", truly not in set)
- **Space-efficient**

Used for: avoiding cache misses, malicious URL detection, DB lookup optimization.

---

## 13. Quorum

For replicated stores: `R + W > N`
- N = total replicas
- R = read quorum
- W = write quorum

This guarantees reading sees at least one updated copy.

---

## 14. Rate Limiting

### Algorithms
- **Token bucket:** Tokens refill at rate; consume per request.
- **Leaky bucket:** Process at fixed rate.
- **Fixed window counter:** Count per time window; simple, can spike at boundaries.
- **Sliding window:** More accurate.

### Implementation
- Redis counters, in-memory LRU.
- Use cases: API throttling, abuse prevention, fair usage.

---

## 15. Top Interview Concepts to Master

1. Scalability (vertical vs horizontal)
2. Load balancing
3. Caching strategies
4. Database sharding & replication
5. CAP theorem
6. Consistent hashing
7. Message queues
8. CDN
9. API design
10. Microservices
11. Rate limiting
12. Bloom filters
13. Quorum
14. Storage types (block, file, object)
15. Proxies (forward, reverse)
