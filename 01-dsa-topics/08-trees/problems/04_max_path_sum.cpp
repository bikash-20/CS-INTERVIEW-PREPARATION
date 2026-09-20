// Problem: Binary Tree Maximum Path Sum (LeetCode #124)
// Difficulty: Hard
// Approach: At each node, compute the max gain going up (single path). The
//           best path THROUGH this node is leftGain + val + rightGain,
//           which we track globally.
// Time:  O(n), Space: O(h)

#include <iostream>
#include <algorithm>
#include <climits>
using namespace std;

struct TreeNode {
    int val;
    TreeNode* left, *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

class Solution {
    int maxSum = INT_MIN;
    int maxGain(TreeNode* node) {
        if (!node) return 0;
        int left = max(0, maxGain(node->left));
        int right = max(0, maxGain(node->right));
        maxSum = max(maxSum, left + right + node->val);
        return node->val + max(left, right);
    }
public:
    int maxPathSum(TreeNode* root) {
        maxGain(root);
        return maxSum;
    }
};

int main() {
    TreeNode* root = new TreeNode(-10);
    root->left = new TreeNode(9);
    root->right = new TreeNode(20);
    root->right->left = new TreeNode(15);
    root->right->right = new TreeNode(7);
    Solution s;
    cout << s.maxPathSum(root) << endl; // 42 (15 + 20 + 7)
    return 0;
}
