// Problem: 3Sum (LeetCode #15)
// Difficulty: Medium
// Approach: Sort array. For each i, run two-pointer search for (-nums[i])
//           in nums[i+1..n-1]. Skip duplicates to avoid duplicate triplets.
// Time:  O(n²), Space: O(1) extra (ignoring output)

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

vector<vector<int>> threeSum(vector<int>& nums) {
    vector<vector<int>> result;
    sort(nums.begin(), nums.end());
    int n = nums.size();

    for (int i = 0; i < n - 2; i++) {
        // Skip duplicates for i
        if (i > 0 && nums[i] == nums[i - 1]) continue;
        // Early termination: if smallest possible sum > 0, break
        if (nums[i] + nums[i + 1] + nums[i + 2] > 0) break;
        // Skip: if largest possible sum < 0, continue
        if (nums[i] + nums[n - 2] + nums[n - 1] < 0) continue;

        int left = i + 1, right = n - 1;
        while (left < right) {
            int sum = nums[i] + nums[left] + nums[right];
            if (sum == 0) {
                result.push_back({nums[i], nums[left], nums[right]});
                // Skip duplicates
                while (left < right && nums[left] == nums[left + 1]) left++;
                while (left < right && nums[right] == nums[right - 1]) right--;
                left++; right--;
            } else if (sum < 0) left++;
            else right--;
        }
    }
    return result;
}

int main() {
    vector<int> nums = {-1, 0, 1, 2, -1, -4};
    auto triplets = threeSum(nums);
    for (auto& t : triplets) {
        cout << "[ ";
        for (int x : t) cout << x << " ";
        cout << "]\n";
    }
    // Expected: [-1 -1 2] [-1 0 1]
    return 0;
}
