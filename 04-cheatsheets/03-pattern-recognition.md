# Pattern Recognition Guide

When you see a problem, look for **keywords and structure** to identify the right algorithm/pattern.

---

## "Find pair / triplet / subarray with sum X"

| Conditions | Pattern |
|------------|---------|
| Sorted input | Two Pointers |
| Unsorted input | Hash Map |
| Subarray (contiguous) | Sliding Window or Prefix Sum + Hash Map |

---

## "Longest / Shortest subarray/substring with condition"

→ **Sliding Window**
- "At most K distinct" → expand/shrink
- "Exactly K" → atMost(K) - atMost(K-1) trick

---

## "Min/Max cost / path / steps"

→ **Dynamic Programming** or **Dijkstra** (if weighted graph)

---

## "Count number of ways"

→ **DP** (top-down with memo, or bottom-up)
- For combinatorics: `nCr` with Pascal's triangle

---

## "Detect cycle / find middle / palindrome LL"

→ **Fast & Slow Pointers** (Floyd's)

---

## "Find Kth largest / smallest / Top K"

| Approach | When |
|----------|------|
| Min/Max Heap of size K | Generic, O(n log k) |
| Quickselect | O(n) avg, O(n²) worst |
| Counting sort / bucket | When range is bounded |
| Sort | O(n log n), simple |

---

## "Sorted input / find target"

→ **Binary Search** (or variant: lower/upper bound)

---

## "Connected components / merging / cycle in graph"

→ **Union-Find** (DSU)

---

## "Shortest path / min cost"

| Graph Type | Algorithm |
|------------|-----------|
| Unweighted | BFS |
| Weighted, non-negative | Dijkstra |
| Weighted with negatives | Bellman-Ford |
| All pairs | Floyd-Warshall |

---

## "Generate all combinations/permutations/subsets"

→ **Backtracking**

---

## "Tree traversal / depth / ancestor"

→ **DFS** (recursive or stack)

---

## "Level-order / shortest in unweighted graph"

→ **BFS** (queue)

---

## "Frequency / majority / mode"

| Need | Approach |
|------|----------|
| Top K | Heap or bucket sort |
| Most frequent (K=1) | Boyer-Moore Voting |
| Anagrams | Sort or count as key |

---

## "Range sum query, multiple queries"

→ **Prefix Sum** (or segment tree for updates)

---

## "Interval scheduling / merge intervals"

| Problem | Sort By |
|---------|---------|
| Maximum non-overlapping intervals | End time asc |
| Merge overlapping | Start time asc |
| Min arrows / intervals to remove | End time asc |

---

## "Min/Max of all subarrays of size K"

→ **Sliding Window** (often with deque for max)

---

## "Build string / sequence step by step"

→ **Stack** for matching (parentheses), or **Greedy** for constructing

---

## "Design add/get/contains with O(1)"

→ **Hash Table** or **Hash + Linked List** (LRU)

---

## "Matrix/grid traversal"

| Direction | Approach |
|-----------|----------|
| 4-directional | BFS/DFS |
| Shortest path (all cells traversable) | BFS |
| All paths / unique paths | DP |

---

## "Looks like string matching"

| Pattern | Algorithm |
|---------|-----------|
| Naive | O(nm) brute force |
| Substring search | KMP, Z-algo, Rabin-Karp |
| Anagrams | Sort/freq |
| Longest palindrome | Expand-around-center / Manacher |

---

## Quick Decision Tree

```
Is it about a subarray / substring?
 YES → Sliding Window or Prefix Sum
 NO ↓

Is it about a tree?
 YES → DFS/BFS depending on need
 NO ↓

Is it about a graph?
 YES → BFS/DFS/Dijkstra/Union-Find
 NO ↓

Is it about combinations/sequences?
 YES → Backtracking
 NO ↓

Is there an optimization (min/max/count)?
 YES → DP or Greedy
 NO ↓

Is the array sorted or sorted-able?
 YES → Two Pointers or Binary Search
 NO ↓

Default: Hash Map or Sort
```
