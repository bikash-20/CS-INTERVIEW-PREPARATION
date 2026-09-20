# Trie — Theory

## What Is a Trie?

A **Trie** (also called prefix tree or digital tree) is a tree-like data structure that stores a set of strings, where each node represents a single character and edges represent characters.

```
 root
 / \
 a b
 / \ \
 p t a
 / \ \
 p e d
 / \ \ \
e l r s
 \ 
 e 
```

Words stored: "ape", "apple", "at", "ate", "bad", "bads"

## Node Structure (C++)
```cpp
struct TrieNode {
 TrieNode* children[26] = {nullptr}; // for lowercase letters
 bool isEnd = false;
};
```

## Operations & Complexity

| Operation | Time | Space |
|-----------|------|-------|
| Insert word | O(L) | O(L) |
| Search word | O(L) | O(1) |
| Search prefix | O(L) | O(1) |
| Delete | O(L) | O(L) |

L = length of word

## Classic Problems

### 1. Implement Trie (LeetCode #208)
- Standard insert/search/startsWith.

### 2. Word Search II (LeetCode #212)
- Given a board and a list of words, find all words that can be formed.
- Build trie from words, then DFS from each cell.

### 3. Longest Word in Dictionary (LeetCode #720)
- Build trie. BFS level order. Find longest word where every prefix exists.

### 4. Design Add and Search Words Data Structure (LeetCode #211)
- Trie with `.` wildcard matching (DFS).

### 5. Replace Words (LeetCode #648)
- Insert dictionary roots; for each word, find shortest matching root.

### 6. Maximum XOR of Two Numbers (LeetCode #421)
- Binary trie.

### 7. Prefix and Suffix Search (LeetCode #745)
- Trie with combined prefix/suffix keys.

## Top Problems

| # | Problem | Difficulty |
|---|---------|-----------|
| 1 | Implement Trie | Medium |
| 2 | Design Add and Search Words | Medium |
| 3 | Word Search II | Hard |
| 4 | Longest Word in Dictionary | Easy |
| 5 | Replace Words | Medium |
| 6 | Maximum XOR of Two Numbers | Medium |
| 7 | Prefix and Suffix Search | Hard |

## Why Use a Trie?

 Fast prefix lookups
 Lexicographic ordering (DFS yields sorted words)
 Autocomplete / typeahead suggestions
 Spell checkers
 IP routing tables
 High memory usage (each node has many pointers)
