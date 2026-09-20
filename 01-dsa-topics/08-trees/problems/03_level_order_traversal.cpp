// Problem: Binary Tree Level Order Traversal (LeetCode #102)
// Difficulty: Medium
// Approach: BFS using queue. Process each level separately.
// Time:  O(n), Space: O(n)

#include <iostream>
#include <vector>
#include <queue>
using namespace std;

struct TreeNode {
    int val;
    TreeNode* left, *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

vector<vector<int>> levelOrder(TreeNode* root) {
    vector<vector<int>> result;
    if (!root) return result;
    queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
        int size = q.size();
        vector<int> level;
        for (int i = 0; i < size; i++) {
            TreeNode* node = q.front(); q.pop();
            level.push_back(node->val);
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        result.push_back(level);
    }
    return result;
}

int main() {
    TreeNode* root = new TreeNode(3);
    root->left = new TreeNode(9);
    root->right = new TreeNode(20);
    root->right->left = new TreeNode(15);
    root->right->right = new TreeNode(7);

    auto result = levelOrder(root);
    for (auto& level : result) {
        cout << "[ ";
        for (int x : level) cout << x << " ";
        cout << "]\n";
    }
    // [ 3 ] [ 9 20 ] [ 15 7 ]
    return 0;
}
