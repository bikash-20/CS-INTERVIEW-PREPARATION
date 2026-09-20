// Problem: Maximum Sum Subarray of Size K (fixed window)
// Difficulty: Easy
// Approach: Compute sum of first K elements. Then slide: each step
//           adds arr[i] and removes arr[i-k].
// Time:  O(n), Space: O(1)

#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

int maxSumK(vector<int>& arr, int k) {
    int n = arr.size();
    if (n < k) return -1;

    int windowSum = 0;
    for (int i = 0; i < k; i++) windowSum += arr[i];

    int best = windowSum;
    for (int i = k; i < n; i++) {
        windowSum += arr[i] - arr[i - k];
        best = max(best, windowSum);
    }
    return best;
}

int main() {
    vector<int> a = {2, 1, 5, 1, 3, 2};
    cout << maxSumK(a, 3) << endl; // 9 (5+1+3)
    return 0;
}
