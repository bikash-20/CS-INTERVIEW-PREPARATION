// Problem: Count Primes (LeetCode #204)
// Difficulty: Medium
// Approach: Sieve of Eratosthenes. Mark multiples of each prime starting from p*p.
// Time:  O(n log log n), Space: O(n)

#include <iostream>
#include <vector>
using namespace std;

int countPrimes(int n) {
    if (n <= 2) return 0;
    vector<bool> isPrime(n, true);
    isPrime[0] = isPrime[1] = false;
    for (int i = 2; (long long)i * i < n; i++) {
        if (isPrime[i]) {
            for (int j = i * i; j < n; j += i)
                isPrime[j] = false;
        }
    }
    int count = 0;
    for (int i = 2; i < n; i++) if (isPrime[i]) count++;
    return count;
}

int main() {
    cout << countPrimes(10) << endl;  // 4 (2, 3, 5, 7)
    cout << countPrimes(0) << endl;   // 0
    cout << countPrimes(1) << endl;   // 0
    return 0;
}
