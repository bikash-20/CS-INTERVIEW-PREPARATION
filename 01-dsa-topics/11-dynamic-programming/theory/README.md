# Dynamic Programming — Theory

## What Is DP?

Dynamic Programming is an optimization technique that solves complex problems by:
1. **Breaking them into overlapping subproblems**
2. **Storing the results** of subproblems to avoid recomputation

## When To Use DP

 The problem has **optimal substructure**: optimal solution can be built from optimal solutions to subproblems.
 The problem has **overlapping subproblems**: same subproblems are solved repeatedly.
 Common keywords: "count the number of ways", "min/max cost", "is it possible", "in how many ways".

## Two Approaches

### 1. Top-Down (Memoization)
- Start with the original problem.
- Recursively break it down.
- Cache results in a hashmap/array.

### 2. Bottom-Up (Tabulation)
- Start with base cases.
- Iteratively build up to the answer.
- Usually more space-efficient (can drop dimensions).

## Classic Patterns

### 1. Fibonacci / Climbing Stairs
```cpp
// dp[i] = dp[i-1] + dp[i-2]
dp[0] = dp[1] = 1;
for (int i = 2; i <= n; i++) dp[i] = dp[i-1] + dp[i-2];
```

### 2. 0/1 Knapsack
For each item i and capacity w:
```
dp[i][w] = max(dp[i-1][w], // skip item i
 dp[i-1][w-weight[i]] + value[i]) // take item i
```

### 3. Unbounded Knapsack / Coin Change
Item can be reused. Difference: the recurrence uses `dp[i]` instead of `dp[i-1]` for "take".

### 4. Longest Common Subsequence (LCS)
```
if (a[i-1] == b[j-1]) dp[i][j] = dp[i-1][j-1] + 1;
else dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
```

### 5. Longest Increasing Subsequence (LIS)
- O(n²): `dp[i] = max(dp[j] + 1) for all j < i where a[j] < a[i]`
- O(n log n): patience sorting with `lower_bound`.

### 6. Edit Distance (Levenshtein)
```
if (a[i-1] == b[j-1]) dp[i][j] = dp[i-1][j-1];
else dp[i][j] = 1 + min(dp[i-1][j], // delete
 dp[i][j-1], // insert
 dp[i-1][j-1]); // replace
```

### 7. Matrix Chain Multiplication / Burst Balloons
- Interval DP. `dp[i][j]` = best for range [i..j].

### 8. DP on Grids
- Paths in grid. `dp[i][j] = dp[i-1][j] + dp[i][j-1]`.
- Variants: minimum path sum, unique paths with obstacles.

### 9. DP on Strings
- Palindromic substrings, longest palindromic subsequence, regex matching, word break.

### 10. State Machine DP
- Stock trading problems (with cooldown, with fee, with k transactions).

## Top Problems

| # | Problem | Difficulty | Pattern |
|---|---------|-----------|---------|
| 1 | Climbing Stairs | Easy | Fibonacci |
| 2 | House Robber | Medium | 1D DP |
| 3 | Coin Change | Medium | Unbounded knapsack |
| 4 | Longest Increasing Subsequence | Medium | LIS |
| 5 | Longest Common Subsequence | Medium | 2D strings |
| 6 | Word Break | Medium | 1D string |
| 7 | Unique Paths | Medium | Grid |
| 8 | Jump Game | Medium | Greedy/DP |
| 9 | Partition Equal Subset Sum | Medium | 0/1 knapsack |
| 10 | Edit Distance | Medium | 2D strings |
| 11 | Burst Balloons | Hard | Interval DP |
| 12 | Best Time to Buy/Sell Stock IV | Hard | State machine |

## Time-Space Optimization Tricks

- **Rolling array**: Use only 2 rows of a 2D DP table → O(n) space.
- **1D DP from 2D**: Sometimes you can flatten the state.
- **In-place modification**: Use the input array itself for the DP table.

## DP Problem-Solving Framework

1. Identify the **state** (what's the dp[i] / dp[i][j]?)
2. Define the **recurrence** (how does state relate to smaller states?)
3. Set the **base cases**.
4. Decide iteration order.
5. Optimize space if needed.
