# Heap / Priority Queue — Theory

## What Is a Heap?

A **heap** is a complete binary tree that satisfies the **heap property**:
- **Min-heap**: parent ≤ children → root is the minimum.
- **Max-heap**: parent ≥ children → root is the maximum.

```
Min-Heap: 1
 / \
 3 2
 /\ /\
 7 6 5 8

Array representation: [1, 3, 2, 7, 6, 5, 8]
 0 1 2 3 4 5 6

For index i:
 - parent = (i-1) / 2
 - left = 2*i + 1
 - right = 2*i + 2
```

## C++ STL

```cpp
#include <queue>
priority_queue<int> maxHeap; // default = max-heap
priority_queue<int, vector<int>, greater<int>> minHeap; // min-heap
```

## When To Use Heap

 "Top K" / "Kth largest/smallest" problems
 Merge K sorted streams
 Running median
 Dijkstra's shortest path
 Huffman encoding
 Scheduling by priority

## Classic Problems

### 1. Kth Largest Element (LeetCode #215)
- Min-heap of size K. For each num, push; if size > K, pop smallest. Top is answer.

### 2. Top K Frequent Elements (LeetCode #347) — covered earlier
- Frequency map + bucket sort OR min-heap of size K.

### 3. Merge K Sorted Lists (LeetCode #23)
- Min-heap by node value. Pop smallest, push next.

### 4. Find Median from Data Stream (LeetCode #295)
- Two heaps: max-heap (lower half) + min-heap (upper half). Median is tops.

### 5. K Closest Points to Origin (LeetCode #973)
- Max-heap of size K (by distance). Maintain K closest.

### 6. Task Scheduler (LeetCode #621)
- Greedy with max-heap.

### 7. Reorganize String (LeetCode #767)
- Place most frequent char first with gaps.

### 8. Last Stone Weight (LeetCode #1046)
- Max-heap. Smash two largest.

### 9. Kth Smallest in Sorted Matrix (LeetCode #378)
- Min-heap from top-left; expand right/down.

### 10. Sliding Window Median (LeetCode #480)
- Two heaps + lazy deletion.

## Top Problems

| # | Problem | Difficulty |
|---|---------|-----------|
| 1 | Kth Largest Element | Medium |
| 2 | Top K Frequent Elements | Medium |
| 3 | Kth Largest in Stream | Easy |
| 4 | Last Stone Weight | Easy |
| 5 | K Closest Points to Origin | Medium |
| 6 | Merge K Sorted Lists | Hard |
| 7 | Find Median from Data Stream | Hard |
| 8 | Task Scheduler | Medium |
| 9 | Reorganize String | Medium |
| 10 | Sliding Window Median | Hard |

## Complexity

| Operation | Time |
|-----------|------|
| Insert | O(log n) |
| Extract min/max | O(log n) |
| Peek min/max | O(1) |
| Heapify | O(n) |
