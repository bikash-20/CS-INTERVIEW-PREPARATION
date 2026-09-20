// Problem: Lowest Common Ancestor of a BST (LeetCode #235)
// Difficulty: Easy
// Approach: Use BST property — both p,q < root -> go left; both > root -> go right;
//           else current node is the LCA.
// Time:  O(h), Space: O(1)

#include <iostream>
using namespace std;

struct TreeNode {
    int val;
    TreeNode* left, *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
    while (root) {
        if (p->val < root->val && q->val < root->val) root = root->left;
        else if (p->val > root->val && q->val > root->val) root = root->right;
        else return root;
    }
    return nullptr;
}

int main() {
    TreeNode* root = new TreeNode(6);
    root->left = new TreeNode(2); root->right = new TreeNode(8);
    root->left->left = new TreeNode(0); root->left->right = new TreeNode(4);
    root->right->left = new TreeNode(7); root->right->right = new TreeNode(9);
    root->left->right->left = new TreeNode(3); root->left->right->right = new TreeNode(5);
    TreeNode* p = root->left; // 2
    TreeNode* q = root->right; // 8
    cout << lowestCommonAncestor(root, p, q)->val << endl; // 6
    return 0;
}
