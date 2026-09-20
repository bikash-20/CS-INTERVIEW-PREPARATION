// Problem: House Robber (LeetCode #198)
// Difficulty: Medium
// Approach: dp[i] = max(dp[i-1], dp[i-2] + nums[i]).
//           Either skip house i (dp[i-1]) or rob it (dp[i-2] + nums[i]).
// Time:  O(n), Space: O(1)

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int rob(vector<int>& nums) {
    int prev2 = 0, prev1 = 0;
    for (int x : nums) {
        int cur = max(prev1, prev2 + x);
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

int main() {
    vector<int> a = {1, 2, 3, 1};
    cout << rob(a) << endl; // 4 (1+3)

    vector<int> b = {2, 7, 9, 3, 1};
    cout << rob(b) << endl; // 12 (2+9+1)
    return 0;
}
