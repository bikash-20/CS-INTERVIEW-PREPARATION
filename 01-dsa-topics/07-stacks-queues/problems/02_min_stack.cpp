// Problem: Min Stack (LeetCode #155)
// Difficulty: Medium
// Approach: Two stacks — main and minStack. minStack.top() is the min so far.
// Time:  O(1) for all ops, Space: O(n)

#include <iostream>
#include <stack>
using namespace std;

class MinStack {
    stack<int> st, minSt;
public:
    void push(int val) {
        st.push(val);
        if (minSt.empty() || val <= minSt.top()) minSt.push(val);
    }
    void pop() {
        if (st.top() == minSt.top()) minSt.pop();
        st.pop();
    }
    int top() { return st.top(); }
    int getMin() { return minSt.top(); }
};

int main() {
    MinStack ms;
    ms.push(-2); ms.push(0); ms.push(-3);
    cout << ms.getMin() << endl; // -3
    ms.pop();
    cout << ms.top() << endl;    // 0
    cout << ms.getMin() << endl; // -2
    return 0;
}
