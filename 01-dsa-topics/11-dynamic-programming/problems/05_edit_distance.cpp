// Problem: Edit Distance (LeetCode #72)
// Difficulty: Medium
// Approach: 2D DP. dp[i][j] = min ops to convert a[0..i-1] to b[0..j-1].
//           Match: dp[i-1][j-1]. Mismatch: 1 + min(dp[i-1][j],   // delete
//                                                  dp[i][j-1],   // insert
//                                                  dp[i-1][j-1]). // replace
// Time:  O(m*n), Space: O(m*n)

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

int editDistance(string a, string b) {
    int m = a.size(), n = b.size();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    for (int i = 0; i <= m; i++) dp[i][0] = i;
    for (int j = 0; j <= n; j++) dp[0][j] = j;

    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++) {
            if (a[i-1] == b[j-1]) dp[i][j] = dp[i-1][j-1];
            else dp[i][j] = 1 + min({dp[i-1][j], dp[i][j-1], dp[i-1][j-1]});
        }
    return dp[m][n];
}

int main() {
    cout << editDistance("horse", "ros") << endl; // 3
    cout << editDistance("intention", "execution") << endl; // 5
    return 0;
}
