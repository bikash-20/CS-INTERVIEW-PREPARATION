// Problem: Single Number (LeetCode #136)
// Difficulty: Easy
// Approach: XOR all elements. Duplicates cancel out (a^a = 0), unique remains.
// Time:  O(n), Space: O(1)

#include <iostream>
#include <vector>
using namespace std;

int singleNumber(vector<int>& nums) {
    int ans = 0;
    for (int x : nums) ans ^= x;
    return ans;
}

int main() {
    vector<int> a = {2, 2, 1};
    cout << singleNumber(a) << endl; // 1

    vector<int> b = {4, 1, 2, 1, 2};
    cout << singleNumber(b) << endl; // 4
    return 0;
}
