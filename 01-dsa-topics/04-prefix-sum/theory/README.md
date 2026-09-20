# Prefix Sum — Theory

## Definition

A **prefix sum** array stores cumulative sums where each element represents the sum of all elements before it (inclusive).

```
Original: [a₀, a₁, a₂, a₃, a₄, a₅]
Prefix: [0, a₀, a₀+a₁, a₀+a₁+a₂, ..., sum of all]
 ↑
 prefix[i] = arr[0] + arr[1] + ... + arr[i-1]
 = sum of subarray [0, i)
```

**With offset of one (most common):**
```
prefix[0] = 0
prefix[i] = arr[0] + arr[1] + ... + arr[i-1]
```

## Why Prefix Sums?

**Range sum queries in O(1).**

Sum of subarray `[L, R]`:
```
sum(L, R) = prefix[R+1] - prefix[L]
```

Without prefix sum: O(n) per query. With prefix sum: O(1) per query.

## Common Patterns

### 1. Subarray Sum Equals K (LeetCode #560)
Count subarrays with sum == k. Use prefix sum + hash map of (prefix sum → count).

```cpp
int count = 0, prefix = 0;
unordered_map<int, int> seen;
seen[0] = 1; // empty prefix
for (int x : arr) {
 prefix += x;
 count += seen[prefix - k];
 seen[prefix]++;
}
```

### 2. Pivot Index (LeetCode #724)
Find index where left sum == right sum.
- Compute total sum. For each index i, left = prefix[i], right = total - prefix[i] - nums[i].

### 3. Range Sum Query (Immutable) (LeetCode #303)
- Precompute prefix sums. Answer queries in O(1).

### 4. Product of Array Except Self (LC #238)
- Use prefix product + suffix product. (Already covered in Arrays topic.)

### 5. 2D Prefix Sums
For matrix sub-rectangle sum queries.
```
ps[i][j] = sum of all elements in rectangle (0,0) to (i-1, j-1)
sum of rectangle (r1,c1) to (r2,c2) = ps[r2+1][c2+1] - ps[r1][c2+1] - ps[r2+1][c1] + ps[r1][c1]
```

## Top Problems

| # | Problem | Difficulty |
|---|---------|-----------|
| 1 | Range Sum Query - Immutable | Easy |
| 2 | Pivot Index | Easy |
| 3 | Subarray Sum Equals K | Medium |
| 4 | Contiguous Array (equal 0s and 1s) | Medium |
| 5 | Product of Array Except Self | Medium |
| 6 | Number of Submatrices That Sum To Target | Hard |
| 7 | Maximum Size Subarray Sum Equals K | Medium |
| 8 | Continuous Subarray Sum (multiple of K) | Medium |
