// Problem: Climbing Stairs (LeetCode #70)
// Difficulty: Easy
// Approach: dp[i] = dp[i-1] + dp[i-2]. Space-optimized to 2 vars.
// Time:  O(n), Space: O(1)

#include <iostream>
using namespace std;

int climbStairs(int n) {
    if (n <= 2) return n;
    int prev2 = 1, prev1 = 2;
    for (int i = 3; i <= n; i++) {
        int cur = prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

int main() {
    cout << climbStairs(2) << endl;  // 2
    cout << climbStairs(3) << endl;  // 3
    cout << climbStairs(10) << endl; // 89
    return 0;
}
