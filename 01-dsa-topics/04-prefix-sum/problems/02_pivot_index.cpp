// Problem: Pivot Index (LeetCode #724)
// Difficulty: Easy
// Approach: Compute total sum. Iterate; leftSum = prefix sum so far,
//           rightSum = total - leftSum - nums[i].
// Time:  O(n), Space: O(1)

#include <iostream>
#include <vector>
#include <numeric>
using namespace std;

int pivotIndex(vector<int>& nums) {
    int total = accumulate(nums.begin(), nums.end(), 0);
    int leftSum = 0;
    for (int i = 0; i < (int)nums.size(); i++) {
        int rightSum = total - leftSum - nums[i];
        if (leftSum == rightSum) return i;
        leftSum += nums[i];
    }
    return -1;
}

int main() {
    vector<int> a = {1, 7, 3, 6, 5, 6};
    cout << pivotIndex(a) << endl; // 3 (left sum=11, right sum=11)

    vector<int> b = {1, 2, 3};
    cout << pivotIndex(b) << endl; // -1
    return 0;
}
