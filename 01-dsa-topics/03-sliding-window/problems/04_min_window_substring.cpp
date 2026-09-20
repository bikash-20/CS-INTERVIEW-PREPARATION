// Problem: Minimum Window Substring (LeetCode #76)
// Difficulty: Hard
// Approach: Two hash maps — required (counts of chars in t) and window (counts in current window).
//           Expand right until window contains all chars of t. Then shrink left
//           as much as possible while still valid. Track min length window.
// Time:  O(n), Space: O(1) (limited alphabet)

#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

string minWindow(string s, string t) {
    if (s.empty() || t.empty()) return "";

    unordered_map<char, int> required, window;
    for (char c : t) required[c]++;

    int have = 0, need = required.size();
    int left = 0, bestLen = INT_MAX, bestLeft = 0;

    for (int right = 0; right < (int)s.size(); right++) {
        char c = s[right];
        window[c]++;
        if (required.count(c) && window[c] == required[c]) have++;

        while (have == need) {
            // Update answer
            if (right - left + 1 < bestLen) {
                bestLen = right - left + 1;
                bestLeft = left;
            }
            // Try to shrink
            char cl = s[left];
            window[cl]--;
            if (required.count(cl) && window[cl] < required[cl]) have--;
            left++;
        }
    }
    return bestLen == INT_MAX ? "" : s.substr(bestLeft, bestLen);
}

int main() {
    string s = "ADOBECODEBANC", t = "ABC";
    cout << minWindow(s, t) << endl; // "BANC"
    return 0;
}
