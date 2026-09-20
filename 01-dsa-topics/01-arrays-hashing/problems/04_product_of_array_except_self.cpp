// Problem: Product of Array Except Self (LeetCode #238)
// Difficulty: Medium
// Approach: Build prefix product array (left[i] = product of elements left of i)
//           and suffix product array (right[i] = product of elements right of i).
//           Answer[i] = left[i] * right[i]. We can do it in O(1) extra space
//           by using the output array for prefix, then multiplying by suffix on the fly.
// Time:  O(n), Space: O(1) (excluding output array)

#include <iostream>
#include <vector>
using namespace std;

vector<int> productExceptSelf(vector<int>& nums) {
    int n = nums.size();
    vector<int> ans(n, 1);

    // Prefix: ans[i] = product of elements to the left of i
    int prefix = 1;
    for (int i = 0; i < n; i++) {
        ans[i] = prefix;
        prefix *= nums[i];
    }

    // Suffix: multiply ans[i] by product of elements to the right
    int suffix = 1;
    for (int i = n - 1; i >= 0; i--) {
        ans[i] *= suffix;
        suffix *= nums[i];
    }

    return ans;
}

int main() {
    vector<int> nums = {1, 2, 3, 4};
    vector<int> ans = productExceptSelf(nums);
    for (int x : ans) cout << x << " ";
    cout << endl; // 24 12 8 6
    return 0;
}
