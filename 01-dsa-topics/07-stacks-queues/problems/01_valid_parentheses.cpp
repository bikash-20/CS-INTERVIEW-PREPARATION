// Problem: Valid Parentheses (LeetCode #20)
// Difficulty: Easy
// Approach: Stack. Push every opening bracket. On closing, check top matches.
// Time:  O(n), Space: O(n)

#include <iostream>
#include <stack>
#include <unordered_map>
using namespace std;

bool isValid(string s) {
    stack<char> st;
    unordered_map<char, char> pairs = {
        {')', '('}, {']', '['}, {'}', '{'}
    };
    for (char c : s) {
        if (pairs.count(c)) {
            if (st.empty() || st.top() != pairs[c]) return false;
            st.pop();
        } else {
            st.push(c);
        }
    }
    return st.empty();
}

int main() {
    cout << isValid("()[]{}") << endl;   // 1
    cout << isValid("(]") << endl;       // 0
    cout << isValid("([)]") << endl;     // 0
    cout << isValid("{[]}") << endl;     // 1
    return 0;
}
