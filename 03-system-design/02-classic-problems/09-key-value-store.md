# Design Key-Value Store (Redis / DynamoDB)

## Requirements

### Functional
- `put(key, value)`, `get(key) → value`.
- `delete(key)`.
- Eventually consistent or strongly consistent (configurable).

### Non-Functional
- Highly available.
- Low latency (sub-millisecond reads).
- Durable (no data loss).
- Horizontally scalable.

---

## High-Level Design

```
Client
 │
 ▼
Coordinator / Router
 │ (consistent hashing)
 ├──► Node 1 (hash range 0-33%)
 ├──► Node 2 (hash range 33-66%)
 └──► Node 3 (hash range 66-100%)
 │ │
 │ └── WAL + SSTable (LSM tree)
 │
 └── Replicas (synchronous or async)
```

---

## Single-Node Design

### Data Structure: **Hash Table** (in-memory)
- O(1) operations.
- Persist to disk periodically (snapshot) or per-write (AOF).

### Issues
- Limited by RAM.
- Slow cold start.
- Vertical scaling only.

---

## Distributed Design

### 1. Consistent Hashing
- Map keys and nodes onto a ring (0 to 2^32).
- Each key belongs to the next node clockwise.
- Adding/removing a node affects only its immediate neighbors.

```
 N1
 / \
 keys N2
 ↓ ↓
 N3
```

- **Virtual nodes:** Each physical node gets multiple positions on the ring for better load balance.

### 2. Replication
- Each key stored on N replicas (e.g., 3).
- Use preference list (next N nodes clockwise).

### 3. Failure Handling
- **Hinted handoff:** If primary fails, another node temporarily stores the write.
- **Anti-entropy:** Periodic Merkle tree sync to detect & fix inconsistencies.
- **Sloppy quorum:** Accept writes on first W nodes (might not be primary replicas).

### 4. Conflict Resolution
- **Last-write-wins (LWW):** simple, can lose data on clock skew.
- **Vector clocks:** detect conflicts; resolve at read.
- **CRDTs:** for special data types.

---

## Storage Engines

### 1. LSM Tree (Log-Structured Merge Tree) — Used by Cassandra, RocksDB, LevelDB
```
Writes → WAL → MemTable (in-memory sorted map)
 ↓ (when full)
 SSTable on disk
 ↓ (background compaction merges SSTables)
```

**Pros:** Fast writes; high throughput.
**Cons:** Reads may need to check multiple SSTables → mitigated by Bloom filters.

### 2. B-Tree — Used by InnoDB, PostgreSQL
- Pages of fixed size; each node has many keys.
- **Pros:** Good read performance; range queries.
- **Cons:** Random writes; page splits.

---

## Caching & Performance

1. **In-memory cache** for hot keys.
2. **Bloom filters** to avoid disk lookups for non-existent keys.
3. **Compression** of values.
4. **Connection pooling** at client.
5. **Pipelining** multiple commands in one TCP round-trip.

---

## Schema (Conceptual)

```
Key: string (or binary)
Value: bytes (opaque)
TTL: optional
Flags: e.g., type marker
```

---

## Operations

### Get
1. Hash key → find nodes in preference list.
2. Query first reachable node.
3. Check cache → hit: return; miss: read storage.
4. If not found, check next node (eventual consistency).

### Put
1. Hash key → preference list.
2. Send to first W nodes (write quorum).
3. Each writes to WAL → MemTable → ack.
4. Coordinator acks after W acks.

### Quorum: `R + W > N`
- Strong consistency if R + W > N (where N = replicas).
- Examples: N=3, W=2, R=2.

---

## Capacity Estimation

- 100 TB data
- 100K QPS reads, 10K QPS writes
- Latency target: <10ms p99

---

## Follow-up Questions

**Q: How to handle node failures?**
Hinted handoff + replication + auto-failover.

**Q: How to scale writes?**
More nodes (consistent hashing); but write contention on hot keys remains.

**Q: How to support transactions?**
Multi-key transactions with 2PC (Paxos / Raft for consensus).

**Q: How to migrate data without downtime?**
Dual-write + backfill + cutover.

**Q: When to use KV store vs SQL?**
- KV: simple access patterns, scale-out, low latency.
- SQL: complex queries, transactions, joins.
