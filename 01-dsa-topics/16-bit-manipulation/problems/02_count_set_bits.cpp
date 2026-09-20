// Problem: Number of 1 Bits (LeetCode #191)
// Difficulty: Easy
// Approach: Brian Kernighan — n & (n-1) clears lowest set bit. Count iterations.
// Time:  O(popcount), Space: O(1)

#include <iostream>
using namespace std;

int hammingWeight(int n) {
    int count = 0;
    while (n) {
        n &= n - 1;
        count++;
    }
    return count;
}

int main() {
    cout << hammingWeight(11) << endl; // 3 (1011)
    cout << hammingWeight(128) << endl; // 1
    return 0;
}
