# Backtracking — Theory

## What Is Backtracking?

A refined form of **brute force**. We build solutions incrementally and **abandon** a path ("backtrack") as soon as we determine it cannot lead to a valid solution.

## Template

```cpp
void backtrack(State& state, vector<int>& choices, vector<vector<int>>& result) {
 if (/* goal reached */) {
 result.push_back(state.path);
 return;
 }
 for (int choice : choices) {
 if (/* valid choice */) {
 state.make(choice); // choose
 backtrack(updatedState, ..., result); // explore
 state.unmake(choice); // un-choose (BACKTRACK)
 }
 }
}
```

The three steps: **CHOOSE → EXPLORE → UNCHOOSE**.

## Classic Problems

### 1. Subsets (LeetCode #78)
- For each element, decide to include or exclude.
- 2^n subsets total.

### 2. Permutations (LeetCode #46)
- Place each element in each position. Track which are used.
- n! permutations.

### 3. Combinations (LeetCode #77)
- Pick k elements from n. Use `start` index to avoid duplicates.

### 4. Combination Sum (LeetCode #39)
- Numbers can be reused. Sort first, skip duplicates.

### 5. N-Queens (LeetCode #51)
- Place n queens such that none attack each other.
- Backtrack on row; for each column, check if safe (no attack from above rows).

### 6. Sudoku Solver (LeetCode #37)
- For each empty cell, try digits 1-9; backtrack if invalid.

### 7. Word Search (LeetCode #79)
- DFS from each cell; mark visited.

### 8. Generate Parentheses (LeetCode #22)
- Generate all valid parentheses of length 2n.
- Track `open` and `close` counts.

### 9. Palindrome Partitioning (LeetCode #131)
- Cut string into palindromic substrings.

### 10. Letter Combinations of Phone Number (LeetCode #17)
- Recursive: for each digit, try each letter.

## Top Problems

| # | Problem | Difficulty |
|---|---------|-----------|
| 1 | Subsets | Medium |
| 2 | Subsets II (with duplicates) | Medium |
| 3 | Permutations | Medium |
| 4 | Permutations II | Medium |
| 5 | Combination Sum | Medium |
| 6 | Combination Sum II | Medium |
| 7 | Generate Parentheses | Medium |
| 8 | N-Queens | Hard |
| 9 | Sudoku Solver | Hard |
| 10 | Word Search | Medium |
| 11 | Palindrome Partitioning | Medium |
| 12 | Letter Combinations of a Phone Number | Medium |

## Complexity Tips

- Subsets: O(n · 2^n) — there are 2^n subsets, each takes O(n) to copy.
- Permutations: O(n · n!) — there are n! permutations.
- Combination Sum: depends on constraints.

## Pitfalls

- **Forgot to undo the choice** — leaves leftover state.
- **Modifying input while iterating** — make a copy first.
- **Duplicate results** — sort input and skip duplicates at same level.
