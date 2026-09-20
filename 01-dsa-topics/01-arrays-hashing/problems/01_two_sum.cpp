// Problem: Two Sum (LeetCode #1)
// Difficulty: Easy
// Approach: Single-pass hash map. For each element, check if its complement
//           (target - num) was already seen. If yes, return indices.
//           Otherwise, store current value → index.
// Time Complexity:  O(n) — one pass through array
// Space Complexity: O(n) — hash map for seen elements

#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

vector<int> twoSum(vector<int>& nums, int target) {
    unordered_map<int, int> seen; // value -> index
    for (int i = 0; i < (int)nums.size(); i++) {
        int complement = target - nums[i];
        if (seen.count(complement)) {
            return {seen[complement], i};
        }
        seen[nums[i]] = i;
    }
    return {}; // no solution (problem guarantees one exists)
}

int main() {
    vector<int> nums = {2, 7, 11, 15};
    int target = 9;
    vector<int> ans = twoSum(nums, target);
    cout << "Indices: " << ans[0] << ", " << ans[1] << endl;
    // Expected: 0, 1
    return 0;
}
