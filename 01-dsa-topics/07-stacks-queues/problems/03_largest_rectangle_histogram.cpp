// Problem: Largest Rectangle in Histogram (LeetCode #84)
// Difficulty: Hard
// Approach: Monotonic increasing stack. For each bar, find the nearest smaller
//           bar on left and right; the rectangle spans that range.
// Time:  O(n), Space: O(n)

#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
using namespace std;

int largestRectangleArea(vector<int>& heights) {
    int n = heights.size();
    stack<int> st; // indices of increasing heights
    int maxArea = 0;

    for (int i = 0; i <= n; i++) {
        int curH = (i == n) ? 0 : heights[i];
        while (!st.empty() && curH < heights[st.top()]) {
            int h = heights[st.top()]; st.pop();
            int w = st.empty() ? i : i - st.top() - 1;
            maxArea = max(maxArea, h * w);
        }
        st.push(i);
    }
    return maxArea;
}

int main() {
    vector<int> h = {2, 1, 5, 6, 2, 3};
    cout << largestRectangleArea(h) << endl; // 10
    return 0;
}
