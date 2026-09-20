# C++ Specific Interview Topics

## 1. Pointers vs References

| Aspect | Pointer | Reference |
|--------|---------|-----------|
| Initialization | Can be null or uninitialized | Must be initialized |
| Reassignment | Can be reassigned | Cannot be rebound |
| Memory address | Has its own address | Alias of target |
| Indirection | `*p` to dereference | Use directly |
| Const-ness | `int* const p` (const ptr) | Cannot make a reference "reseatable" |

```cpp
int a = 5;
int* p = &a; // pointer
int& r = a; // reference

*p = 10; // via pointer
r = 20; // via reference (same as a = 20)
```

**When to use:**
- References: function parameters (no null needed), range-for.
- Pointers: optional values, polymorphism, dynamic memory, data structures.

---

## 2. Virtual Functions, vtable, vptr

### vtable (virtual table)
- Each class with virtual functions has a **vtable** — array of function pointers.
- Each instance has a hidden pointer (`vptr`) pointing to its class's vtable.

```
class Base {
public:
 virtual void f(); // vtable[0] = &Base::f
 virtual void g(); // vtable[1] = &Base::g
};
class Derived : public Base {
public:
 void f() override; // vtable[0] = &Derived::f
};
```

**Cost:** one indirection per virtual call, plus one vptr per object.

### Virtual Destructor
Always make base class destructor virtual if it'll be deleted polymorphically:
```cpp
Base* b = new Derived();
delete b; // calls only Base::~Base if not virtual → memory leak
```

### Pure Virtual Function
```cpp
virtual void draw() = 0; // must be overridden
```
Makes the class **abstract** (cannot instantiate).

---

## 3. Smart Pointers (C++11+)

### `unique_ptr<T>`
- Sole ownership. Cannot copy, can move.
```cpp
std::unique_ptr<int> p = std::make_unique<int>(42);
auto p2 = std::move(p);
```

### `shared_ptr<T>`
- Shared ownership via reference count.
```cpp
std::shared_ptr<int> p = std::make_shared<int>(42);
auto p2 = p; // refcount++
```

### `weak_ptr<T>`
- Non-owning observer of a `shared_ptr`.
- Used to break circular references.
```cpp
std::weak_ptr<int> wp = p;
if (auto sp = wp.lock()) { /* still alive */ }
```

---

## 4. Move Semantics

### lvalue vs rvalue
- **lvalue:** has a name, persists beyond expression. `int x = 5;` — `x` is lvalue.
- **rvalue:** temporary, about to be destroyed. `5`, `x + y`.

### std::move
- Casts lvalue to rvalue reference, enabling move.
```cpp
std::string s = "hello";
std::string s2 = std::move(s); // s is now empty (moved-from)
```

### Move Constructor / Assignment
```cpp
class Buffer {
 int* data;
public:
 Buffer(Buffer&& other) noexcept : data(other.data) {
 other.data = nullptr; // "steal" resources
 }
 Buffer& operator=(Buffer&& other) noexcept {
 if (this != &other) {
 delete[] data;
 data = other.data;
 other.data = nullptr;
 }
 return *this;
 }
};
```

---

## 5. RAII (Resource Acquisition Is Initialization)

Tie resource lifetime to object lifetime:
- Acquire resource in constructor.
- Release in destructor.
- Even if exceptions are thrown, destructor runs.

```cpp
class FileHandle {
 FILE* f;
public:
 FileHandle(const char* name) : f(fopen(name, "r")) {}
 ~FileHandle() { if (f) fclose(f); }
};
```

---

## 6. Rule of Five / Rule of Zero

### Rule of Five
If you define any of: destructor, copy ctor, copy assignment, move ctor, move assignment → define all five.

### Rule of Zero
Prefer to design classes so they don't need any of these (use smart pointers and STL containers).

---

## 7. const Correctness

```cpp
int getSize() const; // member fn won't modify state
const int* p; // pointer to const int
int* const p; // const pointer to int
const int* const p; // both const
```

**`mutable`** — allows modification of a member even in const methods (e.g., caching, mutex).

---

## 8. Templates

### Function Template
```cpp
template <typename T>
T max(T a, T b) { return a > b ? a : b; }
```

### Class Template
```cpp
template <typename T>
class Stack {
 std::vector<T> data;
public:
 void push(const T& v) { data.push_back(v); }
 T pop() { T v = data.back(); data.pop_back(); return v; }
};
```

### Template Specialization
```cpp
template <> class Stack<bool> { /* bit-packed impl */ };
```

### Variadic Templates (C++11+)
```cpp
template <typename... Args>
void print(Args... args) {
 (std::cout << ... << args) << '\n'; // C++17 fold
}
```

---

## 9. STL Containers — Quick Reference

| Container | Underlying | Access | Insert/Delete |
|-----------|-----------|--------|---------------|
| `vector` | Dynamic array | O(1) random | O(1) end / O(n) middle |
| `deque` | Doubly-ended queue | O(1) random | O(1) front/back |
| `list` | Doubly linked list | O(n) | O(1) anywhere |
| `stack` (adapter) | deque/list | top only | O(1) push/pop |
| `queue` (adapter) | deque | front/back | O(1) push/pop |
| `priority_queue` | vector+heap | top only | O(log n) |
| `set` | Red-black tree | O(log n) | O(log n) |
| `map` | Red-black tree | O(log n) | O(log n) |
| `unordered_set` | hash table | O(1) avg | O(1) avg |
| `unordered_map` | hash table | O(1) avg | O(1) avg |

---

## 10. Memory Layout of an Object

```
| vptr (8 bytes, if polymorphic) |
| member1 |
| member2 |
| ... |
| padding |
```

For derived class:
```
[ Base subobject ][ Derived members ]
```

---

## 11. Top Interview Q&A

**Q: Why is `virtual` destructor important?**
Without it, `delete basePtr` (where basePtr points to derived) calls only Base::~Base, leaking Derived's resources.

**Q: When to use `unique_ptr` vs `shared_ptr`?**
Default to `unique_ptr` (cheap, clear ownership). Use `shared_ptr` only when truly shared.

**Q: What is the slicing problem?**
Passing a Derived object by value to a function taking Base — only Base part is copied; Derived members are lost.

**Q: What's the difference between `new` and `malloc`?**
`new` calls constructor; throws on failure; returns typed pointer.
`malloc` returns void*; doesn't call constructor; returns NULL on failure.

**Q: What is `nullptr`?**
Typeless null pointer literal (C++11+). Replaces `NULL` and `0`.

**Q: Difference between `struct` and `class` in C++?**
Default access: `struct` → public, `class` → private. Otherwise identical.

**Q: What is `explicit`?**
Prevents implicit conversions in single-arg constructors.

**Q: Difference between deep copy and shallow copy?**
Shallow: copies pointers (shared data). Deep: allocates new memory and copies values.

**Q: What is a virtual function call in constructor?**
During base constructor, object's type is "currently being constructed as base", so virtual calls resolve to base's version. Avoid this.
