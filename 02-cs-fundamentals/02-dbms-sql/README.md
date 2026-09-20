# DBMS & SQL — Comprehensive Interview Guide

> **Databases are at the heart of every backend system.** From a one-table SQLite app to a thousand-shard distributed system, the same fundamentals — ACID, normalization, indexing, transactions — show up again and again. This file gives you the **theory, comparisons, SQL you'll write in interviews, and 90+ Q&A entries** to handle any DBMS round.

---

## Table of Contents

1. [Database Fundamentals](#1-database-fundamentals)
2. [ACID & Transaction Properties](#2-acid--transaction-properties)
3. [Relational Model & Keys](#3-relational-model--keys)
4. [Normalization (1NF → 5NF)](#4-normalization-1nf--5nf)
5. [SQL Essentials — DDL, DML, DQL, DCL, TCL](#5-sql-essentials--ddl-dml-dql-dcl-tcl)
6. [Joins (Inner, Outer, Self, Cross)](#6-joins-inner-outer-self-cross)
7. [Subqueries, CTEs & Recursive Queries](#7-subqueries-ctes--recursive-queries)
8. [Window Functions & Analytics](#8-window-functions--analytics)
9. [Indexes — B-Tree, Hash, Bitmap, Clustered](#9-indexes--b-tree-hash-bitmap-clustered)
10. [Transactions, Isolation Levels & Locking](#10-transactions-isolation-levels--locking)
11. [MVCC, Deadlocks & Concurrency](#11-mvcc-deadlocks--concurrency)
12. [SQL vs NoSQL — When to Use What](#12-sql-vs-nosql--when-to-use-what)
13. [Distributed Databases — CAP, PACELC, Replication, Sharding](#13-distributed-databases--cap-pacelc-replication-sharding)
14. [Query Optimization & EXPLAIN Plans](#14-query-optimization--explain-plans)
15. [Data Modeling Patterns & Anti-Patterns](#15-data-modeling-patterns--anti-patterns)
16. [Storage Engines, WAL, LSM vs B-Tree](#16-storage-engines-wal-lsm-vs-b-tree)
17. [Stored Procedures, Triggers, Views & Materialized Views](#17-stored-procedures-triggers-views--materialized-views)
18. [Quick Reference Card](#18-quick-reference-card)
19. [Top Interview Q&A (40 entries)](#19-top-interview-qa-40-entries)

---

## 1. Database Fundamentals

A **Database Management System (DBMS)** is software that lets you **store, retrieve, modify, and manage** data efficiently while ensuring **safety, concurrency, and integrity**.

### Types of DBMS

| Type | Example | Notes |
|------|---------|-------|
| **Relational (RDBMS)** | PostgreSQL, MySQL, Oracle, SQL Server | Tables, SQL, ACID |
| **Document** | MongoDB, CouchDB | JSON-like docs, schema-flexible |
| **Key-Value** | Redis, DynamoDB, Memcached | O(1) get/put, no queries |
| **Column-Family** | Cassandra, HBase, Bigtable | Billions of rows, wide columns |
| **Graph** | Neo4j, Amazon Neptune | Nodes + edges, relationship queries |
| **Search** | Elasticsearch, OpenSearch | Full-text, inverted index |
| **Time-Series** | InfluxDB, TimescaleDB | Optimized for append-only |
| **Vector** | Pinecone, Milvus | Embeddings / similarity search |

### Schema vs Schema-less

- **Schema-on-write (SQL):** schema is enforced when data is written. Strong typing, predictable.
- **Schema-on-read (NoSQL):** schema is interpreted when data is read. Flexible, easier to evolve, but bugs surface late.

### OLTP vs OLAP

| Aspect | OLTP | OLAP |
|--------|------|------|
| Use case | Day-to-day transactions | Reporting, analytics |
| Workload | Many small reads/writes | Few heavy reads over huge data |
| Data | Current, normalized | Historical, often denormalized |
| Storage | Row-oriented | Column-oriented |
| Example | Banking, e-commerce order placement | Sales dashboards, BI |
| DB | PostgreSQL, MySQL | Snowflake, BigQuery, Redshift, ClickHouse |

---

## 2. ACID & Transaction Properties

A **transaction** is a logical unit of work — either all of it succeeds or none does.

### The ACID Guarantees

| Property | Meaning | Example |
|----------|---------|---------|
| **Atomicity** | All-or-nothing — partial state never visible | Transfer ₹100: both debit and credit succeed, or neither does |
| **Consistency** | DB moves from one valid state to another | Foreign keys, constraints, triggers remain satisfied |
| **Isolation** | Concurrent transactions don't see each other's intermediate state | Two simultaneous bookings don't double-book the same seat |
| **Durability** | Committed data survives crashes and power loss | WAL (write-ahead log) flushed to disk before `COMMIT` returns |

### ACID vs BASE

| ACID (SQL) | BASE (NoSQL) |
|------------|-------------|
| **Strong** consistency | **Basically** available |
| Atomic, isolated | Soft state (changes over time) |
| Strict schema | **Eventually** consistent |
| Examples: PostgreSQL | Examples: Cassandra, DynamoDB |

> **Interview tip:** Don't say "NoSQL = no consistency." Most NoSQL databases **can** be configured for strong consistency — they just default to eventual consistency for performance.

---

## 3. Relational Model & Keys

### Primary Key (PK)

- Uniquely identifies a row. **NOT NULL**, **unique**.
- Surrogate key (auto-increment / `UUID`) vs natural key (e.g., SSN).
- **Best practice:** use `BIGINT` or `UUID v7`, never expose to clients directly.

### Foreign Key (FK)

- A column (or set of columns) that references the PK of another (or same) table.
- Enforces **referential integrity**: you cannot insert a value that doesn't exist in the parent table.
- Cascading actions: `ON DELETE CASCADE`, `ON DELETE SET NULL`, `ON DELETE RESTRICT`.

### Other Key Types

- **Candidate Key:** any column (or set) that could be a PK.
- **Composite Key:** PK made of multiple columns (e.g., `OrderID + ProductID`).
- **Unique Key:** like PK but allows one NULL in most DBs.
- **Super Key:** any superset of a candidate key.

### Relationships

| Type | Example | Implementation |
|------|---------|----------------|
| 1-to-1 | User ↔ Profile | FK + UNIQUE |
| 1-to-Many | Customer → Orders | FK on the "many" side |
| Many-to-Many | Students ↔ Courses | Junction/bridge table |

```sql
-- Many-to-many: students_courses is the junction table
CREATE TABLE students_courses (
  student_id BIGINT REFERENCES students(id),
  course_id  BIGINT REFERENCES courses(id),
  PRIMARY KEY (student_id, course_id)
);
```

---

## 4. Normalization (1NF → 5NF)

Normalization = **removing redundancy and anomalies** by decomposing tables. Each normal form adds a stronger rule.

### 1NF (First Normal Form)

- **Atomic values** — no multi-valued cells (no comma-separated lists in a column).
- Each row **uniquely identified** (has a primary key).
- **No repeating groups** of columns (no `phone1, phone2, phone3`).

```sql
-- Bad: phones = "9876543210,9876543211"
-- Good:
CREATE TABLE phones (user_id BIGINT, phone VARCHAR(20), PRIMARY KEY(user_id, phone));
```

### 2NF

- 1NF + no **partial dependency** on a composite PK.
- Every non-key column depends on **the whole** primary key.

```sql
-- Bad: (student_id, course_id) PK, but course_name depends only on course_id
-- Fix: split into Enrollments(student_id, course_id) and Courses(course_id, course_name)
```

### 3NF

- 2NF + no **transitive dependency** (non-key column depends on another non-key column).

```sql
-- Bad: Orders(order_id, customer_id, customer_name, customer_city)
-- Fix: keep only customer_id here; move customer_name/city to a Customers table
```

### BCNF (Boyce-Codd)

- 3NF + **every determinant is a candidate key.**
- For every non-trivial functional dependency `X → Y`, `X` must be a superkey.

### 4NF

- BCNF + no **multi-valued dependencies.**
- E.g., a person can have multiple skills and multiple languages — these are independent and create extra rows; split into separate tables.

### 5NF / PJNF (Project-Join Normal Form)

- No **join dependency** that isn't implied by candidate keys. Rare in practice. Mostly theoretical.

### Quick Comparison

| Form | Rule Added | Practical Use |
|------|-----------|---------------|
| 1NF | Atomic values | Always |
| 2NF | No partial dep | Always |
| 3NF | No transitive dep | Always |
| BCNF | Every determinant is a key | Usually |
| 4NF | No multi-valued dep | Sometimes |
| 5NF | No non-trivial join dep | Rarely |

### When to **Denormalize**

- **Read-heavy** dashboards / OLAP.
- When joins are too expensive to compute at runtime.
- Caching layer / materialized view.

> **Interview rule of thumb:** normalize for OLTP, denormalize for OLAP.

---

## 5. SQL Essentials — DDL, DML, DQL, DCL, TCL

### DDL — Data Definition Language

```sql
CREATE TABLE users (
  id          BIGSERIAL PRIMARY KEY,
  email       VARCHAR(255) NOT NULL UNIQUE,
  name        VARCHAR(100) NOT NULL,
  created_at  TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  is_active   BOOLEAN DEFAULT TRUE
);

ALTER TABLE users ADD COLUMN age INT;
DROP TABLE users;
TRUNCATE users;  -- deletes all rows, resets identity
```

### DML — Data Manipulation Language

```sql
INSERT INTO users (email, name) VALUES ('a@b.com', 'Alice');
UPDATE users SET name = 'Bob' WHERE id = 1;
DELETE FROM users WHERE is_active = FALSE;
```

### DQL — Data Query Language

```sql
SELECT id, email FROM users WHERE created_at > '2024-01-01' ORDER BY id DESC LIMIT 10;
```

### DCL — Data Control Language

```sql
GRANT SELECT, INSERT ON users TO app_user;
REVOKE INSERT ON users FROM app_user;
```

### TCL — Transaction Control Language

```sql
BEGIN;
UPDATE accounts SET balance = balance - 100 WHERE id = 1;
UPDATE accounts SET balance = balance + 100 WHERE id = 2;
COMMIT;
-- or ROLLBACK;
```

---

## 6. Joins (Inner, Outer, Self, Cross)

### Visual Venn Diagram of Joins

```
       A            B
     +-----+      +-----+
     |  1  |      |  3  |
     |     +------+     |
     |  2  |      |  4  |
     |     |      |     |
     +-----+      +-----+
  INNER JOIN    returns 2 & 3 (only matches)
  LEFT  JOIN    returns 1, 2, 3
  RIGHT JOIN    returns 2, 3, 4
  FULL  JOIN    returns 1, 2, 3, 4
```

### Inner Join

```sql
SELECT u.name, o.amount
FROM users u
INNER JOIN orders o ON u.id = o.user_id;
```

### Left Outer Join

```sql
-- All users, even those with no orders (NULL on right)
SELECT u.name, o.amount
FROM users u
LEFT JOIN orders o ON u.id = o.user_id;
```

### Right Outer Join

```sql
-- All orders, even if user deleted
SELECT u.name, o.amount
FROM users u
RIGHT JOIN orders o ON u.id = o.user_id;
```

### Full Outer Join

```sql
-- All rows from both; NULLs where no match
SELECT u.name, o.amount
FROM users u
FULL OUTER JOIN orders o ON u.id = o.user_id;
```

### Self Join

```sql
-- Employees and their managers (both in same table)
SELECT e.name AS employee, m.name AS manager
FROM employees e
LEFT JOIN employees m ON e.manager_id = m.id;
```

### Cross Join

```sql
-- Cartesian product: every row of A paired with every row of B
SELECT * FROM colors CROSS JOIN sizes;
-- Generates "red,small", "red,medium", ..., "blue,medium"
```

### Join Performance Tips

- Index **both** sides of the ON clause.
- Replace correlated subqueries with joins when possible.
- Use `STRAIGHT_JOIN` (MySQL) only when optimizer makes a bad plan.
- Prefer `INNER JOIN` over `LEFT JOIN` when NULLs aren't needed (clearer, often faster).

---

## 7. Subqueries, CTEs & Recursive Queries

### Scalar Subquery (returns single value)

```sql
SELECT *
FROM products
WHERE price > (SELECT AVG(price) FROM products);
```

### Correlated Subquery (runs per outer row)

```sql
SELECT name
FROM employees e
WHERE salary > (
  SELECT AVG(salary)
  FROM employees
  WHERE department_id = e.department_id
);
```

### `IN`, `EXISTS`, `ANY`, `ALL`

```sql
SELECT * FROM users WHERE id IN (SELECT user_id FROM orders);
SELECT * FROM users u WHERE EXISTS (SELECT 1 FROM orders o WHERE o.user_id = u.id);
SELECT * FROM products WHERE price > ANY (SELECT price FROM discounted);
SELECT * FROM products WHERE price > ALL (SELECT price FROM cheap_items);
```

> **Tip:** `EXISTS` short-circuits on first match — usually faster than `IN` for large subqueries.

### CTE (Common Table Expression)

```sql
WITH high_value AS (
  SELECT user_id, SUM(amount) AS total
  FROM orders
  GROUP BY user_id
  HAVING SUM(amount) > 10000
)
SELECT u.name, h.total
FROM users u
JOIN high_value h ON u.id = h.user_id;
```

### Recursive CTE (trees, graphs, bills of materials)

```sql
WITH RECURSIVE org_tree AS (
  SELECT id, name, manager_id, 1 AS depth
  FROM employees WHERE manager_id IS NULL
  UNION ALL
  SELECT e.id, e.name, e.manager_id, t.depth + 1
  FROM employees e
  JOIN org_tree t ON e.manager_id = t.id
)
SELECT * FROM org_tree;
```

---

## 8. Window Functions & Analytics

Window functions compute values **across a set of rows related to the current row** — without collapsing them (unlike `GROUP BY`).

```sql
SELECT
  name,
  department,
  salary,
  -- Ranking
  ROW_NUMBER() OVER (PARTITION BY department ORDER BY salary DESC) AS rn,
  RANK()       OVER (PARTITION BY department ORDER BY salary DESC) AS rk,
  DENSE_RANK() OVER (PARTITION BY department ORDER BY salary DESC) AS drk,
  -- Lag/lead
  LAG(salary,  1, 0) OVER (ORDER BY salary) AS prev_salary,
  LEAD(salary, 1)     OVER (ORDER BY salary) AS next_salary,
  -- Aggregates
  SUM(salary) OVER (PARTITION BY department)                       AS dept_total,
  AVG(salary) OVER (PARTITION BY department)                       AS dept_avg,
  -- First / last
  FIRST_VALUE(name) OVER (PARTITION BY department ORDER BY salary DESC) AS top_earner,
  LAST_VALUE(name)  OVER (PARTITION BY department ORDER BY salary DESC
                          ROWS BETWEEN UNBOUNDED PRECEDING AND UNBOUNDED FOLLOWING) AS lowest
FROM employees;
```

### ROW_NUMBER vs RANK vs DENSE_RANK

For salaries `[100, 90, 90, 80]`:

| Function | Salaries | Notes |
|----------|----------|-------|
| `ROW_NUMBER()` | `1, 2, 3, 4` | Always unique, no gaps |
| `RANK()`       | `1, 2, 2, 4` | Ties share rank, gap after ties |
| `DENSE_RANK()` | `1, 2, 2, 3` | Ties share rank, no gap |

### Frames (Window Frame)

```sql
-- Running total across rows in current partition
SUM(amount) OVER (ORDER BY date ROWS BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW)
```

Common frames:

- `ROWS BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW` — running total.
- `ROWS BETWEEN 3 PRECEDING AND CURRENT ROW` — moving average over 4 rows.
- `ROWS BETWEEN CURRENT ROW AND UNBOUNDED FOLLOWING` — countdown.

---

## 9. Indexes — B-Tree, Hash, Bitmap, Clustered

### What an Index Does

A separate data structure that lets the DB find rows matching a predicate **without scanning every row** — the #1 way to speed up reads.

### Index Types

| Type | Best For | Notes |
|------|----------|-------|
| **B-Tree** (default in most RDBMS) | Range queries, equality, sorting, `LIKE 'abc%'` | Balanced tree; O(log n) lookup |
| **B+ Tree** | Same but data only in leaves | Better sequential scans / range reads |
| **Hash** | Equality only (`WHERE id = 5`) | O(1) lookup; no range |
| **Bitmap** | Low-cardinality columns (gender, status) | Common in data warehouses |
| **GIN / GiST** | Full-text search, JSONB containment, geometry | PostgreSQL-specific |
| **BRIN** | Very large naturally-ordered tables (logs) | Tiny, summarized range indexes |

### Clustered vs Non-Clustered

| Clustered | Non-Clustered |
|-----------|---------------|
| Determines **physical** order of rows | Separate structure with row pointers |
| **One per table** (usually the PK) | Many per table |
| Fast range reads (sequential I/O) | Extra lookup to fetch row |

### Composite Index — Leftmost Prefix Rule

```sql
CREATE INDEX idx_user_status_created ON users(status, created_at);
```

This index helps queries on:

- `(status)` ✅
- `(status, created_at)` ✅
- `(status, created_at, ...)` ✅
- `(created_at)` ❌ — `status` is the leftmost column

### When Indexes Hurt

- **Write-heavy** tables: every `INSERT/UPDATE/DELETE` updates indexes.
- **Low-cardinality** columns (boolean, gender): full scan often wins.
- **Small tables**: optimizer may ignore the index.

### Index-Only Scan

If all needed columns are in the index, the DB doesn't touch the table. Create **covering indexes**:

```sql
-- Query: SELECT email FROM users WHERE status = 'active';
CREATE INDEX idx_active_email ON users(status) INCLUDE (email);  -- PostgreSQL
ALTER TABLE users ADD INDEX idx_active_email (status, email);    -- MySQL (leftmost prefix)
```

---

## 10. Transactions, Isolation Levels & Locking

### Isolation Levels (SQL Standard)

| Level | Dirty Read | Non-Repeatable Read | Phantom Read | Throughput |
|-------|------------|---------------------|--------------|------------|
| **Read Uncommitted** | Possible | Possible | Possible | Highest |
| **Read Committed** | Prevented | Possible | Possible | High |
| **Repeatable Read** | Prevented | Prevented | Possible (SQL) / Prevented (InnoDB) | Medium |
| **Serializable** | Prevented | Prevented | Prevented | Lowest |

**Phenomena explained:**

- **Dirty read:** reading uncommitted changes from another transaction.
- **Non-repeatable read:** same row read twice, different values (because another tx updated and committed).
- **Phantom read:** same range query returns different rows (because another tx inserted/deleted).

### Locking

| Lock | Allows |
|------|--------|
| **Shared (S)** | Multiple readers, no writers |
| **Exclusive (X)** | Single writer; no readers |

Plus:

- **Row-level locks:** finer-grain, more concurrency.
- **Table-level locks:** coarser, faster for bulk operations.
- **Intent locks:** signal that a transaction intends to lock rows in a table.

### Two-Phase Locking (2PL)

1. **Growing phase:** acquire locks.
2. **Shrinking phase:** release locks.
3. Ensures serializability but can deadlock.

### Lock Granularity Trade-off

| Coarse (table-level) | Fine (row-level) |
|---------------------|------------------|
| Less concurrency | More concurrency |
| Easier to manage | More lock manager overhead |
| Risk of contention | Risk of deadlocks |

### Optimistic vs Pessimistic Locking

- **Pessimistic:** lock the row, do work, release (e.g., `SELECT ... FOR UPDATE`).
- **Optimistic:** read with a version number, check on update; abort if version changed.

```sql
-- Optimistic concurrency
UPDATE products
SET stock = stock - 1, version = version + 1
WHERE id = 42 AND version = 5;
-- If 0 rows affected, someone else won the race.
```

---

## 11. MVCC, Deadlocks & Concurrency

### MVCC (Multi-Version Concurrency Control)

- Each row has **multiple versions** (e.g., via system columns `xmin`, `xmax` in PostgreSQL).
- Readers don't block writers; writers don't block readers.
- Snapshot taken at statement or transaction start.
- Cleanup via **vacuum** (PostgreSQL) or undo logs (MySQL InnoDB).

```sql
-- PostgreSQL hidden columns
SELECT xmin, xmax, * FROM products WHERE id = 1;
```

### Deadlocks

- Two transactions wait on each other's locks → permanent stall.
- DB detects via **wait-for graph** and **kills** one (the victim) by aborting it.

**Avoiding deadlocks:**

- Acquire locks in a **consistent order**.
- Keep transactions **short**.
- Use **lower isolation levels** when you can.

### Lost Update Problem

Without concurrency control:

```
T1: read balance = 100
T2: read balance = 100
T1: write balance = 90  (subtract 10)
T2: write balance = 80  (subtract 20) -- overwrites T1!
```

Solutions:

- `SELECT FOR UPDATE` (pessimistic).
- Version column + optimistic.
- `INSERT ... ON CONFLICT (id) DO UPDATE SET ...` (atomic upsert).

---

## 12. SQL vs NoSQL — When to Use What

### Comparison

| Aspect | SQL (Relational) | NoSQL |
|--------|------------------|-------|
| **Schema** | Fixed, strong typing | Flexible / dynamic |
| **Scaling** | Vertical; horizontal with sharding | Horizontal native |
| **Transactions** | ACID, multi-row | Often single-document only |
| **Query** | Rich SQL, joins | API or limited query |
| **Consistency** | Strong (configurable) | Eventual (default) |
| **Best for** | OLTP, financial data, complex relations | Massive scale, evolving schema, simple lookups |

### When SQL Wins

- Complex joins across multiple tables.
- Strong consistency (banking, inventory).
- Ad-hoc reporting with SQL.
- Mature tooling, decades of knowledge.

### When NoSQL Wins

- Massive scale (PBs, billions of records).
- Schema is unknown or rapidly evolving.
- Single-digit-millisecond reads at huge QPS (e.g., feeds, sessions).
- Specific data shapes (graph, vector, time-series).

### Hybrid Approach

Most modern systems use **both**: PostgreSQL for transactions + Redis for cache/sessions + Elasticsearch for search + ClickHouse for analytics.

---

## 13. Distributed Databases — CAP, PACELC, Replication, Sharding

### CAP Theorem

In a distributed system under network **partition** (P), you can pick **at most two** of:

- **C**onsistency — every read sees the latest write.
- **A**vailability — every request gets a response.
- **P**artition tolerance — system keeps operating despite network failures.

> Since partitions **always** happen in real networks, the real choice is **CP vs AP**.

| System | Choice | Why |
|--------|--------|-----|
| **HBase, MongoDB (default)** | CP | Refuses writes if can't replicate |
| **Cassandra, DynamoDB** | AP | Always accepts writes, reconciles later |
| **Redis Cluster** | configurable | Per-request consistency choice |

### PACELC

Extends CAP: **E**lse (when no partition), you trade **L**atency vs **C**onsistency.

- **DynamoDB / Cassandra:** PA/EL — available under partition, low-latency else.
- **HBase / MongoDB:** PC/EC — consistent under partition, consistent else.

### Replication Strategies

| Strategy | Description | Use Case |
|----------|-------------|----------|
| **Single leader** | All writes to leader; followers replicate | Classic web apps |
| **Multi-leader** | Multiple leaders; conflict resolution | Multi-region writes |
| **Leaderless** | Any node accepts writes (Quorum) | DynamoDB, Cassandra |
| **Synchronous** | Leader waits for replicas | Strong consistency, slower |
| **Asynchronous** | Fire-and-forget | Faster, risk of data loss |

### Sharding (Partitioning)

Split rows across multiple DBs:

| Type | How | Pros | Cons |
|------|-----|------|------|
| **Hash-based** | `hash(key) % N` | Even distribution | Rebalancing is painful |
| **Range-based** | `user_id 1–1M` on shard A | Range queries efficient | Hotspots (recent data) |
| **Directory-based** | Lookup service maps `key → shard` | Flexible | Extra hop, lookup must be HA |
| **Geographic** | `region` decides | Low latency | Cross-region queries expensive |

### Rebalancing & Hotspots

- **Consistent hashing** spreads keys around a ring → only `k/n` keys move when adding/removing a shard (vs `k` with modulo).
- **Virtual nodes / pre-sharding** help smooth distribution.
- **Hot keys** (a celebrity tweet) need their own mitigation: cache, queue, replicate per-tenant.

---

## 14. Query Optimization & EXPLAIN Plans

### Step 1: Run EXPLAIN

```sql
EXPLAIN ANALYZE
SELECT u.name, COUNT(o.id) AS orders
FROM users u
LEFT JOIN orders o ON u.id = o.user_id
WHERE u.created_at > '2024-01-01'
GROUP BY u.name;
```

Look for:

- **Seq Scan** (full table scan) on a large table — usually need an index.
- **Nested Loop** with very high row counts — consider Hash Join.
- **High loops** in a nested loop — bad index choice upstream.

### Common Optimizations

| Problem | Fix |
|---------|-----|
| Full table scan | Add index |
| Slow joins | Index on join key, both sides |
| Slow `COUNT(*)` | Approximate count, summary table |
| Slow `LIKE '%foo%'` | Full-text index (GIN/pg_trgm) |
| Slow `ORDER BY` | Index matches ORDER BY order |
| Slow pagination | Cursor-based pagination instead of OFFSET |
| Slow correlated subquery | Rewrite as JOIN |

### Pagination

```sql
-- Bad: OFFSET becomes slow as offset grows (DB scans + discards offset+limit rows)
SELECT * FROM items ORDER BY id LIMIT 20 OFFSET 1_000_000;

-- Good: cursor / keyset pagination
SELECT * FROM items WHERE id > 999_999 ORDER BY id LIMIT 20;
```

### ORM Pitfalls

- N+1 queries: lazy-loading related objects one by one. Use eager loading (`SELECT ... JOIN`).
- `SELECT *`: always select only the columns you need.
- Implicit transactions: every save wraps a tx; for bulk operations, manage explicitly.

---

## 15. Data Modeling Patterns & Anti-Patterns

### Patterns

| Pattern | Use |
|---------|-----|
| **Soft delete** | `deleted_at TIMESTAMP NULL` instead of `DELETE` |
| **Audit log** | Append-only table tracking every change |
| **Polymorphic association** | `resource_type, resource_id` pair (use sparingly) |
| **EAV (Entity-Attribute-Value)** | Flexible attributes (mostly an antipattern in OLTP) |
| **Tag tables** | `tag(id, name)` + `item_tag(item_id, tag_id)` |
| **Outbox pattern** | Store events in DB table; a worker publishes to queue (transactional publishing) |
| **Tree modeling** | Adjacency list (`parent_id`), nested sets, materialized path, closure table |

### Anti-Patterns

- Storing **multi-valued** data in a single column (CSV, JSON blob) — kills searchability.
- Using **float for money** — use `DECIMAL(10, 2)`.
- Frequent **`ALTER TABLE ADD COLUMN`** on huge tables in hot path (MySQL pre-8 was instant-instant; PG is fast; MySQL 8+ is fast).
- **Missing indexes on FK columns.**
- **EAV for everything** — only use when truly needed.
- UUID v1/v4 as clustered PK — causes page splits; use UUID v7 or BIGINT.

### Tree Modeling — When to Use What

| Approach | Read Tree | Find Children | Find Ancestors | Move Subtree | Insert |
|----------|-----------|---------------|----------------|--------------|--------|
| **Adjacency list** (`parent_id`) | Easy | Easy | Recursive CTE | Easy | Easy |
| **Nested sets** (`lft, rgt`) | Easy | Easy | Easy | Hard | Hard |
| **Materialized path** (`/a/b/c`) | Easy | Easy | String query | String update | Easy |
| **Closure table** | Easy | Easy | Easy | Easy | Moderate |

---

## 16. Storage Engines, WAL, LSM vs B-Tree

### Write-Ahead Log (WAL)

- Before modifying a page, append the change to a **log file** flushed to disk.
- Crash recovery: replay log to restore consistency.
- Powers durability for almost every modern RDBMS.

### B-Tree (Row-Oriented)

- Traditional choice: sorted pages on disk; reads are sequential.
- Optimized for **OLTP** with mixed reads/writes and range queries.
- Examples: PostgreSQL, MySQL InnoDB, Oracle.

### LSM Tree (Log-Structured Merge)

- Writes go to in-memory memtable, then flushed to **sorted string tables (SSTables)**.
- Reads may need to check multiple SSTables — mitigated by **bloom filters**.
- Optimized for **write-heavy** workloads (logs, time-series).
- Examples: Cassandra, RocksDB, LevelDB, ClickHouse.

### Column-Oriented Storage

- Each column stored separately → great for analytics (`SELECT AVG(price) FROM sales`).
- Excellent compression per column (similar values).
- Examples: ClickHouse, Parquet, Redshift, BigQuery.

---

## 17. Stored Procedures, Triggers, Views & Materialized Views

### Views

- Virtual table backed by a query.
- Doesn't store data; recomputed on read (unless `MATERIALIZED`).

```sql
CREATE VIEW active_users AS
SELECT id, name, email FROM users WHERE deleted_at IS NULL;
```

### Materialized Views

- Stores the result of a query physically.
- Needs `REFRESH MATERIALIZED VIEW` (manual or scheduled).
- Much faster reads; slower updates.

```sql
CREATE MATERIALIZED VIEW daily_revenue AS
SELECT DATE(created_at) AS day, SUM(amount) AS total
FROM orders GROUP BY DATE(created_at);

REFRESH MATERIALIZED VIEW CONCURRENTLY daily_revenue;  -- doesn't block readers
```

### Stored Procedures

- Pre-compiled code stored in the DB.
- Useful for batch jobs and operations that must run close to data.
- Modern apps usually prefer app-level logic + DB transactions.

### Triggers

- Auto-execute code on `INSERT / UPDATE / DELETE`.
- Use sparingly — they hide behavior and complicate debugging.

---

## 18. Quick Reference Card

| Concept | One-Line |
|---------|---------|
| ACID | Atomicity, Consistency, Isolation, Durability |
| 1NF | Atomic, no repeats |
| 2NF | + no partial dep |
| 3NF | + no transitive dep |
| BCNF | + every determinant is key |
| Clustered idx | Sorts table data physically |
| MVCC | Multiple row versions for concurrent readers/writers |
| WAL | Log first, modify page after |
| LSM | Log-structured merges, write-optimized |
| B-Tree | Sort-optimized, read-optimized |
| 2PL | Growing phase + shrinking phase |
| CAP | Pick 2 of C/A/P (P is mandatory) |
| Consistent hashing | Minimal key movement on rebalance |
| Optimistic lock | Version column, abort on mismatch |
| EXPLAIN | Shows the plan — read it before optimizing |
| Materialized view | Physical snapshot of a query |

---

## 19. Top Interview Q&A (40 entries)

**Q1: DELETE vs TRUNCATE vs DROP?**
- `DELETE`: DML, removes rows one by one, supports `WHERE`, can be rolled back, fires triggers, slows down with large row counts.
- `TRUNCATE`: DDL, removes all rows, fast (deallocates pages), resets identity, can't roll back in most DBs.
- `DROP`: removes the entire table (data + structure).

**Q2: Find the 2nd highest salary.**
```sql
SELECT MAX(salary) FROM Employees WHERE salary < (SELECT MAX(salary) FROM Employees);
-- Or
SELECT salary FROM Employees ORDER BY salary DESC LIMIT 1 OFFSET 1;
-- Or (with ties)
SELECT DISTINCT salary FROM Employees ORDER BY salary DESC LIMIT 1 OFFSET 1;
```

**Q3: What is a deadlock and how does the DB handle it?**
Two transactions hold locks the other needs. The DB builds a **wait-for graph** and aborts one (the **victim**) by rolling it back; the client can retry.

**Q4: Clustered vs non-clustered index?**
Clustered: physical order = index order; one per table; data is in the index. Non-clustered: separate structure with row pointers; many per table; leaf nodes point to data.

**Q5: ACID vs BASE?**
ACID = strong guarantees (SQL). BASE = Basically Available, Soft state, Eventually consistent (most distributed NoSQL defaults).

**Q6: What is a foreign key?**
A column that references the primary key of another table, enforcing referential integrity.

**Q7: What is an ORM?**
Object-Relational Mapping — library that converts between objects in code and rows in DB (e.g., Hibernate, SQLAlchemy, Entity Framework, Sequelize).

**Q8: Difference between WHERE and HAVING?**
`WHERE` filters **rows** before aggregation. `HAVING` filters **groups** after aggregation (can use aggregate functions).

**Q9: What is a covering index?**
An index that contains **all** columns needed for a query → the DB can answer it from the index alone without reading the table.

**Q10: Why use `VARCHAR` vs `CHAR`?**
`VARCHAR(n)` uses only the bytes needed (up to n). `CHAR(n)` always uses n bytes (pads with spaces). Prefer `VARCHAR` for variable-length data.

**Q11: What is normalization?**
Process of organizing tables to reduce redundancy and anomalies by decomposing them according to normal forms (1NF through 5NF).

**Q12: Difference between UNION and UNION ALL?**
`UNION` removes duplicates (extra sort). `UNION ALL` keeps duplicates — faster, use when duplicates are OK.

**Q13: What is a CTE?**
Common Table Expression — a temporary named result set defined with `WITH`. Improves readability and enables recursion.

**Q14: What is a window function?**
A function that computes over a set of rows related to the current row without collapsing them. Examples: `ROW_NUMBER`, `RANK`, `LAG`, `SUM OVER`.

**Q15: What is a primary key vs unique key?**
Both enforce uniqueness and create an index. **Primary key** is `NOT NULL` and (typically) clustered; **unique key** allows `NULL`. A table has one PK but many unique keys.

**Q16: What is the N+1 query problem?**
ORM fetches a list (1 query), then issues one query **per item** to load related data (N queries). Fix with eager loading or join-fetch.

**Q17: How does indexing work under the hood?**
Most use B-tree (balanced tree) sorted by the indexed column — `O(log n)` lookups, range-friendly. Some use hash (`O(1)` but no range).

**Q18: What is phantom read?**
In a transaction, two identical range queries return a different set of rows because another transaction inserted/deleted matching rows in between.

**Q19: Why use `EXISTS` over `IN`?**
`EXISTS` short-circuits on first match and is often faster for subqueries on large tables; `IN` materializes the whole subquery.

**Q20: Difference between `REPLACE` and `INSERT ON DUPLICATE KEY UPDATE`?**
`REPLACE` deletes the existing row then inserts (fires triggers, breaks foreign keys). `INSERT ... ON DUPLICATE KEY UPDATE` updates in place — safer.

**Q21: How do you find duplicate rows?**
```sql
SELECT email, COUNT(*) FROM users GROUP BY email HAVING COUNT(*) > 1;
```

**Q22: What is a self-join? When do you need it?**
A join where a table is joined to itself. Used for hierarchical data (manager → reports), comparing rows, finding pairs.

**Q23: Why is `SELECT *` bad?**
Transfers unneeded columns, breaks when schema changes, prevents covering indexes, slower over the wire.

**Q24: What is a materialized view?**
A view whose result is **physically stored**. Faster reads; must be refreshed (manually or scheduled) to reflect new data.

**Q25: Difference between stored procedure and function?**
Procedures can run DML and return multiple result sets; functions must return a value and are typically used in SELECT. Transactions differ across DBs.

**Q26: What is the isolation level default?**
- PostgreSQL: **Read Committed.**
- MySQL InnoDB: **Repeatable Read.**
- SQL Server: **Read Committed.**

**Q27: CAP — which two to pick?**
In real systems P is non-negotiable, so choose **CP** (consistency over availability — refuse writes during partition) or **AP** (availability over consistency — serve from any node, reconcile later).

**Q28: What is consistent hashing?**
Hash keys to positions on a ring; each shard owns a range. Adding a shard moves only the keys in that range, not the entire dataset.

**Q29: What is optimistic vs pessimistic locking?**
Pessimistic: lock row at read time (`SELECT ... FOR UPDATE`); others wait. Optimistic: read with version, check on write; abort if version changed.

**Q30: Difference between replication and sharding?**
Replication: copies of same data for read scaling and HA. Sharding: partitioning data so each node holds **different** rows for write scaling.

**Q31: How does two-phase commit (2PC) work?**
A coordinator asks participants to **prepare**; if all agree, asks them to **commit**. Atomic across heterogeneous systems but slow and brittle.

**Q32: What is eventual consistency?**
Given enough time and no new writes, all replicas converge to the same value. Powers DNS, Cassandra, DynamoDB.

**Q33: Slow query — what's your first step?**
Run `EXPLAIN ANALYZE`. Identify sequential scans on large tables. Check indexes on `WHERE`, `JOIN`, `ORDER BY` columns. Update statistics (`ANALYZE`).

**Q34: What is a temp table vs table variable?**
Temp table (`#temp`) has stats, supports indexes, persists for the session. Table variable (`@table`) has limited stats, no indexes, scoped to batch.

**Q35: Difference between OLTP and OLAP?**
OLTP: many short transactions, current data, row-oriented. OLAP: few long queries, historical data, column-oriented or denormalized for analytics.

**Q36: What is a partial / filtered index?**
An index over only a subset of rows (e.g., `WHERE deleted_at IS NULL`). Smaller and faster when most queries target that subset.

**Q37: What is index cardinality?**
The number of distinct values in a column. **High-cardinality** columns (email, ID) make great indexes. **Low-cardinality** (boolean, status) are usually poor.

**Q38: What is a covering index?**
An index that includes all columns referenced by a query, so the DB reads only the index without going to the table.

**Q39: How would you migrate a billion-row table?**
Schema shadowing or expand-and-contract pattern: add new column, dual-write, backfill in chunks, cut reads over, drop old column. Never `ALTER` in place on huge tables.

**Q40: What is the `information_schema`?**
A standard set of read-only views describing the schema of the database itself — tables, columns, indexes. Used for introspection and tooling.

---

> **Next:** read `03-computer-networks/README.md` to pair the data layer with the network layer that moves it.
