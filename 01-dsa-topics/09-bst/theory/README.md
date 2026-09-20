# Binary Search Tree (BST) — Theory

## Definition

A **Binary Search Tree** is a binary tree where for each node:
- All nodes in the **left subtree** have values **< node's value**
- All nodes in the **right subtree** have values **> node's value**
- Both subtrees are also BSTs.

```
 8
 / \
 3 10
 / \ \
 1 6 14
 / \ /
 4 7 13
```

## Key Property

**In-order traversal of a BST yields sorted order.**

This is the basis for many elegant BST problems.

## Operations & Complexity

| Operation | Average | Worst (skewed) |
|-----------|---------|----------------|
| Search | O(log n) | O(n) |
| Insert | O(log n) | O(n) |
| Delete | O(log n) | O(n) |
| Min / Max | O(log n) / O(n) | O(n) |
| Find successor/predecessor | O(log n) | O(n) |

The worst case (O(n)) happens when the tree is **skewed** (degenerate into a linked list). Self-balancing trees (AVL, Red-Black) prevent this.

## Classic Problems

### 1. Search in BST
```cpp
TreeNode* searchBST(TreeNode* root, int val) {
 if (!root || root->val == val) return root;
 return val < root->val ? searchBST(root->left, val)
 : searchBST(root->right, val);
}
```

### 2. Insert into BST
- Recurse to appropriate child position. (Or iterative.)

### 3. Delete from BST
Three cases:
- Leaf → just remove
- One child → replace with child
- Two children → replace with in-order successor (smallest in right subtree) or predecessor

### 4. Kth Smallest Element (LC #230)
- In-order traversal; count nodes until you reach k-th.

### 5. Lowest Common Ancestor in BST (LC #235)
- Use BST property: if both p and q < root → go left. If both > root → go right. Else root is LCA.

### 6. Validate BST (LC #98) — covered in Trees
- Check each node lies within valid (min, max) bounds.

### 7. Convert Sorted Array to BST (LC #108)
- Pick middle element as root. Recurse on left and right halves.

### 8. Sorted Array to Balanced BST (LC #109, with LL)
- Find middle using fast/slow pointer. Recurse.

## Top Problems

| # | Problem | Difficulty |
|---|---------|-----------|
| 1 | Search in BST | Easy |
| 2 | Insert into BST | Medium |
| 3 | Validate BST | Medium |
| 4 | Kth Smallest Element | Medium |
| 5 | LCA of BST | Easy |
| 6 | Delete Node in BST | Medium |
| 7 | Convert Sorted Array to BST | Easy |
| 8 | Inorder Successor in BST | Medium |
| 9 | Recover BST | Medium |
| 10 | Binary Search Tree Iterator | Medium |
