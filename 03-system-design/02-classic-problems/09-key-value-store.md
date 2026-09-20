# Design Key-Value Store (Redis / DynamoDB / Cassandra Style)

The classic "design a distributed KV store" question. Tests your understanding of consensus, consistency, partitioning, and failure detection.

---

## 1. Requirements

### Functional
- Put(key, value), Get(key), Delete(key).
- Optional: range scans, secondary indexes, TTL, transactions.

### Non-Functional
- High availability — every request gets a response (AP).
- Configurable consistency (R + W > N).
- Horizontally scalable (petabytes).
- Partition-tolerant.

### Out of scope
- SQL features (joins, complex queries).

---

## 2. Capacity Estimation

| Metric | Value |
|---|---|
| Total data | 100 TB → 1 PB |
| Item size | < 10 KB typical |
| QPS | 100K reads + 10K writes |
| Per-shard size | ~100 GB (lots of shards) |
| Latency target | single-digit ms p99 |

---

## 3. Key Design Decisions

| Decision | Choices |
|---|---|
| Storage engine | LSM (Cassandra, RocksDB) vs B-Tree (MySQL, Mongo) |
| Consistency | Strong (Paxos / Raft) vs eventual (Dynamo-style) |
| Partitioning | Consistent hashing with virtual nodes |
| Replication | Multi-master (Dynamo) or single-leader (Cassandra) |
| Conflict resolution | Last-write-wins / vector clocks / CRDTs |

### Why Dynamo-style design (the classical answer)
- High write availability.
- Optimistic replication.
- Sloppy quorums + hinted handoff.
- Anti-entropy (Merkle trees) for background repair.

---

## 4. Partitioning — Consistent Hashing

- Hash keys + nodes onto a ring.
- Each node owns N/K keys (`K` = total keys, `N` = nodes).
- Use **virtual nodes** (e.g., 150 vnodes per real node) for balanced load.
- Add/remove a node → only a fraction of keys reshuffle.

---

## 5. Replication

Each key stored on **N** (e.g., 3) consecutive nodes on the ring.
- Coordinator node: the node that owns the key on the ring.
- For a write, coordinator sends to N replicas in parallel.
- Quorum: write succeeds when W replicas ack.
- Read returns from R replicas; coordinator picks latest (vector clock or timestamp).

`W + R > N` → strong read-your-writes (within a single client).

Typical: N=3, W=2, R=2 → can survive 1 failed replica, strong reads.

---

## 6. Writes — LSM Tree

```
 Memtable (in-memory) ── flush ──► SSTable on disk
 │
 ▼ (WAL — write-ahead log)
```

Process:
1. Append write to **WAL** (durability).
2. Insert to memtable (in-memory tree).
3. When memtable full → flush sorted run to new SSTable on disk.
4. Background **compaction** merges SSTables, drops tombstones.

Why LSM?
- Sequential writes are fast (HDDs and SSDs both).
- Reads check memtable + recent SSTables first; bloom filters skip absent keys.

---

## 7. Reads

1. Check memtable.
2. Check bloom filter per SSTable (skip if "definitely no").
3. Read index block → binary search to find offset.
4. Fetch data block.
5. Merge results across SSTables.
6. If multiple versions, pick latest (using timestamp or vector clock).

To bound read latency: maintain a **block cache** of hot blocks in memory.

---

## 8. Conflict Resolution

Multiple writes for the same key can arrive out of order.

### Vector clocks (Dynamo origin)
- Each version carries a set of (node, counter) pairs.
- Client resolves "no causal link" → multiple versions returned → user picks.

### Timestamp (most modern systems)
- Each write has a timestamp from the coordinator clock.
- "Last-write-wins" by timestamp.
- Risk: clock skew; mitigated with **hybrid logical clocks (HLC)**.

### CRDTs
- Data structures that merge automatically (e.g., counters, sets).
- Used when conflicts are common (counters, shopping carts).

---

## 9. Failure Detection (Gossip)

