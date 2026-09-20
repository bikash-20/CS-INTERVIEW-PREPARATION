// Problem: Top K Frequent Elements (LeetCode #347)
// Difficulty: Medium
// Approach: Count frequencies using hash map. Then use bucket sort:
//           bucket[i] = list of elements with frequency i. Output bucket[n..n-k+1].
// Time:  O(n), Space: O(n)

#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

vector<int> topKFrequent(vector<int>& nums, int k) {
    unordered_map<int, int> freq;
    for (int x : nums) freq[x]++;

    int n = nums.size();
    vector<vector<int>> bucket(n + 1);
    for (auto& [num, count] : freq) {
        bucket[count].push_back(num);
    }

    vector<int> result;
    for (int i = n; i >= 0 && (int)result.size() < k; i--) {
        for (int num : bucket[i]) {
            result.push_back(num);
            if ((int)result.size() == k) break;
        }
    }
    return result;
}

int main() {
    vector<int> nums = {1, 1, 1, 2, 2, 3};
    int k = 2;
    vector<int> ans = topKFrequent(nums, k);
    for (int x : ans) cout << x << " ";
    cout << endl; // 1 2 (order may vary)
    return 0;
}
