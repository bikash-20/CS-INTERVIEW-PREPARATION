# Two Pointers Technique — Theory

The **Two Pointers** technique uses two indices that traverse an array (or two arrays) — typically moving toward each other, in the same direction, or at different speeds. It converts many O(n²) brute-force solutions into O(n).

---

## Common Patterns

### Pattern 1: Opposite Ends (Converging)
Two pointers start at opposite ends and move toward each other.
**Best for:** Sorted arrays, pair-sum problems, palindromes, container with most water.

```
[left → ... ← right]
 1 3 5 7 9
 L R ← sum = 10 (good)
 L R ← sum = 8 (need bigger, move L right)
 L R ← sum = 10 (good)
```

### Pattern 2: Same Direction (Fast & Slow)
Both start at the beginning, one moves faster than the other.
**Best for:** In-place modifications, removing duplicates, detecting cycles (Floyd's).

```
[slow →]
 [fast →→]
0 0 1 1 1 2 2 3
^ ^
S F (slow at 0, fast at 0 — they will move with fast scanning)
```

### Pattern 3: Read/Write Pointers
Two pointers on the same array — one reads, one writes.
**Best for:** Filtering, removing elements in-place.

### Pattern 4: Sliding Window Variant
Two pointers define a window `[left, right]`. Right expands, left contracts.
**Best for:** Substring/subarray problems. (See `03-sliding-window/` for full coverage.)

---

## When To Use Two Pointers

 Array/string is **sorted** (or can be sorted)
 Looking for **pairs/triplets** with a sum condition
 Need to **compare elements from two ends**
 Want to modify array **in-place** with O(1) extra space
 Slow + Fast pointer for **cycle detection** in linked lists

---

## Classic Problems & Approaches

### 1. Two Sum II — Sorted Input
Sorted array, find two numbers that sum to target.
- Left at 0, right at n-1. If sum > target → move right left. If sum < target → move left right.

### 2. 3Sum
Find all unique triplets that sum to zero.
- Sort first. For each `i`, run Two Sum II on `arr[i+1..n-1]`. Skip duplicates.

### 4Sum: Same idea, just one more nested loop.

### 3. Container With Most Water
Two vertical lines, find max area.
- Left and right at ends, move the shorter line inward (greedy proof: the only chance to beat current area is by increasing the shorter height).

### 4. Trapping Rain Water
Compute water trapped between bars.
- **Approach 1:** Precompute max-left and max-right for each index.
- **Approach 2:** Two pointers — leftMax and rightMax maintained as pointers move.

### 5. Valid Palindrome
Skip non-alphanumeric, compare case-insensitive from both ends.

### 6. Remove Duplicates from Sorted Array
Slow pointer = position to write. Fast pointer scans.
- When `nums[fast] != nums[slow]`, advance slow and copy.

### 7. Move Zeroes
Same as above but write `0`s or shift non-zeros forward, then fill rest with 0s.

---

## Complexity Analysis

Most two-pointer solutions are:
- **Time:** O(n) — each pointer moves at most n times
- **Space:** O(1) — no extra data structures (besides output)

---

## Common Pitfalls

- **Off-by-one errors** — be very careful with `left < right` vs `left <= right`
- **Duplicate handling** in 3Sum/4Sum — skip `if (i > 0 && nums[i] == nums[i-1])`
- **Integer overflow** when computing sums — use `long long` for safety
- **Edge cases**: empty array, single element, all duplicates

---

## Top Problems To Practice

| # | Problem | Difficulty | Pattern |
|---|---------|-----------|---------|
| 1 | Two Sum II - Input Array Is Sorted | Easy | Converging |
| 2 | Valid Palindrome | Easy | Converging |
| 3 | Two Sum (Unsorted) | Easy | Hashmap, but doable w/ pointers |
| 4 | Container With Most Water | Medium | Converging |
| 5 | 3Sum | Medium | Sort + Converging |
| 6 | Remove Duplicates from Sorted Array | Easy | Fast/Slow |
| 7 | Move Zeroes | Easy | Read/Write |
| 8 | Trapping Rain Water | Hard | Converging |
| 9 | 4Sum | Medium | Dbl nested + Converging |
| 10 | Sort Colors (Dutch National Flag) | Medium | 3 pointers |
