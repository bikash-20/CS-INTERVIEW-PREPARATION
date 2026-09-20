// Problem: Coin Change (LeetCode #322)
// Difficulty: Medium
// Approach: dp[i] = min coins to make amount i. For each coin, update dp[i] = min(dp[i], dp[i-coin] + 1).
// Time:  O(amount * n_coins), Space: O(amount)

#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

int coinChange(vector<int>& coins, int amount) {
    vector<int> dp(amount + 1, INT_MAX);
    dp[0] = 0;
    for (int i = 1; i <= amount; i++) {
        for (int c : coins) {
            if (i - c >= 0 && dp[i - c] != INT_MAX)
                dp[i] = min(dp[i], dp[i - c] + 1);
        }
    }
    return dp[amount] == INT_MAX ? -1 : dp[amount];
}

int main() {
    vector<int> coins = {1, 5, 10, 25};
    cout << coinChange(coins, 11) << endl; // 2 (5+5+1? No, 10+1 = 2 coins)
    cout << coinChange(coins, 30) << endl; // 2 (25+5)
    return 0;
}
