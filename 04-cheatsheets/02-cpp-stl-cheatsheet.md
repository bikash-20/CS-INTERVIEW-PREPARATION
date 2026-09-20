# C++ STL Cheatsheet

Quick reference for the most-used STL containers and algorithms.

## Containers

### `std::vector<T>`
```cpp
#include <vector>
vector<int> v = {1, 2, 3};
v.push_back(4); // add to end
v.pop_back(); // remove last
v.size(); // size
v.empty(); // is empty?
v[i]; // access (no bounds check)
v.at(i); // access (with bounds check)
v.insert(v.begin() + i, x); // insert at position
v.erase(v.begin() + i); // erase at position
v.clear(); // remove all
sort(v.begin(), v.end()); // sort
reverse(v.begin(), v.end()); // reverse
```

### `std::string`
```cpp
string s = "hello";
s += " world"; // concat
s.substr(pos, len); // substring
s.find("world"); // find substring (returns position or npos)
s.length(); s.size();
s[0]; // char access
stoi(s); // string to int
to_string(42); // int to string
```

### `std::pair<T1, T2>` / `std::tuple`
```cpp
pair<int, string> p = {1, "hello"};
p.first; p.second;

tuple<int, string, double> t = {1, "hi", 3.14};
get<0>(t); get<1>(t); get<2>(t);
// C++17 structured bindings:
auto [a, b, c] = make_tuple(1, "hi", 3.14);
auto& [x, y] = p;
```

### `std::stack<T>` / `std::queue<T>` / `std::deque<T>`
```cpp
stack<int> st;
st.push(x); st.top(); st.pop(); st.empty();

queue<int> q;
q.push(x); q.front(); q.back(); q.pop(); q.empty();

deque<int> dq;
dq.push_back(x); dq.push_front(x);
dq.front(); dq.back(); dq[i];
```

### `std::priority_queue<T>`
```cpp
// Max-heap by default
priority_queue<int> maxHeap;
maxHeap.push(5); maxHeap.top(); maxHeap.pop();

// Min-heap
priority_queue<int, vector<int>, greater<int>> minHeap;
```

### `std::set<T>` / `std::multiset<T>`
```cpp
// Sorted unique elements (BST / red-black tree)
set<int> s;
s.insert(5); // O(log n)
s.count(5); // 0 or 1
s.find(5); // iterator or end()
s.erase(5); // by value
s.lower_bound(x); s.upper_bound(x);
*next(s.begin(), k); // kth smallest

multiset<int> ms; // allows duplicates
ms.count(5); // number of 5s
```

### `std::map<K, V>` / `std::multimap<K, V>`
```cpp
map<string, int> m;
m["apple"] = 3;
m.count("apple"); // 0 or 1
m.find("apple");
m.erase("apple");
// Iteration in sorted key order
for (auto& [key, val] : m) cout << key << " " << val << "\n";
```

### `std::unordered_set<T>` / `std::unordered_map<K, V>`
```cpp
unordered_set<int> us;
us.insert(5); us.count(5); us.find(5); us.erase(5);
// O(1) average, no order

unordered_map<string, int> um;
um["x"] = 1;
um.count("x"); um.find("x"); um.erase("x");
```

### `std::bitset<N>`
```cpp
bitset<8> b;
b[0] = 1;
b.set(2); b.reset(2); b.flip(2);
b.count(); b.any(); b.none(); b.all();
b.to_ulong();
```

## Iterators

```cpp
vector<int> v = {1, 2, 3};
auto it = v.begin(); // begin
auto it_end = v.end(); // one-past-end

*it; // dereference
++it; --it; // advance
it += n;
it1 - it2; // distance
advance(it, n); // advance iterator by n
distance(it1, it2);

// Reverse iterators
auto rit = v.rbegin(); // reverse begin
auto rit_end = v.rend();

// C++20 ranges
sort(v.begin(), v.end());
ranges::sort(v); // (C++20)
```

## Algorithms (`<algorithm>`)

