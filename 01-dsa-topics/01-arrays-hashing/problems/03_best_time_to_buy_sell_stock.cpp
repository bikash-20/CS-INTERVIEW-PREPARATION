// Problem: Best Time to Buy and Sell Stock (LeetCode #121)
// Difficulty: Easy
// Approach: Track the minimum price seen so far. For each day, profit if sold
//           today is (price - minSoFar). Track max profit.
// Time:  O(n), Space: O(1)

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int maxProfit(vector<int>& prices) {
    int minPrice = INT_MAX;
    int maxProfit = 0;
    for (int price : prices) {
        minPrice = min(minPrice, price);
        maxProfit = max(maxProfit, price - minPrice);
    }
    return maxProfit;
}

int main() {
    vector<int> prices = {7, 1, 5, 3, 6, 4};
    cout << "Max profit: " << maxProfit(prices) << endl; // 5 (buy at 1, sell at 6)
    return 0;
}
