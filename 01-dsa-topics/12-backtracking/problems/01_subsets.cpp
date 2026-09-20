// Problem: Subsets (LeetCode #78)
// Difficulty: Medium
// Approach: Backtracking — for each element, choose to include it or not.
// Time:  O(n * 2^n), Space: O(n) recursion stack

#include <iostream>
#include <vector>
using namespace std;

void backtrack(vector<int>& nums, int start, vector<int>& path, vector<vector<int>>& res) {
    res.push_back(path);
    for (int i = start; i < (int)nums.size(); i++) {
        path.push_back(nums[i]);
        backtrack(nums, i + 1, path, res);
        path.pop_back();
    }
}

vector<vector<int>> subsets(vector<int>& nums) {
    vector<vector<int>> res;
    vector<int> path;
    backtrack(nums, 0, path, res);
    return res;
}

int main() {
    vector<int> nums = {1, 2, 3};
    auto subs = subsets(nums);
    for (auto& s : subs) {
        cout << "[ ";
        for (int x : s) cout << x << " ";
        cout << "]\n";
    }
    // [], [1], [1 2], [1 2 3], [1 3], [2], [2 3], [3]
    return 0;
}
