# Operating Systems — Interview Notes

## 1. Process vs Thread

| Aspect | Process | Thread |
|--------|---------|--------|
| Definition | Independent execution unit with own memory | Subset of a process; shares memory with other threads |
| Memory | Separate address space | Shared heap; separate stack |
| Creation cost | Heavy (fork/exec) | Lightweight |
| Communication | IPC (pipes, sockets, shared mem) | Shared variables (with sync) |
| Crash impact | One process down ≠ others | Thread crash can take down process |

### Threads
- Each thread has its own: **stack, registers, PC, TLS**
- Threads share: **heap, code, data segment, OS resources (open files)**

---

## 2. CPU Scheduling Algorithms

| Algorithm | Type | Pros | Cons |
|-----------|------|------|------|
| FCFS | Non-preemptive | Simple | Convoy effect |
| SJF (Shortest Job First) | Non-preemptive | Optimal avg wait time | Starvation; need burst time |
| SRTF (Shortest Remaining Time First) | Preemptive | Even better avg wait | Starvation; context switch overhead |
| Round Robin | Preemptive | Fair, good for time-sharing | Performance depends on quantum |
| Priority | Either | Important jobs first | Starvation (use aging) |
| MLFQ (Multi-Level Feedback Queue) | Preemptive | Adaptive; favors interactive | Complex |

---

## 3. Deadlock

**Coffman Conditions (all 4 must hold):**
1. **Mutual Exclusion** — only one process uses the resource at a time
2. **Hold and Wait** — process holds one resource while waiting for another
3. **No Preemption** — resources can't be forcibly taken
4. **Circular Wait** — circular chain of waiting processes

### Solutions
- **Prevention:** Break one of the 4 conditions.
- **Avoidance:** Banker's Algorithm — only allocate if safe state remains.
- **Detection & Recovery:** Detect cycles (resource allocation graph) and abort/rollback.
- **Ignore:** Ostrich algorithm (used by Unix/Windows most of the time).

### Banker's Algorithm
- Maintains `Max`, `Allocation`, `Need` matrices.
- `Need[i] = Max[i] - Allocation[i]`.
- A state is **safe** if there exists a sequence of all processes such that each can complete.

---

## 4. Process Synchronization

### Race Condition
Multiple threads access shared data concurrently; outcome depends on interleaving.

### Critical Section
Code segment accessing shared resources. Must satisfy:
- **Mutual Exclusion**
- **Progress**
- **Bounded Waiting**

### Solutions
1. **Mutex (Lock)** — only one thread in critical section.
2. **Semaphore** — counter; `wait()` decrements, `signal()` increments.
 - Binary semaphore (0/1) ≈ mutex (but no ownership)
 - Counting semaphore (N)
3. **Monitor** — high-level synchronization construct (mutex + condition vars).
4. **Condition Variables** — wait/signal mechanism.

### Classic Problems
- **Producer-Consumer** — bounded buffer + mutex + 2 semaphores (empty/full).
- **Readers-Writers** — multiple readers OR single writer.
- **Dining Philosophers** — deadlock potential; solutions: resource hierarchy, arbitrator.

---

## 5. Memory Management

### Virtual Memory
Each process has its own **virtual address space**, mapped to physical memory via **page tables**.

### Paging
- Memory divided into **pages** (virtual) and **frames** (physical).
- Page table maps virtual pages → physical frames.
- **TLB** (Translation Lookaside Buffer) caches recent translations.

### Segmentation
- Memory divided by **logical segments** (code, data, stack, heap).
- Each segment has variable size.
- Segment number + offset.

### Page Replacement Algorithms
When a page fault occurs and no free frame:
- **FIFO** — replace oldest. Suffers Belady's anomaly.
- **LRU** (Least Recently Used) — replace least recently used. Best, but expensive.
- **Optimal** — replace page not used for longest in future. Theoretical optimum.
- **Clock (Second Chance)** — approximation of LRU using a reference bit.

### Thrashing
When a system spends more time swapping pages than executing. Caused by excessive paging.
- Fix: reduce multiprogramming, better page replacement, more memory.

---

## 6. File Systems

### Structure
- **Boot block** → **Superblock** → **Inode/FAT** → **Data blocks**

### Allocation Methods
- **Contiguous** — fast seq access; external fragmentation.
- **Linked** — no external frag; slow random access.
- **Indexed** — inode points to data blocks; balance of speed/flexibility.

### Free Space Management
- **Bitmap** — bit per block.
- **Linked list** — pointer to next free block.
- **Grouping / Counting.**

---

## 7. Inter-Process Communication (IPC)

| Method | Description |
|--------|-------------|
| Pipes | Unidirectional, related processes |
| Named Pipes (FIFO) | Bidirectional, unrelated processes |
| Message Queues | Discrete messages |
| Shared Memory | Fastest; requires sync |
| Sockets | Network/Unix domain |
| Signals | Software interrupt |
| Semaphores | (Also for sync) |

---

## 8. Important Concepts

### Context Switch
Saving state of one process/thread and loading another. Pure overhead.

### Kernel vs User Mode
- **Kernel mode:** full access to hardware
- **User mode:** restricted; syscalls cross the boundary

### System Calls
`open`, `read`, `write`, `fork`, `exec`, `wait`, `exit`, `kill`, `mmap`, etc.

### Interrupts
Hardware signals to CPU; trigger interrupt handler in kernel.

---

## 9. Top Interview Q&A

**Q: What is a zombie process?**
A process that has completed but its entry remains in the process table because its parent hasn't called `wait()`. Killed by `wait()`.

**Q: What is an orphan process?**
A process whose parent has terminated; adopted by init (PID 1).

**Q: Difference between mutex and semaphore?**
Mutex has ownership (only the locker can unlock); semaphore doesn't. Binary semaphore ≈ mutex but without ownership.

**Q: What is starvation?**
A process is indefinitely denied resources. Aging is a solution.

**Q: What is the difference between preemptive and non-preemptive scheduling?**
Preemptive allows OS to forcibly take CPU; non-preemptive lets process run to completion or I/O.

**Q: Belady's anomaly?**
FIFO can have more page faults with more frames; LRU/Optimal don't.

**Q: Why is context switching expensive?**
Save/restore registers, switch page tables, TLB flush, cache cold start.

**Q: User-level vs Kernel-level threads?**
User threads: managed by user library; fast context switch; one blocking sys call blocks all. Kernel threads: OS-managed; slower switch; true parallelism on multi-CPU.
