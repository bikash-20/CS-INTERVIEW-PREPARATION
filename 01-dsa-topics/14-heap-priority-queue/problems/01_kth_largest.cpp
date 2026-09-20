// Problem: Kth Largest Element in an Array (LeetCode #215)
// Difficulty: Medium
// Approach: Min-heap of size k. For each num, push; if size > k, pop smallest.
//           Top of heap is the k-th largest overall.
// Time:  O(n log k), Space: O(k)

#include <iostream>
#include <vector>
#include <queue>
using namespace std;

int findKthLargest(vector<int>& nums, int k) {
    priority_queue<int, vector<int>, greater<int>> minHeap;
    for (int x : nums) {
        minHeap.push(x);
        if ((int)minHeap.size() > k) minHeap.pop();
    }
    return minHeap.top();
}

int main() {
    vector<int> a = {3, 2, 1, 5, 6, 4};
    cout << findKthLargest(a, 2) << endl; // 5
    return 0;
}
