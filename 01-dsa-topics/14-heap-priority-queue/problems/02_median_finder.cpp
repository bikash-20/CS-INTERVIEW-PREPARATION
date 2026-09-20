// Problem: Find Median from Data Stream (LeetCode #295)
// Difficulty: Hard
// Approach: Two heaps — maxHeap for lower half, minHeap for upper half.
//           Invariant: size(maxHeap) == size(minHeap) or size(maxHeap) = size(minHeap) + 1.
//           Median = top of maxHeap (if odd) or avg of tops (if even).
// Time:  O(log n) per addNum, O(1) findMedian. Space: O(n)

#include <iostream>
#include <queue>
using namespace std;

class MedianFinder {
    priority_queue<int> maxHeap; // lower half
    priority_queue<int, vector<int>, greater<int>> minHeap; // upper half
public:
    void addNum(int num) {
        maxHeap.push(num);
        // Balance: ensure maxHeap.top <= minHeap.top
        if (!minHeap.empty() && maxHeap.top() > minHeap.top()) {
            minHeap.push(maxHeap.top());
            maxHeap.pop();
        }
        // Rebalance sizes
        if ((int)maxHeap.size() > (int)minHeap.size() + 1) {
            minHeap.push(maxHeap.top());
            maxHeap.pop();
        } else if ((int)maxHeap.size() < (int)minHeap.size()) {
            maxHeap.push(minHeap.top());
            minHeap.pop();
        }
    }

    double findMedian() {
        if (maxHeap.size() == minHeap.size())
            return (maxHeap.top() + minHeap.top()) / 2.0;
        return maxHeap.top();
    }
};

int main() {
    MedianFinder mf;
    mf.addNum(1); mf.addNum(2);
    cout << mf.findMedian() << endl; // 1.5
    mf.addNum(3);
    cout << mf.findMedian() << endl; // 2
    return 0;
}
