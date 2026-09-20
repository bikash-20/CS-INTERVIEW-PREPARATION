# Sliding Window — Theory

The **Sliding Window** technique maintains a "window" over a contiguous portion of an array or string and slides it to find an optimal subarray/substring. It transforms many O(n²) or O(n³) brute-force solutions into **O(n)**.

---

## Anatomy of a Window

```
Window = subarray from index `left` to `right` (inclusive)
 = [ left, left+1, ..., right ]

We usually maintain:
 - Sum / count of elements in the window
 - A data structure (e.g., hashmap) tracking what's inside
 - left/right pointers that only move forward (never backward)
```

```
Array: [ 2 5 1 8 3 7 4 ]

Window of size 3:

Step 1: [ 2 5 1 ] 8 3 7 4 sum=8
Step 2: [ 5 1 8 ] 3 7 4 sum=14 (remove 2, add 8)
Step 3: [ 1 8 3 ] 7 4 sum=12 (remove 5, add 3)
Step 4: [ 8 3 7 ] 4 sum=18 (remove 1, add 7)
Step 5: [ 3 7 4 ] sum=14 (remove 8, add 4)
```

---

## Two Types of Sliding Window

### Type 1: Fixed-Size Window
Window size `k` is constant. Slide by 1 each step.
- Compute initial window
- For each step: subtract leaving element, add entering element

**Examples:** Max sum subarray of size K, Average of subarrays of size K.

### Type 2: Variable-Size Window (Dynamic)
Window grows/shrinks to satisfy a condition.
- Expand `right` until condition violated
- Shrink `left` until condition restored
- Track the optimal answer during the process

**Examples:** Longest substring without repeating chars, Smallest subarray with sum ≥ target, Minimum window substring.

---

## Classic Templates

### Template 1: Fixed-Size Window

```cpp
int fixedWindow(vector<int>& arr, int k) {
 int sum = 0;
 // Initial window
 for (int i = 0; i < k; i++) sum += arr[i];

 int best = sum;
 // Slide
 for (int i = k; i < (int)arr.size(); i++) {
 sum += arr[i] - arr[i - k];
 best = max(best, sum);
 }
 return best;
}
```

### Template 2: Variable Window — Longest

```cpp
int longestWindow(string s) {
 unordered_map<char, int> freq;
 int left = 0, best = 0;
 for (int right = 0; right < (int)s.size(); right++) {
 freq[s[right]]++;

 while (/* window invalid */) {
 freq[s[left]]--;
 left++;
 }
 best = max(best, right - left + 1);
 }
 return best;
}
```

### Template 3: Variable Window — Shortest

```cpp
int shortestWindow(vector<int>& arr, int target) {
 int left = 0, sum = 0, best = INT_MAX;
 for (int right = 0; right < (int)arr.size(); right++) {
 sum += arr[right];
 while (sum >= target) {
 best = min(best, right - left + 1);
 sum -= arr[left++];
 }
 }
 return best == INT_MAX ? 0 : best;
}
```

---

## When To Use Sliding Window

 Problem asks for **subarray / substring / contiguous sequence**
 You see keywords: **longest, shortest, maximum, minimum, contains, at most, at least**
 Brute force would be **O(n²)** or worse
 Elements are positive (for sum-based shrinking)

---

## Top Problems To Practice

| # | Problem | Type | Difficulty |
|---|---------|------|-----------|
| 1 | Maximum Sum Subarray of Size K | Fixed | Easy |
| 2 | Longest Substring Without Repeating Chars | Variable (longest) | Medium |
| 3 | Minimum Size Subarray Sum | Variable (shortest) | Medium |
| 4 | Minimum Window Substring | Variable (shortest) | Hard |
| 5 | Permutation in String | Fixed (freq) | Medium |
| 6 | Find All Anagrams in a String | Fixed (freq) | Medium |
| 7 | Sliding Window Maximum | Fixed (deque) | Hard |
| 8 | Longest Repeating Character Replacement | Variable | Medium |
| 9 | Subarrays with K Different Integers | Variable | Hard |
| 10 | Fruit Into Baskets | Variable | Medium |

---

## Common Pitfalls

- **Empty window** — make sure the window can shrink all the way to size 0 if needed
- **Update answer** at the right moment — inside loop or inside while?
 - For LONGEST: update **after** shrinking
 - For SHORTEST: update **inside** the while loop (when window is valid)
- **Hashmap size** vs **distinct count** — be precise
- Off-by-one: window of size `k` uses indices `[i, i+k-1]`

---

## Complexity

Almost all sliding window problems:
- **Time:** O(n) — each element enters and leaves the window at most once
- **Space:** O(1) to O(k) depending on the auxiliary data structure
