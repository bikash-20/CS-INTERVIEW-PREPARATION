// Problem: Group Anagrams (LeetCode #49)
// Difficulty: Medium
// Approach: For each string, sort its characters → use that as key in a map.
//           All anagrams share the same sorted form, so they group together.
// Time:  O(n * k log k) where k is max string length
// Space: O(n * k)

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
using namespace std;

vector<vector<string>> groupAnagrams(vector<string>& strs) {
    unordered_map<string, vector<string>> groups;
    for (const string& s : strs) {
        string key = s;
        sort(key.begin(), key.end());
        groups[key].push_back(s);
    }

    vector<vector<string>> result;
    for (auto& [key, vec] : groups) result.push_back(vec);
    return result;
}

int main() {
    vector<string> strs = {"eat", "tea", "tan", "ate", "nat", "bat"};
    auto groups = groupAnagrams(strs);
    for (auto& g : groups) {
        cout << "[ ";
        for (auto& s : g) cout << s << " ";
        cout << "]\n";
    }
    // Output: [ eat tea ate ] [ tan nat ] [ bat ]  (order may vary)
    return 0;
}
