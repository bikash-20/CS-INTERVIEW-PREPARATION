# Graphs — Theory

## Definition

A **graph** G = (V, E) is a set of **vertices** (nodes) connected by **edges**.

```
 A --- B
 | |
 C --- D --- E
```

- **Directed** (edges have direction) vs **Undirected**
- **Weighted** (edges have cost) vs **Unweighted**
- **Cyclic** vs **Acyclic** (DAG = Directed Acyclic Graph)

## Representations

### 1. Adjacency Matrix — O(V²) space
```
 A B C D
A [ 0 1 1 0 ]
B [ 1 0 0 1 ]
C [ 1 0 0 1 ]
D [ 0 1 1 0 ]
```

### 2. Adjacency List — O(V + E) space Preferred
```cpp
unordered_map<int, vector<int>> adj;
adj[0] = {1, 2};
adj[1] = {0, 3};
// ...
```

## Core Algorithms

### 1. BFS (Breadth-First Search)
- Uses a **queue**. Explores neighbors first, then deeper.
- **Shortest path** in unweighted graph.
- Used for: level-order traversal in trees, shortest path, connected components, bipartite check.

```cpp
void bfs(int start, vector<vector<int>>& adj, vector<bool>& visited) {
 queue<int> q;
 q.push(start);
 visited[start] = true;
 while (!q.empty()) {
 int u = q.front(); q.pop();
 for (int v : adj[u]) {
 if (!visited[v]) {
 visited[v] = true;
 q.push(v);
 }
 }
 }
}
```

### 2. DFS (Depth-First Search)
- Uses a **stack** (or recursion). Goes deep before backtracking.
- Used for: cycle detection, topological sort, connected components, path finding.

```cpp
void dfs(int u, vector<vector<int>>& adj, vector<bool>& visited) {
 visited[u] = true;
 for (int v : adj[u])
 if (!visited[v]) dfs(v, adj, visited);
}
```

### 3. Topological Sort
- Linear ordering of DAG vertices such that for every edge u→v, u comes before v.
- **Kahn's algorithm** (BFS-based): repeatedly remove nodes with in-degree 0.

### 4. Dijkstra's Algorithm
- Shortest path in weighted graph with **non-negative** weights.
- Uses priority queue. O((V+E) log V).

### 5. Union-Find (Disjoint Set Union)
- Track connected components. Two operations:
 - `find(x)` — returns the root representative
 - `union(x, y)` — merges two sets
- **Path compression** + **union by rank** → near O(1) per operation (α(n)).

### 6. Bellman-Ford
- Single-source shortest paths even with negative weights.
- O(V × E).

### 7. Floyd-Warshall
- All-pairs shortest paths. O(V³).

### 8. Minimum Spanning Tree (Kruskal / Prim)
- Kruskal: sort edges, add smallest that doesn't form cycle (Union-Find).
- Prim: grow tree greedily.

## Top Interview Problems

| # | Problem | Difficulty | Technique |
|---|---------|-----------|-----------|
| 1 | Number of Islands | Medium | DFS/BFS on grid |
| 2 | Clone Graph | Medium | DFS + hashmap |
| 3 | Course Schedule | Medium | Topological sort |
| 4 | Pacific Atlantic Water Flow | Medium | DFS |
| 5 | Word Ladder | Hard | BFS |
| 6 | Network Delay Time | Medium | Dijkstra |
| 7 | Redundant Connection | Medium | Union-Find |
| 8 | Graph Valid Tree | Medium | Union-Find / DFS |
| 9 | Alien Dictionary | Hard | Topological sort |
| 10 | Cheapest Flights Within K Stops | Medium | Bellman-Ford |
