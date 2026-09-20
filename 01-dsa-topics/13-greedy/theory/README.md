# Greedy Algorithms — Theory

## What Is Greedy?

Make the **locally optimal choice** at each step, hoping it leads to a globally optimal solution.

**Greedy works when:**
1. **Greedy choice property**: A globally optimal solution can be reached by making locally optimal choices.
2. **Optimal substructure**: An optimal solution contains within it optimal solutions to subproblems.

**Greedy fails when:**
- The locally best choice leads to a suboptimal global solution (e.g., 0/1 knapsack).

## Classic Greedy Problems

### 1. Activity Selection / Meeting Rooms
- Sort intervals by end time. Greedily pick the one that ends earliest.

### 2. Jump Game (LeetCode #55)
- Maintain the **farthest reachable index**. If we reach it, fail.

### 3. Gas Station (LeetCode #134)
- If total gas >= total cost, a solution exists.
- Start from where the cumulative sum becomes negative (after).

### 4. Fractional Knapsack
- Sort items by value/weight ratio descending. Take as much as possible of the highest ratio.

### 5. Huffman Encoding
- Use a min-heap. Combine two smallest frequencies repeatedly.

### 6. Dijkstra's Algorithm
- Always process the unvisited node with smallest distance (greedy on weights).

### 7. Minimum Coins (Unlimited coins, canonical system)
- For US coins (1, 5, 10, 25), greedy works. For arbitrary coin systems, need DP.

### 8. Task Scheduler (LeetCode #621)
- Greedy with cooling: most frequent task first.

### 9. Partition Labels (LeetCode #763)
- Find last occurrence of each char. Extend current partition to its last occurrence.

### 10. Candy (LeetCode #135)
- Two-pass: one left-to-right, one right-to-left.

## Top Problems

| # | Problem | Difficulty |
|---|---------|-----------|
| 1 | Jump Game | Medium |
| 2 | Jump Game II | Medium |
| 3 | Gas Station | Medium |
| 4 | Partition Labels | Medium |
| 5 | Candy | Hard |
| 6 | Non-overlapping Intervals | Medium |
| 7 | Min Arrows to Burst Balloons | Medium |
| 8 | Task Scheduler | Medium |
| 9 | Assign Cookies | Easy |
| 10 | Best Time to Buy/Sell Stock II | Easy |

## Greedy vs DP

- **Greedy:** Choice depends only on current state and the input. No need to consider future consequences.
- **DP:** Choices depend on previously computed sub-results. We explore all combinations.
- When in doubt: **try greedy with a proof**; if counterexample exists, switch to DP.
