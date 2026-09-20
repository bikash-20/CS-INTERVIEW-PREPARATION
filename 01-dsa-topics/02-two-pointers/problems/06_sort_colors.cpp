// Problem: Sort Colors (Dutch National Flag) (LeetCode #75)
// Difficulty: Medium
// Approach: Three pointers — low, mid, high.
//           nums[0..low-1]   = 0 (red)
//           nums[low..mid-1] = 1 (white)
//           nums[mid..high]  = unsorted
//           nums[high+1..n-1]= 2 (blue)
//           Invariant maintained throughout.
// Time:  O(n), Space: O(1)

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void sortColors(vector<int>& nums) {
    int low = 0, mid = 0, high = nums.size() - 1;
    while (mid <= high) {
        if (nums[mid] == 0) {
            swap(nums[low], nums[mid]);
            low++; mid++;
        } else if (nums[mid] == 1) {
            mid++;
        } else { // nums[mid] == 2
            swap(nums[mid], nums[high]);
            high--;
        }
    }
}

int main() {
    vector<int> nums = {2, 0, 2, 1, 1, 0};
    sortColors(nums);
    for (int x : nums) cout << x << " ";
    cout << endl; // 0 0 1 1 2 2
    return 0;
}
