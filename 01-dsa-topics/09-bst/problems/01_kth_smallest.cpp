// Problem: Kth Smallest Element in a BST (LeetCode #230)
// Difficulty: Medium
// Approach: In-order traversal; BST in-order is sorted. Stop at k-th.
// Time:  O(h + k), Space: O(h)

#include <iostream>
using namespace std;

struct TreeNode {
    int val;
    TreeNode* left, *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

class Solution {
    int count = 0, ans = -1;
    void inorder(TreeNode* root, int k) {
        if (!root) return;
        inorder(root->left, k);
        count++;
        if (count == k) { ans = root->val; return; }
        inorder(root->right, k);
    }
public:
    int kthSmallest(TreeNode* root, int k) {
        inorder(root, k);
        return ans;
    }
};

int main() {
    TreeNode* root = new TreeNode(5);
    root->left = new TreeNode(3);
    root->right = new TreeNode(6);
    root->left->left = new TreeNode(2);
    root->left->right = new TreeNode(4);
    root->left->left->left = new TreeNode(1);
    Solution s;
    cout << s.kthSmallest(root, 3) << endl; // 3
    return 0;
}
