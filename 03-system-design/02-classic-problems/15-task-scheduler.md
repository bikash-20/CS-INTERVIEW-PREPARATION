# Design Distributed Task Scheduler (Airflow / Temporal / Cron-at-Scale)

Tests your understanding of distributed coordination, retries, and failure isolation.

---

## 1. Requirements

### Functional
- Schedule tasks at a specific time or interval (cron-style).
- DAGs (directed acyclic graphs) of dependent tasks.
- Manual triggers and ad-hoc runs.
- Retries with backoff.
- Priorities / SLAs.
- Visibility: status, logs, run history.

### Non-Functional
- **Exactly-once semantics** where possible (with idempotent tasks).
- High availability — scheduler must keep firing.
- Horizontal scale — workers come and go.
- **At-least-once delivery** for triggers.

---

## 2. Capacity Estimation

Assume 10M tasks/day.

| Metric | Value |
|---|---|
| Tasks/day | 10M |
| Tasks/sec avg | ~120, peak ~1,000 |
| DAGs | 10K |
| Concurrent running tasks (peak) | 100K |
| Schedule precision | within 5 seconds |

---

## 3. High-Level Design

```
 DAG definitions (YAML / Python)
 │
 ▼
 Scheduler Service
 │
 ├─ Evaluates cron / dependencies every second
 ├─ Inserts due tasks into Kafka (priority queue)
 │
 ▼
 Workers (auto-scaling pool)
 │
 ├─ Pull task from queue
 ├─ Run task (in container / VM / external)
 └─ Report result → State DB
 │
 ▼
 Metadata DB (Postgres): DAG runs, task states, logs pointers
 │
 ▼
 UI / API
```

---

## 4. Components

### Scheduler
- Watches DAG definitions + clock.
- Computes next run times.
- Pushes ready tasks to queue.
- Stateless; multiple instances via leader election.

### Queue
- Kafka (or Redis Streams) partitioned by `dag_id`.
- Tasks placed in partition = ordering for retries.
- DLQ for poison pills.

### Workers
- Stateless.
- Pull from Kafka.
- Execute task in an isolated environment (Docker, ECS, k8s pod).
- Heartbeat while running; requeue on lease expiry.

### State DB
- Postgres: DAGs, runs, task attempts, logs.
- Sharded if needed.
- Reads feed the UI.

### Lock service (for one-off / singleton tasks)
- e.g., `scheduler-leader` lock in Zookeeper / etcd.

---

## 5. Task Lifecycle

```
 SCHEDULED → QUEUED → RUNNING → SUCCESS
 ↘ FAILED → RETRY_QUEUED → RUNNING ...
 ↘ FAILED (no retries left) → DEAD
```

Each state transition is recorded with timestamps.

---

## 6. DAGs

A DAG = tasks with dependencies:
```yaml
dag:
 id: nightly_etl
 schedule: "0 2 * * *"
 tasks:
 - id: extract
 - id: transform
 depends_on: [extract]
 - id: load
 depends_on: [transform]
 retries: 3
 backoff: exponential
 timeout: 1h
```

Scheduler fires `extract`; on success, fires `transform`; and so on.

---

## 7. Retries & Backoff

- Exponential backoff: 1 min, 5 min, 30 min.
- Capped retries (e.g., 5).
- Different policies per task.

---

## 8. Idempotency

The task should be safe to re-run.
- Pass a unique `run_id` to the task.
- Task uses it for upserts / de-duplication in the destination.

---

## 9. Long-Running Tasks

- Worker holds lease; sends heartbeat every 30 s.
- Scheduler / coordinator misses heartbeat → assume dead → retry.

---

## 10. SLA Monitoring

- `expected_duration` per task.
- If a task is still RUNNING past `expected + slack` → alert.

---

## 11. Schema

```
Table: dags
 dag_id VARCHAR PK
 definition JSON
 schedule VARCHAR
 owner VARCHAR
 enabled BOOL

Table: dag_runs
 run_id VARCHAR PK (dag_id + scheduled_ts)
 dag_id VARCHAR
 scheduled_at TIMESTAMP
 started_at TIMESTAMP
 finished_at TIMESTAMP NULL
 state ENUM(queued, running, success, failed)

Table: task_instances
 run_id VARCHAR
 task_id VARCHAR
 state ENUM
 attempts INT
 started_at, finished_at
 logs_url TEXT
 PRIMARY KEY (run_id, task_id, attempts)
```

---

## 12. Optimizations

- **Task-level parallelism** — workers per pool.
- **Concurrency limits** per DAG to avoid stampedes.
- **Priority queues** for important tasks.
- **Catchup window** — limit how many missed runs to backfill.
- **Pools** — dedicate workers to a tenant or type.

---

## 13. Failure Modes

| Failure | Mitigation |
|---|---|
| Worker dies mid-task | Lease expires; another worker retries. |
| Task consistently fails | Mark dead; alert; manual intervention. |
| Scheduler down | Leader failover; missed runs catch up. |
| Queue full | Backpressure: stop scheduling new tasks. |
| DB slow | Read replicas; cache for UI. |

---

## 14. API Design

```
POST /api/v1/dags
POST /api/v1/dags/{id}/trigger
GET /api/v1/dags/{id}/runs?since=...
GET /api/v1/runs/{run_id}/tasks
POST /api/v1/tasks/{run_id}/{task_id}/retry
GET /api/v1/tasks/{run_id}/{task_id}/logs
```

---

## 15. Follow-up Questions

**Q: How to ensure exactly-once task execution?**
You can't — only at-least-once. Idempotency in the task makes it effectively exactly-once.

**Q: How to avoid duplicate triggers when two schedulers run?**
Leader election with lease; only leader inserts new runs.

**Q: How to handle backfills (run missed days)?**
Generate N runs at once, but throttle to avoid drowning workers.

**Q: How to monitor at scale?**
Track pending / running / failed counts; per-DAG SLA tracking.

**Q: How to migrate a DAG without losing history?**
Versioned DAGs; keep old version's runs; new runs use new version.

**Q: How to schedule tasks at sub-second precision?**
Use a faster scheduler (custom loop) or move to event-driven triggers.

**Q: How to support different worker pools (Python, JVM)?**
Worker types register with a label; tasks specify their preferred label.

---

## 16. End-to-End Diagram

```
 Cron / event
 │
 ▼
 Scheduler (leader) ──► Postgres (state)
 │
 ▼ insert due tasks
 Kafka (priority queue)
 │
 ▼ pull
 Workers (Docker / k8s / ECS)
 │
 ├─ execute
 ├─ heartbeat
 └─ result → Postgres
 │
 UI / dashboards
```