### Sorting
```cpp
sort(v.begin(), v.end()); // ascending
sort(v.begin(), v.end(), greater<int>()); // descending
sort(v.begin(), v.end(), [](int a, int b){ return a > b; }); // custom comparator
stable_sort(v.begin(), v.end());
partial_sort(v.begin(), v.begin() + k, v.end()); // top k
nth_element(v.begin(), v.begin() + k, v.end()); // partition around kth
```

### Searching
```cpp
binary_search(v.begin(), v.end(), target);
lower_bound(v.begin(), v.end(), target); // first >= target
upper_bound(v.begin(), v.end(), target); // first > target
equal_range(v.begin(), v.end(), target);

find(v.begin(), v.end(), target);
find_if(v.begin(), v.end(), [](int x){ return x > 5; });
```

### Min/Max
```cpp
*min_element(v.begin(), v.end());
*max_element(v.begin(), v.end());
min(a, b); max(a, b);
min({a, b, c, d}); // initializer list
clamp(x, low, high); // C++17
```

### Permutations
```cpp
next_permutation(v.begin(), v.end()); // next lex permutation
prev_permutation(v.begin(), v.end());
```

### Heap Operations (on any container)
```cpp
make_heap(v.begin(), v.end()); // max-heap
push_heap(v.begin(), v.end()); // add to heap
pop_heap(v.begin(), v.end()); // move largest to end
sort_heap(v.begin(), v.end()); // sort the heap
```

### Other Useful
```cpp
reverse(v.begin(), v.end());
rotate(v.begin(), v.begin() + k, v.end());
unique(v.begin(), v.end()); // remove consecutive duplicates
count(v.begin(), v.end(), x);
count_if(v.begin(), v.end(), pred);
accumulate(v.begin(), v.end(), 0); // sum
iota(v.begin(), v.end(), 1); // fill with 1, 2, 3, ...
fill(v.begin(), v.end(), 0);
copy(src.begin(), src.end(), dst.begin());
transform(v.begin(), v.end(), result.begin(), [](int x){ return x * 2; });
all_of, any_of, none_of
```

## Numeric (`<numeric>`)

```cpp
accumulate(v.begin(), v.end(), 0); // sum
reduce(v.begin(), v.end(), 0); // parallel-friendly
inner_product(a.begin(), a.end(), b.begin(), 0); // dot product
partial_sum(v.begin(), v.end(), result.begin()); // prefix sums
gcd(a, b); // C++17
lcm(a, b); // C++17
```

## Useful Utilities

```cpp
#include <utility>
swap(a, b); move(a); // std::move
pair<int, string> p = make_pair(1, "x");
tuple<int, string, double> t = make_tuple(1, "x", 3.14);
```

## Smart Pointers

```cpp
#include <memory>
auto p = make_unique<int>(42); // unique_ptr
auto sp = make_shared<int>(42); // shared_ptr
weak_ptr<int> wp = sp; // non-owning observer
sp.reset(); // release
```

## Common Idioms

### Sort + Custom Comparator
```cpp
sort(arr.begin(), arr.end(), [](auto& a, auto& b){ return a.second < b.second; });
```

### Min-Heap with Custom Comparator
```cpp
priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
```

### Range-Based For
```cpp
for (auto& x : vec) cout << x;
for (const auto& [k, v] : map) cout << k << " " << v;
```

### Emplace vs Push
```cpp
v.emplace_back(1, 2, 3); // construct in-place
v.push_back({1, 2, 3}); // construct then copy
```

## Time Complexities (STL)

| Container | Insert | Find | Erase | Notes |
|-----------|--------|------|-------|-------|
| `vector` | O(1)* / O(n) | O(n) | O(n) | *amortized at end |
| `deque` | O(1) ends / O(n) mid | O(n) | O(n) | |
| `list` | O(1) | O(n) | O(1) given iterator | |
| `set` / `map` | O(log n) | O(log n) | O(log n) | |
| `unordered_set` / `unordered_map` | O(1) avg | O(1) avg | O(1) avg | |
| `priority_queue` | O(log n) | O(1) for top | O(log n) | |
