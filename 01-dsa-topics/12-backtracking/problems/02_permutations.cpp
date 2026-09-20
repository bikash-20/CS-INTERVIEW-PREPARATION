// Problem: Permutations (LeetCode #46)
// Difficulty: Medium
// Approach: Backtrack. Track "used" elements. Try each unused element at the current position.
// Time:  O(n * n!), Space: O(n) recursion

#include <iostream>
#include <vector>
using namespace std;

void backtrack(vector<int>& nums, vector<bool>& used, vector<int>& path, vector<vector<int>>& res) {
    if ((int)path.size() == (int)nums.size()) {
        res.push_back(path);
        return;
    }
    for (int i = 0; i < (int)nums.size(); i++) {
        if (used[i]) continue;
        used[i] = true;
        path.push_back(nums[i]);
        backtrack(nums, used, path, res);
        path.pop_back();
        used[i] = false;
    }
}

vector<vector<int>> permute(vector<int>& nums) {
    vector<vector<int>> res;
    vector<int> path;
    vector<bool> used(nums.size(), false);
    backtrack(nums, used, path, res);
    return res;
}

int main() {
    vector<int> nums = {1, 2, 3};
    auto perms = permute(nums);
    cout << "Count: " << perms.size() << endl; // 6
    return 0;
}
