# Bit Manipulation — Theory

## Why Bits?

- **Speed**: Bit operations are CPU-level fast.
- **Memory**: Compact storage (bitmask for sets).
- **Math**: Elegant solutions to some problems.

## Basic Operations

| Operation | Operator | Example |
|-----------|----------|---------|
| AND | `&` | `5 & 3 = 1` (`101 & 011`) |
| OR | `\|` | `5 \| 3 = 7` |
| XOR | `^` | `5 ^ 3 = 6` (`101 ^ 011 = 110`) |
| NOT | `~` | `~5 = -6` (two's complement) |
| Left shift | `<<` | `5 << 1 = 10` (`101 → 1010`) |
| Right shift | `>>` | `5 >> 1 = 2` |

## Useful Tricks

### 1. Check if i-th bit is set
```cpp
bool isSet = (n >> i) & 1;
```

### 2. Set i-th bit
```cpp
n | (1 << i);
```

### 3. Clear i-th bit
```cpp
n & ~(1 << i);
```

### 4. Toggle i-th bit
```cpp
n ^ (1 << i);
```

### 5. Check if power of 2
```cpp
bool isPow2 = (n > 0) && ((n & (n - 1)) == 0);
```

### 6. Count set bits (Brian Kernighan)
```cpp
int count = 0;
while (n) { n &= n - 1; count++; }
```

### 7. Get lowest set bit
```cpp
int lowbit = n & -n;
```

### 8. XOR Properties
- `a ^ a = 0`
- `a ^ 0 = a`
- XOR of all elements of an array where every element appears twice except one gives the unique one.

## Top Problems

| # | Problem | Difficulty |
|---|---------|-----------|
| 1 | Single Number | Easy |
| 2 | Number of 1 Bits | Easy |
| 3 | Counting Bits | Easy |
| 4 | Reverse Bits | Easy |
| 5 | Missing Number | Easy |
| 6 | Sum of Two Integers (without +/-) | Medium |
| 7 | Single Number II | Medium |
| 8 | Bitwise AND of Numbers Range | Medium |
| 9 | Maximum XOR of Two Numbers | Medium |
| 10 | Power of Two | Easy |
