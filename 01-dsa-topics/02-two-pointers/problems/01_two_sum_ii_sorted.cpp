// Problem: Two Sum II - Input Array Is Sorted (LeetCode #167)
// Difficulty: Easy
// Approach: Two pointers from both ends. If sum == target, done.
//           If sum < target, we need a bigger sum → move left right.
//           If sum > target, we need a smaller sum → move right left.
// Time:  O(n), Space: O(1)

#include <iostream>
#include <vector>
using namespace std;

vector<int> twoSumSorted(vector<int>& numbers, int target) {
    int left = 0, right = numbers.size() - 1;
    while (left < right) {
        int sum = numbers[left] + numbers[right];
        if (sum == target) return {left + 1, right + 1}; // 1-indexed
        else if (sum < target) left++;
        else right--;
    }
    return {};
}

int main() {
    vector<int> nums = {2, 7, 11, 15};
    int target = 9;
    auto ans = twoSumSorted(nums, target);
    cout << ans[0] << " " << ans[1] << endl; // 1 2
    return 0;
}
