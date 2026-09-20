// Problem: Longest Palindromic Substring (LeetCode #5)
// Difficulty: Medium
// Approach: Expand around center. For each center (each char + between-chars),
//           expand outward while chars match. Track longest.
// Time:  O(n^2), Space: O(1)

#include <iostream>
#include <string>
using namespace std;

string longestPalindrome(string s) {
    int n = s.size();
    int bestL = 0, bestR = 0;

    auto expand = [&](int l, int r) {
        while (l >= 0 && r < n && s[l] == s[r]) {
            if (r - l > bestR - bestL) { bestL = l; bestR = r; }
            l--; r++;
        }
    };

    for (int i = 0; i < n; i++) {
        expand(i, i);     // odd-length center
        expand(i, i + 1); // even-length center
    }
    return s.substr(bestL, bestR - bestL + 1);
}

int main() {
    cout << longestPalindrome("babad") << endl; // "bab" or "aba"
    cout << longestPalindrome("cbbd") << endl;  // "bb"
    return 0;
}
