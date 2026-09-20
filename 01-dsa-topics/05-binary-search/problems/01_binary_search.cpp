// Problem: Binary Search (LeetCode #704)
// Difficulty: Easy
// Approach: Classic binary search on sorted array.
// Time:  O(log n), Space: O(1)

#include <iostream>
#include <vector>
using namespace std;

int search(vector<int>& nums, int target) {
    int left = 0, right = (int)nums.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (nums[mid] == target) return mid;
        else if (nums[mid] < target) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

int main() {
    vector<int> a = {-1, 0, 3, 5, 9, 12};
    cout << search(a, 9) << endl;  // 4
    cout << search(a, 2) << endl;  // -1
    return 0;
}
