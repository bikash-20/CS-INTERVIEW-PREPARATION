// Problem: Longest Substring Without Repeating Characters (LeetCode #3)
// Difficulty: Medium
// Approach: Use a sliding window [left, right]. Track last seen index of each char
//           in a hashmap. When we see a repeat inside window, jump left past it.
// Time:  O(n), Space: O(min(n, 26))

#include <iostream>
#include <string>
#include <unordered_map>
#include <algorithm>
using namespace std;

int lengthOfLongestSubstring(string s) {
    unordered_map<char, int> lastIdx;
    int best = 0, left = 0;
    for (int right = 0; right < (int)s.size(); right++) {
        char c = s[right];
        if (lastIdx.count(c) && lastIdx[c] >= left)
            left = lastIdx[c] + 1;
        lastIdx[c] = right;
        best = max(best, right - left + 1);
    }
    return best;
}

int main() {
    cout << lengthOfLongestSubstring("abcabcbb") << endl; // 3
    cout << lengthOfLongestSubstring("bbbbb") << endl;    // 1
    cout << lengthOfLongestSubstring("pwwkew") << endl;   // 3
    return 0;
}
