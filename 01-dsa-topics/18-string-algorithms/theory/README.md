# String Algorithms — Theory

## Common Patterns

### 1. Anagram Check / Frequency Map
Use array of size 26 (for lowercase) to count chars.

### 2. Palindromes
- Two-pointer from ends.
- For substring: expand around center.
- For subsequence: DP (longest palindromic subsequence).

### 3. Substring Search
- **Naive**: O(nm)
- **KMP**: O(n + m)
- **Rabin-Karp**: O(n + m) average using rolling hash
- **Z-algorithm**: O(n + m)

## KMP Algorithm (Knuth-Morris-Pratt)

Finds all occurrences of pattern `P` in text `T` in **O(n + m)**.

**Key idea:** Preprocess `P` to compute the **failure function** (LPS array) — the length of the longest proper prefix that is also a suffix.

```
P = "aabaaab"
LPS = [0, 1, 0, 1, 2, 2, 3]
 a a b a a a b
```

Then during search, when there's a mismatch, we use LPS to know how many chars to skip.

## Rabin-Karp (Rolling Hash)

Compute hash of substrings in O(1) each by rolling.

```
hash("abc") = a*p^2 + b*p^1 + c*p^0
hash("bcd") = (hash("abc") - a*p^2) * p + d
```

- O(n + m) average. Worst O(nm) due to hash collisions.
- Good for "find all anagram occurrences" etc.

## Z-Algorithm

For each position i, Z[i] = length of longest substring starting at i that is also a prefix of the string.
- O(n).
- Useful for pattern matching, finding repetitions.

## Top Problems

| # | Problem | Difficulty |
|---|---------|-----------|
| 1 | Valid Anagram | Easy |
| 2 | Longest Palindromic Substring | Medium |
| 3 | String to Integer (atoi) | Medium |
| 4 | Longest Substring Without Repeating Chars | Medium |
| 5 | Longest Repeating Character Replacement | Medium |
| 6 | Group Anagrams | Medium |
| 7 | Minimum Window Substring | Hard |
| 8 | Valid Parentheses | Easy |
| 9 | Implement strStr() | Easy |
| 10 | String Compression | Medium |
| 11 | Multiply Strings | Medium |
| 12 | Word Break | Medium |
| 13 | Edit Distance | Medium |
| 14 | Distinct Subsequences | Hard |
