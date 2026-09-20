# C++ Interview Mastery — The Complete Preparation Repository

> A **public, comprehensive, and battle-tested** interview preparation repository for Software Engineer roles. Every topic includes **theory, intuition, optimized C++ code, complexity analysis, and common interview follow-ups**.

Whether you're targeting **FAANG**, top product companies, service-based companies, or fast-growing startups — this repo is designed to be your **single source of truth**.

---

## What's Inside

### `01-dsa-topics/` — Data Structures & Algorithms (C++)
Every major DSA topic with theory + multiple problems (Easy / Medium / Hard).

| # | Topic | LeetCode Tag |
|---|-------|--------------|
| 01 | Arrays & Hashing | `array`, `hash-table` |
| 02 | Two Pointers | `two-pointers` |
| 03 | Sliding Window | `sliding-window` |
| 04 | Prefix Sum | `prefix-sum` |
| 05 | Binary Search | `binary-search` |
| 06 | Linked List | `linked-list` |
| 07 | Stacks & Queues | `stack`, `queue` |
| 08 | Trees | `tree` |
| 09 | BST | `binary-search-tree` |
| 10 | Graphs | `graph`, `bfs`, `dfs`, `union-find` |
| 11 | Dynamic Programming | `dynamic-programming` |
| 12 | Backtracking | `backtracking` |
| 13 | Greedy | `greedy` |
| 14 | Heap / Priority Queue | `heap`, `priority-queue` |
| 15 | Trie | `trie` |
| 16 | Bit Manipulation | `bit-manipulation` |
| 17 | Math & Number Theory | `math` |
| 18 | String Algorithms | `string`, `kmp`, `rabin-karp`, `z-algorithm` |

### `02-cs-fundamentals/` — Core CS Theory
- **Operating Systems** — Scheduling, Deadlock, Memory Management, Synchronization
- **DBMS & SQL** — ACID, Normalization, Indexing, Transactions, Query Patterns
- **Computer Networks** — OSI/TCP-IP, HTTP, TCP handshake, Load Balancing
- **OOP Concepts** — 4 Pillars, SOLID, Composition vs Inheritance
- **C++ Specifics** — Smart Pointers, Move Semantics, Virtual Functions, RAII, STL
- **Design Patterns** — Singleton, Factory, Observer, Strategy, Decorator, etc.

### `03-system-design/` — System Design
- **Fundamentals** — Scalability, Caching, Sharding, CAP Theorem, Consistent Hashing
- **Classic Problems** — URL Shortener, Twitter, YouTube, Uber, WhatsApp, Dropbox, Rate Limiter, etc.

### `04-cheatsheets/` — Quick Revision
- Time & Space Complexity Cheatsheet
- STL Cheatsheet
- C++ Quick Reference
- Pattern Recognition Guide

### `05-resources/` — External Resources
- Curated lists of books, YouTube channels, websites

### `06-frontend-interview/` — Frontend Interview Prep
- HTML, CSS, JavaScript, TypeScript, React (hooks, state, routing, performance)
- Testing, accessibility, build tools, networking, browser storage, security, web performance
- Coding challenges and behavioral / system design questions

### `07-backend-interview/` — Backend Interview Prep
- REST, JWT, SQL/NoSQL, caching, message queues, rate limiting
- Node.js + Express (event loop, async, middleware, testing)
- Java + Spring Boot (JPA, AOP, security, microservices)
- Python + Django / FastAPI (ORM, DRF, Pydantic, async)
- Backend coding challenges and system design questions

---

## How To Use This Repo

1. **Topic-wise preparation** → Pick a topic from `01-dsa-topics/`, read `theory/README.md`, then solve the `problems/` in order.
2. **Quick revision** → Use `04-cheatsheets/` before interviews.
3. **System Design** → Read `03-system-design/01-fundamentals/` first, then practice classic problems.
4. **Daily practice** → Solve 2-3 problems consistently for 8-12 weeks.

---

## How To Compile

Each `.cpp` file is **standalone** and self-contained. To compile and run:

```bash
# Compile a single problem
g++ -std=c++17 -O2 -Wall problem.cpp -o problem

# Run it
./problem

# Or compile & run in one line
g++ -std=c++17 -O2 -Wall problem.cpp -o problem && ./problem
```

> **Recommendation:** Use `-std=c++17` or `-std=c++20`. Modern C++ features (smart pointers, `auto`, range-based for, structured bindings) are used throughout.

---

## Contributing

This is a **public repo**. PRs welcome — fix typos, add problems, improve explanations. See `CONTRIBUTING.md`.

---

## Coverage Targets

- **150+** curated LeetCode-style problems solved
- **18** DSA topics fully covered
- **7** CS fundamentals modules with interview Q&A
- **18** classic system design problems
- **25+** design patterns with C++ examples

---

## License

MIT — use freely, share widely, attribute kindly.

---

> *"The expert in anything was once a beginner."* — Keep grinding! 
