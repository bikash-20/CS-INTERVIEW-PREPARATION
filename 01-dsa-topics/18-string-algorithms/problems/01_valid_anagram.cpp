// Problem: Valid Anagram (LeetCode #242)
// Difficulty: Easy
// Approach: Count character frequencies in both strings; compare.
// Time:  O(n), Space: O(1) (26 chars)

#include <iostream>
#include <string>
#include <vector>
using namespace std;

bool isAnagram(string s, string t) {
    if (s.size() != t.size()) return false;
    vector<int> count(26, 0);
    for (int i = 0; i < (int)s.size(); i++) {
        count[s[i] - 'a']++;
        count[t[i] - 'a']--;
    }
    for (int c : count) if (c != 0) return false;
    return true;
}

int main() {
    cout << isAnagram("anagram", "nagaram") << endl; // 1
    cout << isAnagram("rat", "car") << endl;          // 0
    return 0;
}
