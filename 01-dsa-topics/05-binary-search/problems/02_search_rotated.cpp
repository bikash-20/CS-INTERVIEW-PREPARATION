// Problem: Search in Rotated Sorted Array (LeetCode #33)
// Difficulty: Medium
// Approach: After rotation, one half is always sorted. Determine which half,
//           then check if target lies in the sorted half.
// Time:  O(log n), Space: O(1)

#include <iostream>
#include <vector>
using namespace std;

int searchRotated(vector<int>& nums, int target) {
    int left = 0, right = (int)nums.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (nums[mid] == target) return mid;

        if (nums[left] <= nums[mid]) { // left half sorted
            if (target >= nums[left] && target < nums[mid])
                right = mid - 1;
            else
                left = mid + 1;
        } else { // right half sorted
            if (target > nums[mid] && target <= nums[right])
                left = mid + 1;
            else
                right = mid - 1;
        }
    }
    return -1;
}

int main() {
    vector<int> a = {4, 5, 6, 7, 0, 1, 2};
    cout << searchRotated(a, 0) << endl; // 4
    cout << searchRotated(a, 3) << endl; // -1
    return 0;
}
