# Linked List — Theory

## What Is a Linked List?

A **linked list** is a linear data structure where elements (nodes) are stored at arbitrary memory locations and connected via pointers.

```
[head] -> [1|next] -> [2|next] -> [3|next] -> [4|null]
```

### Singly Linked List Node
```cpp
struct ListNode {
 int val;
 ListNode* next;
 ListNode(int x) : val(x), next(nullptr) {}
};
```

### Doubly Linked List Node
```cpp
struct DListNode {
 int val;
 DListNode* prev, *next;
};
```

## Types
1. **Singly LL** — each node has data + next pointer
2. **Doubly LL** — each node has data + prev + next
3. **Circular LL** — last node points back to head

## Why Linked Lists?
- **O(1) insertion/deletion** at head/tail (with tail pointer)
- No wasted capacity (vs dynamic arrays)
- Easy to grow/shrink
- O(n) random access (no index-based access)
- Extra memory per node (pointer)
- Poor cache locality

## Time Complexity

| Operation | Singly LL | Doubly LL |
|-----------|-----------|-----------|
| Insert at head | O(1) | O(1) |
| Insert at tail | O(1) with tail ptr | O(1) |
| Insert in middle | O(n) to find + O(1) | O(n) + O(1) |
| Delete at head | O(1) | O(1) |
| Delete at tail | O(n) | O(1) with tail ptr |
| Search | O(n) | O(n) |

## Classic Techniques

### 1. Fast & Slow Pointer (Floyd's Tortoise & Hare)
- Two pointers, one moves at 2x speed.
- **Cycle detection:** If they meet, there's a cycle.
- **Find middle:** When fast reaches end, slow is at middle.
- **Find cycle start:** After meeting, reset one to head, move both at 1x; they meet at cycle entry.

### 2. Reverse a Linked List
Three pointers: prev, curr, next.
```
nullptr <- [1] <- [2] <- [3] <- [4]
```

### 3. Dummy Head
Create a `dummy(0)` before head to simplify edge cases (delete head, merge lists, etc.).

### 4. Recursion
Many LL problems have elegant recursive solutions (reverse, merge, etc.).

## Top Problems

| # | Problem | Difficulty |
|---|---------|-----------|
| 1 | Reverse Linked List | Easy |
| 2 | Merge Two Sorted Lists | Easy |
| 3 | Linked List Cycle | Easy |
| 4 | Remove Nth Node From End | Medium |
| 5 | Add Two Numbers | Medium |
| 6 | Copy List with Random Pointer | Medium |
| 7 | LRU Cache | Medium |
| 8 | Reverse Nodes in k-Group | Hard |
| 9 | Merge K Sorted Lists | Hard |
| 10 | Find Intersection of Two Linked Lists | Easy |
