# Stacks & Queues — Theory

## Stack (LIFO)

Last In, First Out — like a stack of plates.

```
push 1, push 2, push 3
Stack (top -> bottom): 3 2 1

pop() -> 3
pop() -> 2
```

### Operations — all O(1)
- `push(x)` — add to top
- `pop()` — remove top
- `top()/peek()` — view top
- `empty()`, `size()`

### C++ STL: `std::stack<T>`

## Queue (FIFO)

First In, First Out — like a real-world queue.

```
enqueue 1, enqueue 2, enqueue 3
Queue (front -> back): 1 2 3

dequeue() -> 1
dequeue() -> 2
```

### C++ STL: `std::queue<T>` (backed by deque)

## Classic Stack Problems

### 1. Valid Parentheses (LeetCode #20)
- Push opening brackets. On closing, check if top of stack matches.
- Use a map of closing → opening brackets.

### 2. Min Stack (LeetCode #155)
- Support `getMin()` in O(1).
- Use an auxiliary stack that stores minimums so far.
- Or store pairs `{value, current_min}`.

### 3. Next Greater Element (LeetCode #496)
- For each element, find next greater to the right.
- **Monotonic stack:** Iterate right-to-left, pop from stack while top <= current, top of stack is the answer.

### 4. Largest Rectangle in Histogram (LeetCode #84)
- For each bar, find nearest smaller bar on left and right.
- Use monotonic increasing stack to find boundaries in O(n).

### 5. Daily Temperatures (LeetCode #739)
- Same as Next Greater Element but return distance.

### 6. Evaluate Reverse Polish Notation (LeetCode #150)
- Stack-based expression evaluation.

### 7. Implement Queue using Stacks (LeetCode #232)
- Two stacks: `inStack` and `outStack`. Amortized O(1).

### 8. Simplify Path (LeetCode #71)
- Split by `/`, use stack for valid directory names.

## Monotonic Stack Pattern

**Use when:**
- Next greater/smaller element
- Largest rectangle in histogram
- Trapping rain water (some approaches)
- Stock span problem

**Template — Next Smaller to Left:**
```cpp
vector<int> nextSmallerLeft(vector<int>& arr) {
 vector<int> ans(arr.size(), -1);
 stack<int> st; // stores indices
 for (int i = 0; i < (int)arr.size(); i++) {
 while (!st.empty() && arr[st.top()] >= arr[i]) st.pop();
 ans[i] = st.empty() ? -1 : st.top();
 st.push(i);
 }
 return ans;
}
```

## Top Problems

| # | Problem | Difficulty |
|---|---------|-----------|
| 1 | Valid Parentheses | Easy |
| 2 | Min Stack | Medium |
| 3 | Implement Queue using Stacks | Easy |
| 4 | Next Greater Element I | Easy |
| 5 | Daily Temperatures | Medium |
| 6 | Largest Rectangle in Histogram | Hard |
| 7 | Sliding Window Maximum | Hard |
| 8 | Decode String | Medium |
| 9 | Simplify Path | Medium |
| 10 | Basic Calculator II | Medium |
