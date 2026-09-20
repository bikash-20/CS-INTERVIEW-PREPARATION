# Trees — Theory

## What Is a Tree?

A tree is a hierarchical, non-linear data structure with a root and child nodes. A binary tree has at most 2 children per node.

```
 1
 / \
 2 3
 / \ \
 4 5 6
```

### Terminology
- **Root** — top node
- **Leaf** — node with no children
- **Depth** — distance from root
- **Height** — longest path from node to a leaf
- **Subtree** — a node and all its descendants
- **Ancestor / Descendant**

### Node Definition (C++)
```cpp
struct TreeNode {
 int val;
 TreeNode* left, *right;
 TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};
```

## Tree Traversals

### Depth-First Search (DFS)

1. **Pre-order** (root → left → right): `4 2 1 3 5`
2. **In-order** (left → root → right): `1 2 3 4 5` (gives sorted order for BST)
3. **Post-order** (left → right → root): `1 3 2 5 4`

```
 4
 / \
 2 5
 / \
 1 3
```

### Breadth-First Search (BFS) — Level Order
`4 2 5 1 3` — visit each level left to right.

### Recursive Templates

```cpp
// Pre-order
void preorder(TreeNode* root) {
 if (!root) return;
 visit(root);
 preorder(root->left);
 preorder(root->right);
}
```

## Classic Problems

### 1. Maximum Depth of Binary Tree (LC #104)
```cpp
int maxDepth(TreeNode* root) {
 if (!root) return 0;
 return 1 + max(maxDepth(root->left), maxDepth(root->right));
}
```

### 2. Invert Binary Tree (LC #226)
- Swap left and right children recursively.

### 3. Validate BST (LC #98)
- Each node must satisfy: max(left subtree) < node < min(right subtree).
- Recursive helper with `(min, max)` bounds.

### 4. Lowest Common Ancestor (LC #236)
- If both p and q are in left/right subtrees, root is LCA.
- If both in same subtree, recurse there.

### 5. Serialize/Deserialize Binary Tree (LC #297)
- BFS or DFS to convert to string and back.

### 6. Binary Tree Maximum Path Sum (LC #124)
- At each node, the best path through it = left_gain + node + right_gain.

### 7. Diameter of Binary Tree (LC #543)
- Longest path between any two nodes.
- At each node, diameter = max(leftHeight + rightHeight).

### 8. Level Order Traversal (LC #102)
- BFS using queue.

### 9. Construct Tree from Preorder + Inorder (LC #105)
- First element of preorder is root; find it in inorder to split left/right.

### 10. Kth Smallest Element in BST (LC #230)
- In-order traversal, count until k.

## Top Problems

| # | Problem | Difficulty |
|---|---------|-----------|
| 1 | Maximum Depth of Binary Tree | Easy |
| 2 | Invert Binary Tree | Easy |
| 3 | Same Tree | Easy |
| 4 | Subtree of Another Tree | Easy |
| 5 | Validate BST | Medium |
| 6 | Level Order Traversal | Medium |
| 7 | Construct from Preorder + Inorder | Medium |
| 8 | LCA of BST | Medium |
| 9 | Binary Tree Maximum Path Sum | Hard |
| 10 | Serialize and Deserialize | Hard |

## Time Complexity

| Operation | Avg / Worst |
|-----------|-------------|
| Search | O(n) — must visit every node |
| Insert | O(n) |
| Delete | O(n) |
| Traversal | O(n) |
