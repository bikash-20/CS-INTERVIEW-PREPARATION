// Problem: Trapping Rain Water (LeetCode #42)
// Difficulty: Hard
// Approach: Water at index i = min(maxLeft[i], maxRight[i]) - height[i].
//           Two-pointer version: maintain leftMax and rightMax.
//           Whichever side is shorter, process that side.
// Time:  O(n), Space: O(1)

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int trap(vector<int>& height) {
    int left = 0, right = height.size() - 1;
    int leftMax = 0, rightMax = 0;
    int water = 0;

    while (left < right) {
        if (height[left] < height[right]) {
            if (height[left] >= leftMax) leftMax = height[left];
            else water += leftMax - height[left];
            left++;
        } else {
            if (height[right] >= rightMax) rightMax = height[right];
            else water += rightMax - height[right];
            right--;
        }
    }
    return water;
}

int main() {
    vector<int> h = {0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1};
    cout << "Water trapped: " << trap(h) << endl; // 6
    return 0;
}
