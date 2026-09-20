# Math & Number Theory — Theory

## Common Topics

### 1. GCD (Greatest Common Divisor)
**Euclidean algorithm**: `gcd(a, b) = gcd(b, a % b)`.
```cpp
int gcd(int a, int b) { return b ? gcd(b, a % b) : a; }
```
LCM: `lcm(a, b) = a / gcd(a, b) * b`.

### 2. Prime Numbers
- **Sieve of Eratosthenes**: O(n log log n) to find all primes up to n.
```cpp
vector<bool> sieve(int n) {
 vector<bool> isPrime(n + 1, true);
 isPrime[0] = isPrime[1] = false;
 for (int i = 2; i * i <= n; i++)
 if (isPrime[i])
 for (int j = i * i; j <= n; j += i)
 isPrime[j] = false;
 return isPrime;
}
```

### 3. Modular Arithmetic
- `(a + b) % m = ((a % m) + (b % m)) % m`
- `(a * b) % m = ((a % m) * (b % m)) % m`
- **Modular exponentiation** (fast pow): `pow(x, n) % m` in O(log n).

### 4. Factorials, Permutations, Combinations
- `n!` — `n * (n-1) * ... * 1`
- `P(n, k) = n! / (n-k)!`
- `C(n, k) = n! / (k! * (n-k)!)`
- Pascal's triangle: `C(n, k) = C(n-1, k-1) + C(n-1, k)`.

### 5. Fast Exponentiation
```cpp
long long fastPow(long long base, long long exp, long long mod) {
 long long result = 1;
 base %= mod;
 while (exp > 0) {
 if (exp & 1) result = result * base % mod;
 base = base * base % mod;
 exp >>= 1;
 }
 return result;
}
```

### 6. Modular Inverse (Fermat's little theorem)
When `m` is prime: `a^(-1) ≡ a^(m-2) (mod m)`.
```cpp
long long modInverse(long long a, long long m) {
 return fastPow(a, m - 2, m);
}
```

### 7. Matrix Exponentiation
For linear recurrences (Fibonacci), compute matrix^n in O(k³ log n) where k is matrix size.

## Top Problems

| # | Problem | Difficulty |
|---|---------|-----------|
| 1 | Reverse Integer | Easy |
| 2 | Palindrome Number | Easy |
| 3 | Pow(x, n) | Medium |
| 4 | Sqrt(x) | Easy |
| 5 | Happy Number | Easy |
| 6 | Plus One | Easy |
| 7 | Count Primes | Medium |
| 8 | Fraction to Recurring Decimal | Medium |
| 9 | Multiply Strings | Medium |
| 10 | Integer to Roman | Medium |
| 11 | Largest Number | Medium |
| 12 | Excel Sheet Column Number | Easy |
