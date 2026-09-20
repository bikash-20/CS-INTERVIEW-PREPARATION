# Time & Space Complexity Cheatsheet

Quick reference for Big-O of common operations across data structures and algorithms.

## Data Structures

### Array (Dynamic, e.g., `std::vector`)

| Operation | Time |
|-----------|------|
| Access by index | O(1) |
| Search (unsorted) | O(n) |
| Search (sorted, binary search) | O(log n) |
| Insert at end | O(1) amortized |
| Insert at middle | O(n) |
| Delete at end | O(1) |
| Delete at middle | O(n) |

### Linked List (Singly)

| Operation | Time |
|-----------|------|
| Access by index | O(n) |
| Search | O(n) |
| Insert at head | O(1) |
| Insert at tail | O(1) with tail ptr, O(n) without |
| Delete at head | O(1) |
| Delete at tail | O(n) |

### Doubly Linked List

| Operation | Time |
|-----------|------|
| Delete at tail | O(1) with tail ptr |
| All others | Same as singly |

### Stack / Queue

| Operation | Time |
|-----------|------|
| Push/Pop/Peek | O(1) |

### Hash Table (`unordered_map` / `unordered_set`)

| Operation | Avg | Worst |
|-----------|-----|-------|
| Insert | O(1) | O(n) |
| Delete | O(1) | O(n) |
| Search | O(1) | O(n) |

### Tree (`std::map` / `std::set` — Red-Black Tree)

| Operation | Time |
|-----------|------|
| Insert | O(log n) |
| Delete | O(log n) |
| Search | O(log n) |
| Min/Max | O(log n) |
| In-order traversal | O(n) |

### Binary Heap (`priority_queue`)

| Operation | Time |
|-----------|------|
| Insert | O(log n) |
| Extract min/max | O(log n) |
| Peek | O(1) |
| Heapify | O(n) |

### Trie

| Operation | Time |
|-----------|------|
| Insert | O(L) |
| Search | O(L) |
| Prefix search | O(L) |

Where L = length of key.

### Graph (Adjacency List)

| Operation | Time |
|-----------|------|
| Add vertex | O(1) |
| Add edge | O(1) |
| Remove vertex | O(V + E) |
| Remove edge | O(E) |
| BFS/DFS | O(V + E) |

### Union-Find (with path compression + union by rank)

| Operation | Time |
|-----------|------|
| Find | O(α(n)) ≈ O(1) |
| Union | O(α(n)) ≈ O(1) |

## Sorting Algorithms

| Algorithm | Time (Best) | Time (Avg) | Time (Worst) | Space | Stable |
|-----------|-------------|------------|--------------|-------|--------|
| Bubble Sort | O(n) | O(n²) | O(n²) | O(1) | Yes |
| Selection Sort | O(n²) | O(n²) | O(n²) | O(1) | No |
| Insertion Sort | O(n) | O(n²) | O(n²) | O(1) | Yes |
| Merge Sort | O(n log n) | O(n log n) | O(n log n) | O(n) | Yes |
| Quick Sort | O(n log n) | O(n log n) | O(n²) | O(log n) | No |
| Heap Sort | O(n log n) | O(n log n) | O(n log n) | O(1) | No |
| Counting Sort | O(n+k) | O(n+k) | O(n+k) | O(k) | Yes |
| Radix Sort | O(nk) | O(nk) | O(nk) | O(n+k) | Yes |

## Searching Algorithms

| Algorithm | Time | Space |
|-----------|------|-------|
| Linear Search | O(n) | O(1) |
| Binary Search | O(log n) | O(1) |
| Jump Search | O(√n) | O(1) |
| Interpolation Search | O(log log n) avg | O(1) |

## Graph Algorithms

| Algorithm | Time | Space |
|-----------|------|-------|
| BFS | O(V + E) | O(V) |
| DFS | O(V + E) | O(V) |
| Dijkstra (min-heap) | O((V + E) log V) | O(V) |
| Bellman-Ford | O(V · E) | O(V) |
| Floyd-Warshall | O(V³) | O(V²) |
| Kruskal MST | O(E log E) | O(V) |
| Prim MST | O(E log V) | O(V) |
| Topological Sort | O(V + E) | O(V) |
| Tarjan's SCC | O(V + E) | O(V) |

## Common Complexities (Quick Lookup)

| Complexity | Name | Example |
|------------|------|---------|
| O(1) | Constant | Hash lookup, array access |
| O(log n) | Logarithmic | Binary search, BST |
| O(√n) | Square root | Trial division |
| O(n) | Linear | Single loop |
| O(n log n) | Linearithmic | Merge sort, heap sort |
| O(n²) | Quadratic | Nested loops, naive DP |
| O(n³) | Cubic | Floyd-Warshall |
| O(2ⁿ) | Exponential | Subsets, naive Fibonacci |
| O(n!) | Factorial | Permutations, brute-force TSP |

## Space Complexity Patterns

- **O(1):** Constant extra space — in-place algorithms.
- **O(log n):** Recursion depth for balanced trees.
- **O(n):** Hash maps, arrays, recursion depth for skewed tree.
- **O(n²):** 2D DP tables.

## Master Theorem (Divide & Conquer)

For `T(n) = aT(n/b) + f(n)`:
- If `f(n) = O(n^(log_b(a) - ε))`: `T(n) = Θ(n^log_b(a))`
- If `f(n) = Θ(n^log_b(a))`: `T(n) = Θ(n^log_b(a) · log n)`
- If `f(n) = Ω(n^(log_b(a) + ε))`: `T(n) = Θ(f(n))`

## Amortized Analysis

Some operations are O(n) worst but O(1) average:
- `vector::push_back` (occasional resize)
- `unordered_map` insertions (occasional rehash)
- Dynamic array append
