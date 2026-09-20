# DBMS & SQL — Interview Notes

## 1. ACID Properties

| Property | Meaning |
|----------|---------|
| **Atomicity** | Transaction is all-or-nothing |
| **Consistency** | DB moves from one valid state to another |
| **Isolation** | Concurrent transactions don't interfere |
| **Durability** | Committed data survives crashes |

---

## 2. Normalization

### 1NF (First Normal Form)
- Atomic values (no multi-valued cells)
- Each row unique (primary key)
- No repeating groups

### 2NF
- 1NF + no **partial dependency** (every non-key column depends on the whole primary key)

### 3NF
- 2NF + no **transitive dependency** (non-key columns don't depend on other non-key columns)

### BCNF (Boyce-Codd)
- 3NF + every determinant is a candidate key
- Stricter than 3NF

### 4NF / 5NF
- 4NF: no multi-valued dependencies
- 5NF: no join dependencies (rare in practice)

### Example: When to Denormalize
- Read-heavy workloads
- Reporting/analytics
- Trade-off: write performance and redundancy vs read speed

---

## 3. SQL Essentials

### Joins
```sql
-- INNER JOIN: matching rows in both
SELECT * FROM A INNER JOIN B ON A.id = B.aid;

-- LEFT JOIN: all from A, matched from B (or NULL)
SELECT * FROM A LEFT JOIN B ON A.id = B.aid;

-- FULL OUTER JOIN: all rows from both
SELECT * FROM A FULL OUTER JOIN B ON A.id = B.aid;

-- CROSS JOIN: cartesian product
SELECT * FROM A CROSS JOIN B;

-- SELF JOIN
SELECT e.name, m.name FROM Employee e LEFT JOIN Employee m ON e.mgr_id = m.id;
```

### Subqueries
```sql
-- Scalar
SELECT * FROM Employees WHERE salary > (SELECT AVG(salary) FROM Employees);

-- Correlated (runs per row)
SELECT name FROM Employees e WHERE salary > (
 SELECT AVG(salary) FROM Employees WHERE dept_id = e.dept_id
);
```

### Window Functions (Analytic)
```sql
SELECT
 name,
 salary,
 ROW_NUMBER() OVER (PARTITION BY dept_id ORDER BY salary DESC) AS rn,
 RANK() OVER (PARTITION BY dept_id ORDER BY salary DESC) AS rk,
 DENSE_RANK() OVER (PARTITION BY dept_id ORDER BY salary DESC) AS dr,
 LAG(salary) OVER (ORDER BY salary) AS prev_salary
FROM Employees;
```

### CTEs (Common Table Expressions)
```sql
WITH high_earners AS (
 SELECT * FROM Employees WHERE salary > 100000
)
SELECT dept_id, COUNT(*) FROM high_earners GROUP BY dept_id;
```

### Aggregate Functions
`COUNT, SUM, AVG, MIN, MAX` — combined with `GROUP BY`, `HAVING`.

---

## 4. Indexing

### Types
- **B-Tree Index** — default; good for range queries, equality.
- **B+ Tree Index** — data only in leaves; better range scans, sequential access.
- **Hash Index** — equality only; no range.
- **Bitmap Index** — low-cardinality columns (gender, status).
- **Clustered Index** — sorts table data physically (1 per table, usually PK).
- **Non-Clustered** — separate structure with pointers.

### When Indexes Hurt
- Write-heavy tables (each insert updates indexes)
- Small tables (full scan is faster)
- Low-cardinality columns

### Composite Index
- Order matters: `(a, b, c)` index helps queries on `(a)`, `(a, b)`, `(a, b, c)` — but not `(b)` alone.

---

## 5. Transactions & Concurrency

### Isolation Levels (lowest to highest)

| Level | Dirty Read | Non-Repeatable Read | Phantom Read |
|-------|------------|---------------------|--------------|
| Read Uncommitted | Possible | Possible | Possible |
| Read Committed | Prevented | Possible | Possible |
| Repeatable Read | Prevented | Prevented | Possible |
| Serializable | Prevented | Prevented | Prevented |

### Locking
- **Shared (S) lock** — read; multiple OK.
- **Exclusive (X) lock** — write; only one.
- **2PL (Two-Phase Locking):**
 - Growing phase: acquire locks
 - Shrinking phase: release locks
- **Deadlocks** can occur in 2PL — DB detects via wait-for graph and aborts victim.

### MVCC (Multi-Version Concurrency Control)
- Readers don't block writers; each row has multiple versions.
- Used by PostgreSQL, MySQL InnoDB, Oracle.

---

## 6. SQL vs NoSQL

| Feature | SQL | NoSQL |
|---------|-----|-------|
| Schema | Fixed, rigid | Schema-less / dynamic |
| Scaling | Vertical (mostly) | Horizontal (sharding) |
| Transactions | ACID | Eventually consistent (mostly) |
| Query | SQL, joins | API or limited query |
| Best for | Complex queries, OLTP | Big data, flexible schema, scale |

### CAP Theorem
In a distributed system, you can have **at most 2 of 3**:
- **C**onsistency
- **A**vailability
- **P**artition tolerance

Since P is mandatory in real networks, you choose between CP (e.g., MongoDB, HBase) and AP (e.g., Cassandra, DynamoDB).

---

## 7. Sharding & Replication

### Sharding (Partitioning)
Split data across multiple DBs:
- **Horizontal** — split rows by hash of key (e.g., user_id % N).
- **Vertical** — split tables/columns.
- **Directory-based** — lookup table maps key → shard.

Pros: scale writes, large datasets.
Cons: cross-shard joins/queries hard, rebalancing.

### Replication
- **Master-Slave** — writes to master, reads from slaves. Read scaling.
- **Master-Master** — multi-master; conflict resolution tricky.
- **Synchronous** vs **Asynchronous**.

---

## 8. Query Optimization

1. Look at **EXPLAIN / EXPLAIN ANALYZE** output
2. Use indexes wisely
3. Avoid `SELECT *`
4. Avoid correlated subqueries (use joins)
5. Use LIMIT for pagination
6. Denormalize for hot read paths
7. Partitioning for huge tables

---

## 9. Top Interview Q&A

**Q: Difference between DELETE, TRUNCATE, DROP?**
- `DELETE`: DML, removes rows, can use WHERE, can be rolled back.
- `TRUNCATE`: DDL, removes all rows, faster, can't rollback (in most DBs).
- `DROP`: DDL, removes the table itself.

**Q: What is a deadlock? How to handle?**
Two transactions waiting for each other's locks. DB detects via wait-for graph; kills victim (rollback).

**Q: Difference between clustered and non-clustered index?**
Clustered: data physically sorted by index. Non-clustered: separate structure with row pointers.

**Q: ACID vs BASE?**
ACID: strong consistency (SQL). BASE: Basically Available, Soft state, Eventually consistent (NoSQL).

**Q: What is a foreign key?**
A column that references the primary key of another table, enforcing referential integrity.

**Q: What is an ORM?**
Object-Relational Mapping — translates between objects in code and rows in DB (e.g., Hibernate, SQLAlchemy).

**Q: How would you find the second highest salary?**
```sql
SELECT MAX(salary) FROM Employees WHERE salary < (SELECT MAX(salary) FROM Employees);
-- Or
SELECT salary FROM Employees ORDER BY salary DESC LIMIT 1 OFFSET 1;
```
