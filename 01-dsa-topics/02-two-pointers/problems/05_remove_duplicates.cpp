// Problem: Remove Duplicates from Sorted Array (LeetCode #26)
// Difficulty: Easy
// Approach: Slow pointer = position to write next unique element.
//           Fast pointer scans through array.
// Time:  O(n), Space: O(1)

#include <iostream>
#include <vector>
using namespace std;

int removeDuplicates(vector<int>& nums) {
    if (nums.empty()) return 0;
    int slow = 0;
    for (int fast = 1; fast < (int)nums.size(); fast++) {
        if (nums[fast] != nums[slow]) {
            slow++;
            nums[slow] = nums[fast];
        }
    }
    return slow + 1;
}

int main() {
    vector<int> nums = {0, 0, 1, 1, 1, 2, 2, 3, 3, 4};
    int k = removeDuplicates(nums);
    cout << "Unique count: " << k << endl; // 5
    cout << "Array: ";
    for (int i = 0; i < k; i++) cout << nums[i] << " ";
    cout << endl; // 0 1 2 3 4
    return 0;
}
