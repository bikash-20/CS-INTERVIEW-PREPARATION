# Arrays & Hashing — Theory

## What is an Array?

An array is a **contiguous block of memory** storing elements of the same type, accessible in O(1) by index.

```
Index: 0 1 2 3 4 5 6
Value: [ 5 ][ 12][ 3 ][ 8 ][ 1 ][ 7 ][ 11 ]
Address: 100 104 108 112 116 120 124 (assuming 4 bytes per int)
```

### Key Properties
- **Random access:** `arr[i]` in O(1)
- **Cache-friendly:** contiguous memory → great spatial locality
- **Fixed size** (static arrays) or **dynamic** (C++ `std::vector`)
- **Insertion/Deletion** in middle: O(n) due to shifting
- **Time Complexities:**
 - Access by index: O(1)
 - Search (unsorted): O(n)
 - Search (sorted, binary search): O(log n)
 - Insert at end: O(1) amortized (vector)
 - Insert at middle: O(n)

---

## What is Hashing?

Hashing is a technique to map data of arbitrary size to **fixed-size values** (hashes) using a **hash function**. The output is used as an index into a **hash table**.

```
"hello" → hash function → 42 → bucket[42] stores "hello"
"world" → hash function → 17 → bucket[17] stores "world"
```

### Why Hashing Matters
- **O(1) average** insert, lookup, delete
- Used in `unordered_map`, `unordered_set` in C++ STL
- Powers databases (hash indexes), caches, sets, dictionaries

### Hash Collisions
Two different keys can produce the same hash. Two main strategies:
1. **Chaining** — each bucket holds a linked list
2. **Open Addressing** — probe next available slot (linear/quadratic/double hashing)

### Load Factor & Rehashing
When the table gets too full (`load_factor > 0.7`), we **resize and rehash** → amortized O(1).

### C++ STL Hash Containers

| Container | Underlying | Lookup | Insert |
|-----------|-----------|--------|--------|
| `unordered_set<T>` | hash table | O(1) avg | O(1) avg |
| `unordered_map<K,V>` | hash table | O(1) avg | O(1) avg |
| `unordered_multiset` | allows duplicates | O(1) avg | O(1) avg |
| `unordered_multimap` | allows duplicate keys | O(1) avg | O(1) avg |

---

## Common Patterns in Array/Hashing Problems

### 1. **Frequency Counting**
Use a hash map to count occurrences of elements.
```cpp
unordered_map<int, int> freq;
for (int x : arr) freq[x]++;
```

### 2. **Two-Sum Family**
For "find pair/triplet with target sum", use a hash map for O(n) lookup.
```cpp
// Two Sum — look for complement
for (int i = 0; i < n; i++) {
 int complement = target - arr[i];
 if (seen.count(complement)) return {seen[complement], i};
 seen[arr[i]] = i;
}
```

### 3. **Index Mapping**
Map value → index (or value → list of indices).
```cpp
unordered_map<int, vector<int>> positions;
for (int i = 0; i < n; i++) positions[arr[i]].push_back(i);
```

### 4. **Grouping / Anagrams**
Use sorted string or frequency tuple as the key.
```cpp
// Group anagrams
string key = sortString(s);
groups[key].push_back(s);
```

### 5. **Sliding Window on Frequency**
Track character counts in a window — see Sliding Window topic.

### 6. **Prefix Sum + Hash Map**
For subarray sum problems — see Prefix Sum topic.

---

## Edge Cases To Always Consider

- Empty array (`n == 0`)
- Single element
- All elements the same
- Negative numbers
- Very large / very small values (overflow!)
- Duplicates
- Sorted vs unsorted input

---

## Top Interview Problems on This Topic

| # | Problem | Difficulty | Key Pattern |
|---|---------|-----------|-------------|
| 1 | Two Sum | Easy | Hash map lookup |
| 2 | Best Time to Buy/Sell Stock | Easy | Track minimum |
| 3 | Contains Duplicate | Easy | Hash set |
| 4 | Product of Array Except Self | Medium | Prefix/suffix products |
| 5 | Top K Frequent Elements | Medium | Bucket sort / heap |
| 6 | Group Anagrams | Medium | Sorted key |
| 7 | Longest Consecutive Sequence | Medium | Hash set O(n) |
| 8 | Encode and Decode Strings | Medium | Custom encoding |
| 9 | Subarray Sum Equals K | Medium | Prefix sum + hash map |
| 10 | LRU Cache | Medium | Hash map + doubly LL |

---

## Complexity Quick Reference

| Operation | Unsorted Array | Sorted Array | Hash Table |
|-----------|---------------|-------------|------------|
| Search | O(n) | O(log n) | O(1) avg |
| Insert | O(1)* / O(n) | O(n) | O(1) avg |
| Delete | O(n) | O(n) | O(1) avg |
| Min/Max | O(n) | O(1) | O(n) |

*amortized at end
