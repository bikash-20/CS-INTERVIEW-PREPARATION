# Operating Systems — Comprehensive Interview Guide

A complete, plain-English guide to operating system concepts asked in software engineering interviews. Every section has theory, examples, and 8-15 follow-up Q&A entries.

---

## Table of Contents

1. [Process vs Thread vs Coroutine](#1-process-vs-thread-vs-coroutine)
2. [CPU Scheduling](#2-cpu-scheduling)
3. [Deadlocks](#3-deadlocks)
4. [Process Synchronization](#4-process-synchronization)
5. [Inter-Process Communication (IPC)](#5-inter-process-communication-ipc)
6. [Memory Management — Virtual Memory & Paging](#6-memory-management--virtual-memory--paging)
7. [Page Replacement Algorithms](#7-page-replacement-algorithms)
8. [File Systems](#8-file-systems)
9. [I/O Scheduling & Disk Management](#9-io-scheduling--disk-management)
10. [Kernel, User Mode, System Calls](#10-kernel-user-mode-system-calls)
11. [Virtualization & Containers](#11-virtualization--containers)
12. [Security & Protection](#12-security--protection)
13. [Quick Reference Card](#13-quick-reference-card)

---

## 1. Process vs Thread vs Coroutine

### Theory

A **process** is an independent execution unit with its own address space. A **thread** is a lightweight execution unit that shares the process's address space. A **coroutine** is a user-mode cooperative unit, even lighter than a thread.

### Process

- Has its own **virtual address space**: code, data, heap, stack.
- Has at least one thread.
- Isolated from other processes (memory protection).
- Creation: `fork` (copy) or `exec` (replace).

### Thread

- Shares heap, code segment, and OS resources with other threads in the same process.
- Has its own **stack**, registers, PC, TLS.
- Lighter to create/switch than processes.
- Communication via shared variables (with sync).

### Coroutine

- User-space cooperative multitasking.
- Suspend/resume without OS scheduling.
- Examples: goroutines (Go), async/await in JS/Python, Kotlin coroutines.
- Many coroutines can run on a single thread.

### Q&A

**Q1: Process vs thread — table comparison.**
| Aspect | Process | Thread |
|---|---|---|
| Address space | Isolated | Shared (heap, code) |
| Stack | Own | Own |
| Creation cost | Heavy (fork/exec) | Lightweight |
| Communication | IPC | Shared memory |
| Crash impact | Isolated | Can crash entire process |
| Parallelism | Independent cores | Same process |

**Q2: User-level vs kernel-level threads?**
- **User-level** — managed by user library; fast switch; **one blocking syscall blocks all**.
- **Kernel-level** — managed by OS; slower switch; real parallelism.

**Q3: Context switch steps.**
1. Save registers (PC, SP, general purpose) of current thread/process to PCB/TCB.
2. Update scheduler state.
3. Load registers of next thread.
4. Switch memory maps (if process).
5. TLB flush (slower for processes).
6. CPU cache is cold → first few instructions are slow.

**Q4: Why are context switches expensive?**
- Save/restore state.
- TLB flush (process switch).
- Cache pollution.
- Scheduler overhead.
On modern Linux, ~1-10 μs for a thread switch.

**Q5: Zombie vs orphan process.**
- **Zombie** — finished but parent hasn't called `wait()`. Entry still in process table. Killed by `wait()`.
- **Orphan** — parent died. Adopted by init (PID 1).

**Q6: fork() in C.**
```c
pid_t pid = fork();
if (pid == 0) {
 // child — running the same program
 execlp("/bin/ls", "ls", NULL);
} else {
 // parent
 waitpid(pid, NULL, 0);
}
```

**Q7: When to use threads vs processes?**
- **Threads** — shared state, low overhead (e.g., request handlers in a web server).
- **Processes** — need isolation (security, reliability) or scaling across machines.

**Q8: Coroutines vs threads?**
- Coroutines are **cooperative** (yield voluntarily); threads are **preemptive** (OS schedules).
- Coroutines are cheaper; thousands fit in one thread.
- Threads can use multiple cores; coroutines on one OS thread can't (unless worker pool).

**Q9: What is a fiber?**
Microsoft Windows term for user-mode cooperative threads. Similar concept to goroutines.

**Q10: Why are coroutines gaining popularity?**
Cheap concurrency for I/O-bound work, simpler than callback hell, deterministic scheduling.

---

## 2. CPU Scheduling

### Theory

The **scheduler** decides which runnable process/thread gets the CPU. Goals: minimize wait time, turnaround, response time, and maximize throughput and CPU utilization.

### Scheduling algorithms

| Algorithm | Type | Pros | Cons |
|---|---|---|---|
| FCFS | Non-preemptive | Simple | Convoy effect |
| SJF | Non-preemptive | Optimal avg wait | Starvation; needs burst time |
| SRTF | Preemptive SJF | Best avg wait | Starvation; overhead |
| Round Robin | Preemptive | Fair, time-sharing | Quantum-dependent |
| Priority | Either | Importance first | Starvation → use aging |
| MLFQ | Preemptive, adaptive | Good for mixed workloads | Tuning complex |

### Q&A

**Q1: What is the convoy effect?**
A long CPU-bound process blocks all I/O-bound processes behind it in FCFS.

**Q2: Round Robin quantum choice.**
- Too large → degenerates to FCFS.
- Too small → context switch overhead dominates.
- Rule of thumb: a few ms to tens of ms.

**Q3: Starvation and how to avoid?**
A process never gets CPU. Solutions:
- **Aging** — increase priority as wait time grows.
- **Fair-share scheduling** — ensure all users get proportional CPU.

**Q4: Preemptive vs non-preemptive.**
- **Non-preemptive** — process runs to completion or I/O.
- **Preemptive** — OS can forcibly take CPU (after quantum or higher-priority arrival).

**Q5: Throughput vs latency.**
- **Throughput** — processes per unit time.
- **Latency / turnaround / wait time** — time per process.
Some algorithms optimize one at the cost of the other.

**Q6: Real-time scheduling?**
- Rate Monotonic (fixed priority by period).
- Earliest Deadline First (dynamic priority).
- Used in embedded systems and robotics.

**Q7: Linux CFS (Completely Fair Scheduler)?**
Linux's default scheduler. Tracks virtual runtime per task; always picks the task with lowest vruntime. Red-black tree for O(log N) lookup.

**Q8: Multi-level Feedback Queue?**
Classifies processes by behavior: CPU-bound processes get demoted to lower-priority queues, I/O-bound stay high. Adapts without knowing a priori.

**Q9: What is a Gantt chart?**
A horizontal bar chart showing which process runs when. Used to visualize scheduling.

---

## 3. Deadlocks

### Theory

A deadlock is a state where a set of processes are each waiting for a resource held by another in the set, so none can proceed.

### Coffman Conditions (all 4 needed)
1. **Mutual Exclusion** — resource is non-sharable.
2. **Hold and Wait** — process holds at least one resource while waiting for another.
3. **No Preemption** — resources can't be forcibly taken.
4. **Circular Wait** — circular chain of processes each waiting for next's resource.

### Solutions

| Strategy | What |
|---|---|
| **Prevention** | Break one Coffman condition. |
| **Avoidance** | Banker's algorithm; only allocate if safe state. |
| **Detection + Recovery** | Detect cycles; abort/rollback victim. |
| **Ignore** | Ostrich algorithm (used by Unix/Windows most of the time). |

### Q&A

**Q1: Banker's algorithm.**
Maintains `Max`, `Allocation`, `Need` matrices. `Need[i] = Max[i] - Allocation[i]`. A state is safe if there's an order where each process can complete. Avoid allocation that leads to unsafe state.

**Q2: Resource Allocation Graph.**
Nodes: processes + resource types. Edges: assignment (P→R), request (R→P). **Cycle ⇒ deadlock** (if each resource has only one instance).

**Q3: Deadlock vs starvation.**
- Deadlock — all processes blocked forever.
- Starvation — one process never gets resource, others do.

**Q4: How to break circular wait in practice?**
**Order resources** — every process acquires resources in a global order. Impossible to form a cycle.

**Q5: Lock ordering example.**
```java
void transfer(Account a, Account b) {
 // Always lock lower-id first
 Account first = a.id < b.id ? a : b;
 Account second = a.id < b.id ? b : a;
 synchronized (first) { synchronized (second) { /* move money */ } }
}
```

**Q6: Livelock?**
Processes keep changing state in response to each other but never make progress. Example: two people trying to pass in a hallway and both stepping aside.

**Q7: Priority inversion?**
Low-priority process holds a lock needed by a high-priority process. Solution: **priority inheritance** (low-priority process temporarily gets the higher priority).

**Q8: Detect & recover approach.**
Used by databases (deadlock detection thread). Victim: abort cheapest-running transaction; release its locks.

**Q9: Why does Linux ignore deadlocks?**
Deadlocks are rare in practice; the overhead of prevention hurts throughput. Apps handle their own locking.

**Q10: Can you have deadlock with a single thread?**
No — circular wait requires at least two.

**Q11: Can two threads deadlock on one mutex?**
No — mutual exclusion means only one can hold it.

---

## 4. Process Synchronization

### Theory

When multiple threads access shared state, you need **mutual exclusion** to prevent race conditions.

### Synchronization primitives

| Primitive | Purpose |
|---|---|
| **Mutex** | One thread at a time; ownership. |
| **Semaphore** | Counter; `wait()` (P) decrements, `signal()` (V) increments. Counting (N) or binary (1). |
| **Monitor** | High-level: mutex + condition vars. |
| **Condition variable** | Block until a condition holds. |
| **Spinlock** | Busy-wait; short critical sections only. |
| **Read-Write lock** | Multiple readers OR one writer. |

### Critical Section Requirements
- **Mutual Exclusion**
- **Progress** — only processes in CS can decide who enters.
- **Bounded Waiting** — no starvation.

### Q&A

**Q1: Race condition?**
Outcome depends on the order of interleaved accesses to shared data.

**Q2: Mutex vs semaphore.**
- Mutex has **ownership** (only the locker can unlock).
- Semaphore has no ownership. Binary semaphore ≈ mutex without ownership (subtle differences).

**Q3: Producer-Consumer (bounded buffer).**
Two semaphores: `empty` and `full`. Mutex protects the buffer.
```c
sem_init(&empty, 0, N);
sem_init(&full, 0, 0);
pthread_mutex_init(&m, NULL);

// producer
add(item);
sem_wait(&empty);
pthread_mutex_lock(&m);
buffer[in] = item;
in = (in + 1) % N;
pthread_mutex_unlock(&m);
sem_post(&full);

// consumer
sem_wait(&full);
pthread_mutex_lock(&m);
item = buffer[out];
out = (out + 1) % N;
pthread_mutex_unlock(&m);
sem_post(&empty);
```

**Q4: Readers-Writers problem.**
Multiple readers OK simultaneously; writers exclusive.
- **Reader-preference** — readers can starve writers.
- **Writer-preference** — writers can starve readers.
- **Fair** — FIFO.

**Q5: Dining Philosophers.**
Classic deadlock example. Solutions:
- **Resource hierarchy** — pick up forks in a fixed order.
- **Arbitrator** — central mutex.
- **Chandy/Misra** — message-passing with fork states.

**Q6: Spinlock vs mutex.**
- Spinlock — busy-wait; use on multi-core for very short waits (< context switch time).
- Mutex — sleeps on contention; better for longer waits.

**Q7: Deadlock with two locks.**
Always acquire locks in the same order. Use `std::lock()` (C++) or `tryLock` with timeout.

**Q8: Lock-free programming?**
Use atomic operations (`std::atomic`, CAS) to avoid locks. Examples: lock-free queues, counters.
**Pros:** no deadlock, scales well.
**Cons:** harder to reason about, ABA problem.

**Q9: ABA problem?**
Value A → B → A; CAS doesn't notice. Fix: versioned pointers (`std::atomic::compare_exchange_weak` with tag).

**Q10: Memory barriers / fences?**
CPU may reorder reads/writes. Memory barriers enforce ordering. Needed for lock-free code.

**Q11: Futex (Linux)?**
Fast Userspace muTEX. Sleeps in kernel only on contention. Building block for pthread mutex.

---

## 5. Inter-Process Communication (IPC)

### Methods

| Method | Description |
|---|---|
| Pipes | Unidirectional; related processes (parent/child) |
| Named pipes (FIFO) | Bidirectional; unrelated processes; filesystem path |
| Message queues | Discrete messages; kernel-mediated |
| Shared memory | Fastest; requires synchronization |
| Sockets | Unix domain (same host) or network (TCP/UDP) |
| Signals | Software interrupts (`SIGTERM`, `SIGKILL`, `SIGUSR1`) |
| Semaphores | Also for sync |
| Memory-mapped files | Same file mapped into multiple address spaces |

### Q&A

**Q1: Fastest IPC?**
Shared memory (no kernel data copy) — but requires sync.

**Q2: When to use what?**
- Same machine, related processes → pipe.
- Unrelated processes → named pipe or message queue.
- Service-to-service → sockets.
- Bulk data, low latency → shared memory.

**Q3: Signal vs signal handler?**
A signal is an asynchronous notification. Handlers run in the interrupted context — only async-signal-safe functions allowed (no `printf`, no `malloc`).

**Q4: UNIX socket vs TCP socket?**
UNIX domain sockets skip the network stack — faster on same host.

**Q5: Zero-copy?**
Transfer data without copying between user and kernel space.
- `sendfile(out_fd, in_fd, ...)` — kernel copies directly.
- `mmap` + `write`.
- `splice` and `tee` in modern Linux.

**Q6: How does shared memory stay consistent?**
Synchronization via semaphores or mutexes. Otherwise: race conditions.

**Q7: Linux namespace-based IPC?**
Containers use PID, network, mount, UTS, IPC, user namespaces. Each container has its own isolated namespace.

---

## 6. Memory Management — Virtual Memory & Paging

### Theory

**Virtual memory** gives each process the illusion of its own large, contiguous address space, backed by physical RAM and disk. Benefits:
- Isolation.
- Efficient use of RAM via paging.
- Allows more memory than physically installed.

### Paging
- Memory divided into fixed-size **pages** (virtual) and **frames** (physical).
- **Page table** maps virtual page → physical frame.
- **TLB** (Translation Lookaside Buffer) caches recent translations.

### Segmentation
- Memory divided by **logical segments** (code, data, stack, heap).
- Variable-size; segment number + offset.

### Multi-level Page Tables
- For 64-bit systems, single-level tables are huge.
- Multi-level (2-, 3-, 4-level) keeps tables sparse.

### Q&A

**Q1: Why virtual memory?**
- Process isolation.
- Programs can use more memory than physical RAM.
- Simplifies memory allocation (contiguous in virtual, sparse in physical).
- Enables copy-on-write `fork`.

**Q2: Page fault?**
CPU references a page not in RAM. OS handles the fault:
1. Find a free frame (or evict).
2. Read from disk.
3. Update page table.
4. Restart the instruction.

**Q3: What is the TLB?**
A hardware cache of recent virtual→physical translations. Typical: 64-512 entries. TLB miss → walk page table (slow) → refill TLB.

**Q4: TLB reach?**
`TLB entries × page size` = amount of memory accessible without TLB miss. With 64 entries and 4 KB pages = 256 KB. Hint: large pages (2 MB, 1 GB) extend reach.

**Q5: Demand paging?**
Pages are loaded only when first accessed. Reduces startup time and memory usage.

**Q6: Copy-on-write (COW)?**
`fork()` shares physical pages between parent and child; only copy when one writes. Saves time and memory.

**Q7: Working set?**
The set of pages a process actively uses. If working set > RAM, **thrashing** occurs.

**Q8: Thrashing?**
System spends more time swapping pages than executing. Mitigation: reduce multiprogramming, better replacement, more RAM.

**Q9: What is a swap partition?**
Disk space used as overflow for pages evicted from RAM. Slow (SSD vs RAM).

**Q10: Paging vs segmentation?**
| Paging | Segmentation |
|---|---|
| Fixed-size | Variable-size |
| No external fragmentation | External fragmentation |
| Invisible to programmer | Visible (segment # matters) |
| Modern OS standard | Legacy (x86 supports it) |

**Q11: Huge pages?**
Larger pages (2 MB / 1 GB) reduce TLB misses for large-memory workloads (databases). Linux: `hugepages` / `THP`.

---

## 7. Page Replacement Algorithms

### Algorithms

| Algorithm | Approach |
|---|---|
| FIFO | Replace oldest |
| LRU | Replace least recently used |
| Optimal | Replace not used for longest in future |
| Clock (Second Chance) | Approx LRU using reference bit |
| NFU / Aging | Aging counter; approximates LRU |
| Working set | Keep recently used pages |
| WSClock | Clock + working set |

### Q&A

**Q1: Belady's anomaly?**
FIFO can have more page faults with more frames. LRU and Optimal don't suffer from this. (Stack property.)

**Q2: LRU implementation issues?**
Need timestamp on every access → expensive. In practice: approximation (clock / aging).

**Q3: Clock algorithm.**
Pages arranged in a circular list. Each has a reference bit:
- Sweep pointer advances.
- If bit = 1 → set to 0, skip.
- If bit = 0 → replace this page.
Approximates LRU cheaply.

**Q4: Why is Optimal impractical?**
Requires future knowledge. Used as upper bound for comparison.

**Q5: Working set model.**
The set of pages referenced in the last N references. If working set doesn't fit in RAM → thrashing.

**Q6: Prepaging vs demand paging?**
- **Demand** — load on first reference. Saves I/O.
- **Prepaging** — load ahead of time. Better when locality is good.

**Q7: Dirty bit (modified bit)?**
Indicates a page has been modified. Evicting a clean page is free (no write-back). Evicting a dirty page requires writing to disk.

**Q8: Page size trade-offs.**
- Small pages → less internal fragmentation, larger page tables.
- Large pages → fewer TLB misses, more internal fragmentation.

---

## 8. File Systems

### Structure (typical UNIX)
- **Boot block** — bootstrap.
- **Superblock** — file system metadata.
- **Inode table** — file metadata (per file).
- **Data blocks** — actual content.

### Allocation methods

| Method | Pros | Cons |
|---|---|---|
| Contiguous | Fast sequential; simple | External fragmentation |
| Linked | No external frag | Slow random access |
| Indexed | Random access; no external frag | Large inode; multiple levels |
| Multi-level indexed | Scalable | More indirection |

### Directory structures
- Single-level.
- Two-level (per-user).
- Hierarchical (tree) — modern.

### Free space management
- **Bitmap** — one bit per block.
- **Linked list** — pointer to next free block.
- **Counting** — run of N free blocks.

### Journaling
- Log intent before applying changes.
- On crash, replay log → recover to consistent state.
- ext4, NTFS, XFS use it.

### Q&A

**Q1: Inode?**
Data structure per file: permissions, timestamps, size, pointers to data blocks. Doesn't store filename (stored in directory entry).

**Q2: Soft link vs hard link.**
- **Soft (symbolic)** — pointer to a filename; can break.
- **Hard** — another name for the same inode; persists as long as any link exists.

**Q3: FAT vs inode?**
FAT chains data blocks via a table (no inode). Used in small/embedded systems. inode is more scalable and faster for metadata ops.

**Q4: Journaling file systems?**
Log transactions to a journal; commit to disk. On crash, replay log → consistent state. ext4, NTFS, XFS.

**Q5: ext4 features.**
Extent-based allocation, journaling, delayed allocation, multiblock allocator, large file support.

**Q6: Copy-on-write file systems?**
ZFS, Btrfs. Never overwrite blocks; write new location. Cheap snapshots, integrity checksums.

**Q7: File descriptor?**
Integer handle to an open file. `0` stdin, `1` stdout, `2` stderr. Created by `open()`, `pipe()`, `socket()`.

**Q8: File descriptor leak?**
Process opens files without closing → FD table fills up → `Too many open files` errors. Fix: ensure close, use RAII wrappers in C++.

**Q9: fsync()?**
Force write to disk. Without `fsync`, data may be lost on crash even after a successful `write()`.

**Q10: Why are unlink + create not atomic on POSIX?**
Two syscalls — crash between them can leave a half-state. Solution: `rename()` is atomic and replaces target.

**Q11: Difference between `mmap` and `read`?**
- `read` — kernel copies file bytes to user buffer.
- `mmap` — kernel maps file pages into your address space; access goes through page cache; no copy. Saves memcpy and disk I/O for repeated access.

---

## 9. I/O Scheduling & Disk Management

### Disk geometry
- Platter, track, sector, cylinder.
- Disk arm moves to the cylinder (seek time) — biggest cost.
- Rotational latency.
- Transfer time.

### I/O scheduling

| Algorithm | Approach |
|---|---|
| FCFS | As-is |
| SSTF | Shortest seek first |
| SCAN (elevator) | Move in one direction, reverse |
| C-SCAN | Move in one direction, jump back |
| LOOK / C-LOOK | Same but no full sweep |
| Deadline | Combine seek + per-request deadline |
| CFQ / BFQ | Per-process fairness |

### SSD characteristics
- No moving parts → no seek time.
- Pages must be erased before rewrite.
- Wear leveling.
- TRIM command informs SSD of unused blocks.

### Q&A

**Q1: Disk access time formula.**
`T = T_seek + T_rotational + T_transfer`
- Seek time: 5-15 ms typical HDD.
- Rotational latency: half of one rotation.
- Transfer: small for a single sector.

**Q2: SCAN vs C-SCAN?**
- **SCAN** — goes back and forth; outer/inner tracks may wait long.
- **C-SCAN** — circular; consistent wait time.

**Q3: RAID levels.**
- **0** — striping, no redundancy.
- **1** — mirroring.
- **5** — striping + 1 parity disk.
- **6** — striping + 2 parity.
- **10** — mirrored stripes.

**Q4: SSD vs HDD for databases?**
SSDs win on random I/O (no seek). HDDs win on $/GB for bulk storage.

**Q5: IOPS?**
I/O operations per second. SSD: 50K-500K IOPS. HDD: 100-200 IOPS.

**Q6: Why does the OS cache writes?**
Reduces perceived latency; coalesces writes; protects against disk slowness. Risk: data loss on crash unless fsync.

**Q7: Direct I/O (O_DIRECT)?**
Bypass the OS page cache. Used by databases that manage their own caching.

**Q8: `fsync` vs `fdatasync`?**
`fsync` flushes data + metadata. `fdatasync` flushes only data (faster).

---

## 10. Kernel, User Mode, System Calls

### Privilege levels
- **Kernel mode** — full hardware access.
- **User mode** — restricted; syscalls cross the boundary.

### System calls
Standard interface for user programs to request OS services.

| Category | Examples |
|---|---|
| Process | `fork`, `exec`, `wait`, `exit`, `kill` |
| File | `open`, `read`, `write`, `close`, `stat`, `mmap` |
| Memory | `brk`, `mmap`, `munmap`, `mprotect` |
| Network | `socket`, `bind`, `listen`, `accept`, `send` |
| Time | `gettimeofday`, `nanosleep`, `clock_gettime` |
| Signal | `sigaction`, `sigprocmask`, `kill` |

### Interrupts
- **Hardware interrupts** — devices signal CPU.
- **Software interrupts** — exceptions (e.g., divide by zero), system calls (via `syscall` instruction).
- **Interrupt handler** runs in kernel; can preempt current process.

### Q&A

**Q1: User mode → kernel mode?**
- System call (explicit).
- Interrupt (hardware).
- Exception (fault).

**Q2: Monolithic vs microkernel?**
- **Monolithic** (Linux, Windows) — entire OS in kernel. Fast, harder to maintain.
- **Microkernel** (QNX, Minix) — minimal kernel; drivers/servers in user mode. Robust, slower IPC.

**Q3: What is a syscall overhead?**
Mode switch (~100 ns) + parameter copy + kernel dispatch. Mitigation: `vDSO` for trivial calls like `gettimeofday`.

**Q4: `fork` vs `exec`?**
- `fork` — duplicates current process (COW).
- `exec` — replaces current process image with new program.
Often combined: `fork` then `exec` in child.

**Q5: Bootstrap?**
BIOS/UEFI loads bootloader → bootloader loads kernel → kernel initializes hardware → mounts root → starts `init`.

**Q6: What is `init`?**
PID 1. The first user-mode process. Linux: `systemd` (most modern distros), `sysvinit`, or `runit`.

**Q7: `setuid` programs?**
Run with the file owner's privileges. Used for `passwd`, `sudo`. Security risk if misconfigured.

---

## 11. Virtualization & Containers

### Virtualization (hypervisors)
- **Type 1 (bare-metal)** — ESXi, Hyper-V, KVM.
- **Type 2 (hosted)** — VirtualBox, VMware Workstation.

### Containers
- Share host kernel; isolated via **namespaces** and **cgroups**.
- Lightweight: start in ms; no separate OS.
- Examples: Docker, containerd, Podman.

### Namespaces
| Namespace | Isolates |
|---|---|
| PID | Process IDs |
| Network | Network stack, IPs, ports |
| Mount | Filesystem mount points |
| UTS | Hostname, domain |
| IPC | Inter-process communication |
| User | User and group IDs |
| Cgroup | Resource limits (CPU, mem, blkio) |

### Q&A

**Q1: VM vs container.**
| | VM | Container |
|---|---|---|
| Boot | Seconds | Milliseconds |
| Size | GB | MB |
| Kernel | Own | Shares host |
| Isolation | Strong | Process-level |
| Density | 10s per host | 1000s |

**Q2: Why are containers faster?**
No separate kernel; less context switching; faster I/O (no virtualization overhead).

**Q3: What is cgroups?**
Limits resources (CPU, memory, blkio) per group of processes. Foundation of container resource management.

**Q4: What is overlayfs?**
Union mount — combines multiple layers into one. Used by Docker images.

**Q5: Why is the OCI standard important?**
Open Container Initiative defines image format and runtime. Docker, containerd, Podman, CRI-O all compatible.

**Q6: Container security?**
- Run as non-root.
- Read-only file systems.
- Drop capabilities.
- Seccomp / AppArmor / SELinux profiles.
- Scan images for CVEs.

**Q7: Why not use VMs for everything?**
Cost, density, boot time, image size. Containers are better for microservices.

---

## 12. Security & Protection

### Concepts
- **Authentication** — verify identity.
- **Authorization** — what you can do.
- **Confidentiality** — only authorized see data.
- **Integrity** — data not tampered.
- **Non-repudiation** — sender can't deny.

### Access control models
- **DAC** (Discretionary) — owner decides.
- **MAC** (Mandatory) — system-enforced (SELinux).
- **RBAC** — role-based.
- **ABAC** — attribute-based.

### Threats
- Malware, rootkits, trojans, worms.
- Buffer overflow, format string, race conditions.
- Side-channel (Spectre, Meltdown).
- Privilege escalation.

### Q&A

**Q1: Buffer overflow prevention?**
Stack canaries, ASLR, DEP/NX bit, safe languages (Rust), bounds-checked APIs.

**Q2: ASLR?**
Address Space Layout Randomization — randomizes code/library/stack addresses. Makes ROP attacks harder.

**Q3: Setuid bit?**
File mode bit that runs a program with the owner's privileges. Used for `passwd`, `ping`, `sudo`. Easy to misuse.

**Q4: TOCTOU?**
Time-of-check to time-of-use race condition. Example: check permission, then act. Between check and use, attacker changes the file.

**Q5: Defense in depth?**
Multiple layers: firewall, intrusion detection, sandboxing, least privilege, audit logging, encryption.

**Q6: Capabilities (Linux)?**
Fine-grained root privileges (e.g., `CAP_NET_BIND_SERVICE`). Container runtimes drop most.

**Q7: Seccomp?**
Filters syscalls a process can make. Used heavily in containers and browsers.

---

## 13. Quick Reference Card

| Concept | One-liner |
|---|---|
| Process | Independent execution; own address space |
| Thread | Lightweight; shares heap |
| Coroutine | Cooperative; user-mode |
| Context switch | Save/restore thread state |
| FCFS / SJF / RR | Scheduling algorithms |
| MLFQ | Adaptive multi-level queue |
| Deadlock | All 4 Coffman conditions |
| Banker's | Avoid unsafe states |
| Critical section | Shared resource access |
| Mutex | Lock with ownership |
| Semaphore | Counter-based signaling |
| Monitor | High-level mutex + cond var |
| Spinlock | Busy-wait mutex |
| Virtual memory | Indirection via pages |
| Page table | Virtual → physical |
| TLB | Cache of recent translations |
| Page fault | Reference to non-resident page |
| LRU / FIFO / Clock | Page replacement |
| Thrashing | Paging dominates CPU |
| Inode | File metadata |
| COW | Share until first write |
| fsync | Force write to disk |
| mmap | Map file into address space |
| Seek time | Disk arm movement |
| IOPS | I/O ops per second |
| RAID | Redundant disk arrays |
| System call | User → kernel interface |
| Kernel mode | Full hardware access |
| Namespaces | Container isolation |
| cgroups | Container resource limits |
| OCI | Container image standard |

---

Good luck with your OS interviews!
