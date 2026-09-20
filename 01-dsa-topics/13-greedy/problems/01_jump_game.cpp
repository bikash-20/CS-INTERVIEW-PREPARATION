// Problem: Jump Game (LeetCode #55)
// Difficulty: Medium
// Approach: Track the farthest index reachable. If at any index i, i > farthest,
//           we can't reach i → fail. Else update farthest = max(farthest, i + nums[i]).
// Time:  O(n), Space: O(1)

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

bool canJump(vector<int>& nums) {
    int farthest = 0;
    for (int i = 0; i < (int)nums.size(); i++) {
        if (i > farthest) return false;
        farthest = max(farthest, i + nums[i]);
    }
    return true;
}

int main() {
    vector<int> a = {2, 3, 1, 1, 4};
    cout << canJump(a) << endl; // 1

    vector<int> b = {3, 2, 1, 0, 4};
    cout << canJump(b) << endl; // 0
    return 0;
}
