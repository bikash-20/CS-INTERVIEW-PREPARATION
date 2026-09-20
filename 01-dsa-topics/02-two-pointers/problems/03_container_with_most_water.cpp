// Problem: Container With Most Water (LeetCode #11)
// Difficulty: Medium
// Approach: Place left at 0, right at n-1. Area = (right-left) * min(height[l], height[r]).
//           To potentially increase area, move the SHORTER side inward —
//           the only way to get more area is with a taller line, which can only
//           come from the side we move.
// Time:  O(n), Space: O(1)

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int maxArea(vector<int>& height) {
    int left = 0, right = height.size() - 1;
    int maxArea = 0;
    while (left < right) {
        int h = min(height[left], height[right]);
        int area = (right - left) * h;
        maxArea = max(maxArea, area);
        if (height[left] < height[right]) left++;
        else right--;
    }
    return maxArea;
}

int main() {
    vector<int> h = {1, 8, 6, 2, 5, 4, 8, 3, 7};
    cout << "Max area: " << maxArea(h) << endl; // 49
    return 0;
}
