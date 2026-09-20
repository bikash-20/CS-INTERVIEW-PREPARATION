// Problem: Pow(x, n) (LeetCode #50)
// Difficulty: Medium
// Approach: Fast exponentiation. Handle negative n by computing 1/x^-n.
//           Be careful with INT_MIN.
// Time:  O(log n), Space: O(1)

#include <iostream>
using namespace std;

double myPow(double x, long long n) {
    if (n == 0) return 1;
    if (n < 0) { x = 1 / x; n = -n; }
    double result = 1;
    while (n > 0) {
        if (n & 1) result *= x;
        x *= x;
        n >>= 1;
    }
    return result;
}

int main() {
    cout << myPow(2.0, 10) << endl;   // 1024
    cout << myPow(2.1, 3) << endl;    // 9.261
    cout << myPow(2.0, -2) << endl;   // 0.25
    return 0;
}
