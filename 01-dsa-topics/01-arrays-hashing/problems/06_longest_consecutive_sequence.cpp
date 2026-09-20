// Problem: Longest Consecutive Sequence (LeetCode #128)
// Difficulty: Medium
// Approach: Put all elements in unordered_set. For each number, check if it's
//           the START of a sequence (i.e., num-1 not in set). Then count how
//           many consecutive numbers follow. This ensures O(n) total work.
// Time:  O(n), Space: O(n)

#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
using namespace std;

int longestConsecutive(vector<int>& nums) {
    unordered_set<int> s(nums.begin(), nums.end());
    int longest = 0;

    for (int num : s) {
        // Only start counting if num is the beginning of a sequence
        if (!s.count(num - 1)) {
            int length = 1;
            while (s.count(num + length)) length++;
            longest = max(longest, length);
        }
    }
    return longest;
}

int main() {
    vector<int> nums = {100, 4, 200, 1, 3, 2};
    cout << "Longest consecutive sequence length: "
         << longestConsecutive(nums) << endl; // 4
    return 0;
}
