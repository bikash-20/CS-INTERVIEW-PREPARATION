// Problem: Longest Common Subsequence (LeetCode #1143)
// Difficulty: Medium
// Approach: 2D DP. If chars match: dp[i][j] = dp[i-1][j-1] + 1.
//           Else: dp[i][j] = max(dp[i-1][j], dp[i][j-1]).
// Time:  O(m*n), Space: O(m*n) → can be optimized to O(min(m,n))

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

int lcs(string a, string b) {
    int m = a.size(), n = b.size();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++) {
            if (a[i-1] == b[j-1]) dp[i][j] = dp[i-1][j-1] + 1;
            else dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
        }
    return dp[m][n];
}

int main() {
    cout << lcs("abcde", "ace") << endl; // 3 ("ace")
    cout << lcs("abc", "abc") << endl;   // 3
    cout << lcs("abc", "def") << endl;   // 0
    return 0;
}
