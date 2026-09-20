// Problem: Contains Duplicate (LeetCode #217)
// Difficulty: Easy
// Approach: Insert elements into unordered_set. If size grows slower than
//           expected, we found a duplicate. Even simpler: if (set.size() != n)
//           return true.
// Time:  O(n), Space: O(n)

#include <iostream>
#include <vector>
#include <unordered_set>
using namespace std;

bool containsDuplicate(vector<int>& nums) {
    unordered_set<int> seen;
    for (int x : nums) {
        if (seen.count(x)) return true;
        seen.insert(x);
    }
    return false;
}

int main() {
    vector<int> a = {1, 2, 3, 1};
    cout << (containsDuplicate(a) ? "true" : "false") << endl; // true

    vector<int> b = {1, 2, 3, 4};
    cout << (containsDuplicate(b) ? "true" : "false") << endl; // false
    return 0;
}
