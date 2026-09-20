// Problem: Koko Eating Bananas (LeetCode #875)
// Difficulty: Medium
// Approach: Binary search on answer (speed k). Predicate: can Koko finish
//           within h hours at speed k? Monotonic: if speed k works, any
//           larger speed also works.
// Time:  O(n log(max)), Space: O(1)

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

bool canFinish(vector<int>& piles, int k, int h) {
    int hours = 0;
    for (int p : piles) {
        hours += (p + k - 1) / k; // ceil(p / k)
        if (hours > h) return false;
    }
    return true;
}

int minEatingSpeed(vector<int>& piles, int h) {
    int left = 1, right = *max_element(piles.begin(), piles.end());
    int best = right;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (canFinish(piles, mid, h)) {
            best = mid;
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    return best;
}

int main() {
    vector<int> piles = {3, 6, 7, 11};
    int h = 8;
    cout << minEatingSpeed(piles, h) << endl; // 4
    return 0;
}