- Nodes periodically exchange membership info.
- Each node keeps a "phi" suspicion level for peers.
- Beyond a threshold → mark as failed; replicate to new nodes.

```
 gossip every 1 s
 node A → node B: "I'm alive, my peers are [...], I think node C is alive"
```

---

## 10. Anti-Entropy & Repair (Merkle Trees)

- Per-node Merkle tree over key ranges.
- Compare trees between replicas to find which key ranges differ.
- Sync only differing ranges.

Without this, dropped writes would stay divergent forever.

---

## 11. Hinted Handoff

If a replica is down at write time:
- Coordinator **stores the write** locally with a hint.
- When the dead replica comes back, the original (or any of its predecessors on the ring) replays the hint.

This keeps writes available during short outages.

---

## 12. Sloppy Quorums

Even with quorum writes, if W of N can't be reached, accept W writes to the next nodes on the ring (with hints to "real" owner later). This keeps writes available across AZ failures.

Cost: stronger consistency temporarily lost.

---

## 13. Data Model

```
key: 64-bit hash ring position, or composite
value: opaque bytes
metadata: timestamp, ttl, vector clock, version
```

Secondary indexes (optional): store reverse map `attr → list of keys`.

---

## 14. API Design

```
PUT /kv/{key}
 Body: { value, ttl? }
 Headers: If-Match (version) for conditional writes
 → 200 { version: 42 }

GET /kv/{key}?consistency=quorum|one
 → 200 { value, version }

DELETE /kv/{key}
 → 204

SCAN /kv?prefix=user:&limit=100 (optional, range scan)
 → 200 { items: [...], next_cursor: ... }
```

---

## 15. Optimizations

- **Bloom filters** per SSTable.
- **Block cache** for hot reads.
- **Compression** (LZ4, Snappy) per SSTable.
- **Tiered storage** — old SSTables to object storage.
- **Read repair** — when a read hits a stale replica, repair it inline.
- **Bulk load** for big inputs (skip WAL).

---

## 16. Consistency Levels

| Level | W or R | Trade-off |
|---|---|---|
| ONE | 1 | Fastest; can miss recent writes |
| QUORUM | ceil(N/2) | Strong if R+W > N |
| ALL | N | Strongest; fails if any replica down |
| LOCAL_QUORUM | majority in local DC | Lower latency |

---

## 17. Failure Modes

| Failure | Mitigation |
|---|---|
| Replica down | Sloppy quorum + hinted handoff. |
| Disk failure | Re-replicate from peers. |
| Network partition | AP behavior; clients reconcile later. |
| Long outage of a node | Merkle tree resync after recovery. |
| Corruption | Checksum per block. |

---

## 18. Follow-up Questions

**Q: How to support range scans?**
Hash the keys onto the ring, but for range scans, partition by **ordered keys** (Cassandra-style) and store each range in order-preserving storage. Bloom filters and SSTable ordering enable range scans.

**Q: How to add secondary indexes?**
Maintain a separate index per attribute. Either co-located or async via streaming.

**Q: How to expire (TTL) keys?**
Lazy expiration on read + periodic compaction drops expired entries.

**Q: How to back up?**
Snapshot SSTables to S3. Restore by streaming them back to peers.

**Q: Why LSM over B-Tree for KV?**
LSM writes are sequential → faster on HDD/SSD. Reads slower but optimized with bloom filters + cache.

**Q: How to handle hot keys?**
- Replicate the key across many nodes.
- Use a small in-process cache in front for the hottest keys.

**Q: How to migrate data between clusters?**
- Dual writes for new keys; background copy of old; verify; cut over; stop old.

**Q: How to encrypt at rest?**
Per-disk encryption with KMS-managed keys; per-key encryption for sensitive data.

---

## 19. End-to-End Diagram

```
 Client ─► Coordinator (ring position for key)
 │
 ├─ WAL append ─► Memtable ─► SSTable on disk
 │
 ├─ replicate to N successors (write quorum W)
 │
 └─ read from R replicas → reconcile
 ▲
 │ gossip + Merkle-tree anti-entropy between replicas
 │
 peers
```
