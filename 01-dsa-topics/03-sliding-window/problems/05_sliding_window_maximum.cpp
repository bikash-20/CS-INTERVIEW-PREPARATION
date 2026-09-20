// Problem: Sliding Window Maximum (LeetCode #239)
// Difficulty: Hard
// Approach: Monotonic deque. Store INDICES in deque such that values are decreasing.
//           - Before adding arr[i], pop from back while arr[back] <= arr[i].
//           - Pop from front if its index < i - k + 1 (out of window).
//           - The front of deque is always the max for the current window.
// Time:  O(n), Space: O(k)

#include <iostream>
#include <vector>
#include <deque>
using namespace std;

vector<int> maxSlidingWindow(vector<int>& nums, int k) {
    deque<int> dq; // stores indices, decreasing values
    vector<int> result;

    for (int i = 0; i < (int)nums.size(); i++) {
        // Remove indices out of window
        while (!dq.empty() && dq.front() < i - k + 1) dq.pop_front();

        // Maintain decreasing order
        while (!dq.empty() && nums[dq.back()] <= nums[i]) dq.pop_back();

        dq.push_back(i);

        // Window formed when i >= k-1
        if (i >= k - 1) result.push_back(nums[dq.front()]);
    }
    return result;
}

int main() {
    vector<int> a = {1, 3, -1, -3, 5, 3, 6, 7};
    auto ans = maxSlidingWindow(a, 3);
    for (int x : ans) cout << x << " ";
    cout << endl; // 3 3 5 5 6 7
    return 0;
}
