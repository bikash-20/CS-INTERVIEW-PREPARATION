// Problem: Subarray Sum Equals K (LeetCode #560)
// Difficulty: Medium
// Approach: At each index, current prefix sum is `prefix`. We need earlier
//           prefix sum == prefix - k. Count how many times that occurred.
// Time:  O(n), Space: O(n)

#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

int subarraySum(vector<int>& nums, int k) {
    unordered_map<int, int> prefixCount;
    prefixCount[0] = 1; // empty prefix
    int prefix = 0, count = 0;
    for (int x : nums) {
        prefix += x;
        count += prefixCount[prefix - k];
        prefixCount[prefix]++;
    }
    return count;
}

int main() {
    vector<int> a = {1, 1, 1};
    cout << subarraySum(a, 2) << endl; // 2 ([1,1] at idx 0-1, 1-2)

    vector<int> b = {1, 2, 3};
    cout << subarraySum(b, 3) << endl; // 2 ([1,2], [3])
    return 0;
}
