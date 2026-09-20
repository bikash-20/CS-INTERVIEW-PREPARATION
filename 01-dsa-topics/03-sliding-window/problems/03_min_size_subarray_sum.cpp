// Problem: Minimum Size Subarray Sum (LeetCode #209)
// Difficulty: Medium
// Approach: Two pointers — expand right, shrink left while sum >= target.
//           Track the smallest window satisfying the constraint.
// Time:  O(n), Space: O(1)

#include <iostream>
#include <vector>
#include <climits>
using namespace std;

int minSubArrayLen(int target, vector<int>& nums) {
    int n = nums.size();
    int left = 0, sum = 0, best = INT_MAX;
    for (int right = 0; right < n; right++) {
        sum += nums[right];
        while (sum >= target) {
            best = min(best, right - left + 1);
            sum -= nums[left++];
        }
    }
    return best == INT_MAX ? 0 : best;
}

int main() {
    vector<int> a = {2, 3, 1, 2, 4, 3};
    int target = 7;
    cout << minSubArrayLen(target, a) << endl; // 2 ([4,3])
    return 0;
}
