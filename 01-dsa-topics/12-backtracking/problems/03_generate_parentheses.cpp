// Problem: Generate Parentheses (LeetCode #22)
// Difficulty: Medium
// Approach: Backtrack. Add '(' if open < n. Add ')' if close < open.
// Time:  O(Catalan(n)) ~ O(4^n / sqrt(n)), Space: O(n)

#include <iostream>
#include <vector>
#include <string>
using namespace std;

void backtrack(int n, int open, int close, string& path, vector<string>& res) {
    if ((int)path.size() == 2 * n) {
        res.push_back(path);
        return;
    }
    if (open < n) {
        path.push_back('(');
        backtrack(n, open + 1, close, path, res);
        path.pop_back();
    }
    if (close < open) {
        path.push_back(')');
        backtrack(n, open, close + 1, path, res);
        path.pop_back();
    }
}

vector<string> generateParenthesis(int n) {
    vector<string> res;
    string path;
    backtrack(n, 0, 0, path, res);
    return res;
}

int main() {
    auto ps = generateParenthesis(3);
    for (auto& s : ps) cout << s << " ";
    cout << endl;
    // ((())) (()()) (())() ()(()) ()()()
    return 0;
}
