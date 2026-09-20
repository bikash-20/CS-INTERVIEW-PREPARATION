# Binary Search — Theory

## What Is Binary Search?

Binary search finds a target in a **sorted** array by repeatedly halving the search space.

**Time:** O(log n) — each comparison eliminates half the elements.

## The Template

The most important thing: pick ONE template and stick with it. Here's the **canonical version**:

```cpp
int binarySearch(vector<int>& arr, int target) {
 int left = 0, right = arr.size() - 1; // [left, right]
 while (left <= right) {
 int mid = left + (right - left) / 2; // avoid overflow
 if (arr[mid] == target) return mid;
 else if (arr[mid] < target) left = mid + 1;
 else right = mid - 1;
 }
 return -1;
}
```

### Why `left + (right - left) / 2` instead of `(left + right) / 2`?
- `(left + right)` can overflow `int` for large values.
- `left + (right - left) / 2` is overflow-safe.

## Variations of Binary Search

### 1. Find First Occurrence (Leftmost)
When duplicates exist, find the leftmost target.

```cpp
int lowerBound(vector<int>& arr, int target) {
 int left = 0, right = arr.size(); // [left, right)
 while (left < right) {
 int mid = left + (right - left) / 2;
 if (arr[mid] < target) left = mid + 1;
 else right = mid;
 }
 return left; // first index where arr[i] >= target
}
```

### 2. Find Last Occurrence (Rightmost)
```cpp
int upperBound(vector<int>& arr, int target) {
 int left = 0, right = arr.size();
 while (left < right) {
 int mid = left + (right - left) / 2;
 if (arr[mid] <= target) left = mid + 1;
 else right = mid;
 }
 return left - 1; // last index where arr[i] <= target
}
```

### 3. Search in Rotated Sorted Array
Array was rotated at some pivot. Find target in O(log n).
- Check which half is sorted, then decide if target is in that half.

```cpp
int searchRotated(vector<int>& arr, int target) {
 int left = 0, right = arr.size() - 1;
 while (left <= right) {
 int mid = left + (right - left) / 2;
 if (arr[mid] == target) return mid;
 if (arr[left] <= arr[mid]) { // left half sorted
 if (target >= arr[left] && target < arr[mid]) right = mid - 1;
 else left = mid + 1;
 } else { // right half sorted
 if (target > arr[mid] && target <= arr[right]) left = mid + 1;
 else right = mid - 1;
 }
 }
 return -1;
}
```

### 4. Binary Search on Answer (Monotonic Search Space)
When the problem asks "find min/max value such that condition holds":
- Define a predicate function `f(x) = true/false`
- Find the boundary where f changes from false to true (or vice versa)
- Use binary search over the value range

**Example:** Koko Eating Bananas — find min eating speed.
- Predicate: "Can Koko finish all bananas at speed k within h hours?"
- Monotonic: if speed k works, speed k+1 also works.
- Binary search k.

## Top Problems

| # | Problem | Variation | Difficulty |
|---|---------|-----------|-----------|
| 1 | Binary Search | Standard | Easy |
| 2 | Search Insert Position | Lower bound | Easy |
| 3 | Find First and Last Position | Both bounds | Medium |
| 4 | Search in Rotated Sorted Array | Rotated | Medium |
| 5 | Find Minimum in Rotated Sorted Array | Rotated | Medium |
| 6 | Search a 2D Matrix | 2D | Medium |
| 7 | Koko Eating Bananas | On answer | Medium |
| 8 | Find Peak Element | Modified | Medium |
| 9 | Split Array Largest Sum | On answer | Hard |
| 10 | Median of Two Sorted Arrays | Modified | Hard |

## Common Pitfalls

- **Infinite loops** — usually from `left = mid` instead of `left = mid + 1`, or wrong loop condition
- **Off-by-one** — decide if your search space is `[left, right]` or `[left, right)`
- **Overflow** — use `left + (right - left) / 2`
- **Empty array** — handle `arr.empty()` separately or check `left <= right`
- **Integer division** — `mid = (left+right)/2` rounds down, which is fine for our logic
