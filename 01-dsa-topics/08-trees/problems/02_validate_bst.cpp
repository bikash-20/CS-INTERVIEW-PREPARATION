// Problem: Validate Binary Search Tree (LeetCode #98)
// Difficulty: Medium
// Approach: Recurse with (min, max) bounds. Each node's value must be within (min, max).
// Time:  O(n), Space: O(h)

#include <iostream>
#include <climits>
using namespace std;

struct TreeNode {
    int val;
    TreeNode* left, *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

bool validate(TreeNode* root, long long minVal, long long maxVal) {
    if (!root) return true;
    if (root->val <= minVal || root->val >= maxVal) return false;
    return validate(root->left, minVal, root->val)
        && validate(root->right, root->val, maxVal);
}

bool isValidBST(TreeNode* root) {
    return validate(root, LLONG_MIN, LLONG_MAX);
}

int main() {
    TreeNode* root = new TreeNode(2);
    root->left = new TreeNode(1);
    root->right = new TreeNode(3);
    cout << isValidBST(root) << endl; // 1 (true)

    TreeNode* bad = new TreeNode(5);
    bad->left = new TreeNode(1);
    bad->right = new TreeNode(4);
    bad->right->left = new TreeNode(3);
    bad->right->right = new TreeNode(6);
    cout << isValidBST(bad) << endl; // 0 (false)
    return 0;
}
