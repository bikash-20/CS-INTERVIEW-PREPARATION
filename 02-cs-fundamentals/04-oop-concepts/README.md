# Object-Oriented Programming — Comprehensive Guide

> **OOP is the language you use to think about programs** before you write them. Even when you write functional or procedural code, you're often modeling with OOP concepts in your head. This guide covers the **4 pillars, relationships, design heuristics, and 35+ Q&A**.

---

## Table of Contents

1. [OOP in a Nutshell](#1-oop-in-a-nutshell)
2. [Encapsulation — Hide the State](#2-encapsulation--hide-the-state)
3. [Abstraction — Show the What, Hide the How](#3-abstraction--show-the-what-hide-the-how)
4. [Inheritance — Reuse Through "Is-A"](#4-inheritance--reuse-through-is-a)
5. [Polymorphism — One Interface, Many Behaviors](#5-polymorphism--one-interface-many-behaviors)
6. [Association, Aggregation, Composition — The Three Relationships](#6-association-aggregation-composition--the-three-relationships)
7. [Abstract Classes vs Interfaces](#7-abstract-classes-vs-interfaces)
8. [Coupling, Cohesion, and Other Design Heuristics](#8-coupling-cohesion-and-other-design-heuristics)
9. [Common Pitfalls — Slicing, Diamond, Fragile Base Class](#9-common-pitfalls--slicing-diamond-fragile-base-class)
10. [SOLID Overview (Deeper Coverage in 06-)](#10-solid-overview-deeper-coverage-in-06-)
11. [Design Patterns Overview](#11-design-patterns-overview)
12. [UML Class Diagram — Interview Quick Reference](#12-uml-class-diagram--interview-quick-reference)
13. [Top Interview Q&A (35 entries)](#13-top-interview-qa-35-entries)

---

## 1. OOP in a Nutshell

Object-Oriented Programming models software as a collection of **objects** that **encapsulate state (fields)** and **behavior (methods)**, communicate by **messages** (method calls), and are organized by **classes**.

### Why OOP?

- **Modularity:** files of code become independent classes.
- **Reuse:** composition and inheritance.
- **Flexibility:** polymorphism lets you swap behavior.
- **Modeling:** real-world entities map naturally.

### The 4 Pillars (mnemonic: A PIE in an Encapsulation)

| Pillar | Question It Answers |
|--------|---------------------|
| **Encapsulation** | How do I keep state private and safe? |
| **Abstraction** | How do I expose a clean interface? |
| **Inheritance** | How do I reuse code across classes? |
| **Polymorphism** | How do I call the right behavior at runtime? |

---

## 2. Encapsulation — Hide the State

> *Bundle data + methods; hide internals; expose via a controlled API.*

### Why encapsulate?

- **Invariant protection** — prevent outside code from putting the object in an invalid state.
- **Free to change internals** — public API stays the same.
- **Easier testing** — narrow surface area.

### C++ Example

```cpp
class Account {
private:
    double balance_;          // hidden state
    std::string owner_;
public:
    Account(std::string owner, double initial)
      : owner_(std::move(owner)), balance_(initial) {}

    void deposit(double amount) {
        if (amount <= 0) throw std::invalid_argument("deposit must be positive");
        balance_ += amount;
    }

    void withdraw(double amount) {
        if (amount <= 0) throw std::invalid_argument("withdraw must be positive");
        if (amount > balance_) throw std::runtime_error("insufficient funds");
        balance_ -= amount;
    }

    double balance() const { return balance_; }   // read-only access
};
```

### Access Modifiers

| Modifier | Where Accessible |
|----------|------------------|
| `private` | The class itself + friends |
| `protected` | The class + its subclasses + friends |
| `public` | Everywhere |

> **Best practice:** members private by default; expose only what's needed via `public` methods. Avoid getter/setter pairs for everything — they leak internal representation.

### Encapsulation vs Information Hiding

- **Encapsulation** = bundling state + behavior together.
- **Information hiding** = hiding implementation details behind an interface.

Good OOP combines both.

---

## 3. Abstraction — Show the What, Hide the How

> *Show only the essential characteristics; hide the background details.*

### Pure Abstraction in C++

```cpp
class Shape {
public:
    virtual double area() const = 0;   // pure virtual — no implementation here
    virtual void   draw() const = 0;   // abstract interface
    virtual ~Shape() = default;
};

class Circle : public Shape {
    double r_;
public:
    explicit Circle(double r) : r_(r) {}
    double area() const override { return 3.14159265358979 * r_ * r_; }
    void   draw() const override { /* ... */ }
};
```

### Abstract Class

A class with at least one **pure virtual function**. Cannot be instantiated. May have data members and method implementations.

### Interfaces in C++

C++ has no `interface` keyword. Conventionally:

```cpp
class IDrawable {
public:
    virtual void draw() const = 0;
    virtual ~IDrawable() = default;
};

class ISerializable {
public:
    virtual std::string serialize() const = 0;
    virtual ~ISerializable() = default;
};
```

Use the **`I`-prefix naming convention** and only pure virtual functions + a virtual destructor.

### Abstraction vs Encapsulation

| Abstraction | Encapsulation |
|------------|---------------|
| "What" the object exposes | "How" the internals are protected |
| Reduces complexity for users | Reduces coupling to internals |
| Achieved via interfaces / abstract classes | Achieved via access specifiers |

---

## 4. Inheritance — Reuse Through "Is-A"

> *A derived class inherits members from a base class to **reuse and extend** behavior.*

### Basic C++ Inheritance

```cpp
class Animal {                   // base / parent / super
public:
    void eat() { std::cout << "eating\n"; }
};
class Dog : public Animal {      // derived / child / sub
public:
    void bark() { std::cout << "woof\n"; }
};

Dog d;
d.eat();    // inherited
d.bark();   // own
```

### Inheritance Types

| Type | Syntax in C++ | Notes |
|------|---------------|-------|
| **public** | `class D : public B` | Public members stay public. Most common. |
| **protected** | `class D : protected B` | Public & protected become protected. Rare. |
| **private** | `class D : private B` | All inherited become private. Used for "implemented in terms of". |

### Inheritance vs Composition (the big debate)

> **Favor composition over inheritance.** Inheritance is a strong coupling — change a base class, every derived class is at risk ("fragile base class").

| Use Inheritance When | Use Composition When |
|----------------------|----------------------|
| Clear "is-a" relationship (Dog is an Animal) | "Has-a" relationship (Car has an Engine) |
| Need polymorphic substitution | Behavior might change at runtime |
| Stable, deeply agreed hierarchy | Behavior should be swappable / testable |

### Multiple Inheritance & the Diamond Problem

```cpp
class A { public: int x; };
class B : public A {};
class C : public A {};
class D : public B, public C {};   // ambiguous A::x
```

Fix with **virtual inheritance**:

```cpp
class B : virtual public A {};
class C : virtual public A {};
class D : public B, public C {};   // OK, only one A subobject
```

### Interfaces (Multiple) in C++

```cpp
class Dog : public IRunnable, public IBarkable { /* ... */ };
```
This avoids the diamond problem because interfaces typically have no data members.

---

## 5. Polymorphism — One Interface, Many Behaviors

> *Same call, different behavior depending on the actual object's type.*

### Compile-Time (Static) Polymorphism

Achieved via:

- **Function overloading** — same name, different parameter types.
- **Operator overloading.**
- **Templates** — same template, different types.

```cpp
template <typename T>
T add(T a, T b) { return a + b; }

add(1, 2);        // int
add(1.0, 2.0);    // double
```

### Run-Time (Dynamic) Polymorphism

Achieved via **virtual functions** and **base-class pointers/references**.

```cpp
std::vector<std::unique_ptr<Shape>> shapes;
shapes.push_back(std::make_unique<Circle>(2.0));
shapes.push_back(std::make_unique<Rectangle>(3, 4));

for (auto& s : shapes) std::cout << s->area() << "\n";  // dispatch to Circle or Rectangle
```

### How It Works (vtable)

| Concept | Explanation |
|---------|-------------|
| **vtable** | Per-class table of function pointers for virtual functions |
| **vptr**    | Hidden pointer stored in each object, points to its class's vtable |
| **Cost**    | One indirection per virtual call + one pointer per object |

### When NOT to Use Polymorphism

- **Templates** are usually faster (inlining).
- Use dynamic dispatch only when the type **truly** varies at runtime.

### Slicing (Pitfall)

```cpp
void print(Shape s) { std::cout << s.area(); }    // pass by value — SLICES!
Circle c(2.0);
print(c);   // only Shape copy remains; Circle data lost
```

**Fix:** pass by pointer or reference.

```cpp
void print(const Shape& s) { std::cout << s.area(); }
```

---

## 6. Association, Aggregation, Composition — The Three Relationships

### Quick Distinction

| Type | Phrase | Lifetime Coupling | UML Diamond |
|------|--------|-------------------|-------------|
| **Association** | "uses-a" | Independent | Plain line |
| **Aggregation** | "has-a" (weak) | Whole can exist without part | Hollow diamond (whole) |
| **Composition** | "part-of" (strong) | Part dies with whole | Filled diamond (whole) |

### Examples in C++

#### Association — Loose use

```cpp
class Doctor;             // forward declaration
class Patient {
public:
    void visit(Doctor& d) { /* uses Doctor for a moment */ }
};
```

#### Aggregation — Weak "whole-part"

```cpp
class Team {
    std::vector<Player*> players_;   // pointers; players exist without Team
};
```

#### Composition — Strong ownership

```cpp
class House {
    std::vector<Room> rooms_;       // value semantics; rooms die with House
public:
    House() : rooms_{4} {}           // build 4 rooms inside constructor
};
```

### Coding Heuristics

- **Composition:** value members or `unique_ptr<Part>`.
- **Aggregation:** raw pointer, weak reference, or `weak_ptr`.
- **Association:** function parameter, local reference.

---

## 7. Abstract Classes vs Interfaces

### Pure Abstract Class = Interface (in C++ idiom)

```cpp
class ILogger {
public:
    virtual void log(std::string_view msg) = 0;
    virtual ~ILogger() = default;
};
```

### Abstract Class with Implementation

```cpp
class Animal {
public:
    void breathe() { std::cout << "breathing\n"; }   // shared code
    virtual void speak() const = 0;                  // must override
    virtual ~Animal() = default;
};

class Dog : public Animal {
public:
    void speak() const override { std::cout << "woof\n"; }
};
```

### When to Use Which

| Use Abstract Class | Use Interface |
|--------------------|---------------|
| Shared base code (data + functions) | Pure contract — no state, no implementation |
| Implementation needs to vary in some parts | Multiple unrelated classes implement same behavior |
| Strong "is-a" link | Capability ("can-do") |

### Multiple Interfaces — Common Pattern

```cpp
class IShape        { public: virtual double area() const = 0; };
class IDrawable     { public: virtual void draw() const = 0; };
class ISerializable { public: virtual std::string toJson() const = 0; };

class Square : public IShape, public IDrawable, public ISerializable { /* ... */ };
```

---

## 8. Coupling, Cohesion, and Other Design Heuristics

### Cohesion — Inside a Module

- **High cohesion:** elements in a module belong together.
- **Low cohesion:** module does unrelated things.

Goal: **HIGH cohesion.** SRP is one face of this.

### Coupling — Between Modules

- **Tight coupling:** changing one forces changes in the other.
- **Loose coupling:** modules interact via small, stable interfaces.

Goal: **LOOSE coupling.** DIP/ISP are tools to achieve it.

### Common Metrics

- **LCOM (Lack of Cohesion of Methods):** how unrelated methods are within a class.
- **CBO (Coupling Between Objects):** number of other classes a class is coupled to.
- **Cyclomatic complexity:** independent paths through code — lower is easier.

### Other Heuristics

- **DRY:** Don't Repeat Yourself — extract common behavior.
- **YAGNI:** You Aren't Gonna Need It — don't add speculative generality.
- **KISS:** Keep It Simple, Stupid — simplicity over cleverness.
- **Law of Demeter (Principle of Least Knowledge):** only talk to immediate friends, not strangers.

```cpp
// Bad: chain access (train wreck)
std::cout << order.customer().address().city();
// Good: ask the object to do it
std::cout << order.shipToCity();
```

---

## 9. Common Pitfalls — Slicing, Diamond, Fragile Base Class

### Slicing (See §5)

Slice by passing by value. Use pointers/references.

### Diamond Problem (See §4)

Solved via `virtual` inheritance. Or design without multiple data inheritance.

### Forgetting Virtual Destructor

```cpp
class Base {
public:
    ~Base() { std::cout << "Base\n"; }      // NOT virtual
};
class Derived : public Base {
    int* arr_;
public:
    Derived() : arr_(new int[100]) {}
    ~Derived() { delete[] arr_; std::cout << "Derived\n"; }
};

Base* b = new Derived();
delete b;     // calls only Base::~Base → arr_ leaks
```

**Fix:** declare `~Base() virtual`.

### Fragile Base Class Problem

When you change a base class, derived classes break in subtle ways. Mitigations:

- Prefer **composition** over inheritance.
- Mark designed-for-overriding methods `virtual` explicitly.
- Follow **NVI (Non-Virtual Interface)** idiom: public non-virtual, private virtual.

```cpp
// NVI: public non-virtual template method, private customizable hook
class Transaction {
public:
    bool execute() {                     // public template
        if (preCheck()) { doWork(); postCommit(); return true; }
        return false;
    }
private:
    virtual bool preCheck()   = 0;       // customization point
    virtual void doWork()     = 0;
    virtual void postCommit() {}
};
```

### Public Inheritance for Code Reuse Only

```cpp
// Bad: "Reuse" only — no real is-a
class Stack : public std::vector<int> {  // breaks invariants: vector ops expose internals
};
```

**Fix:** use a `std::vector<int> vec_` privately and expose only `push/pop/top`.

### Overriding Without `override`

Easy to typo: `void draw() const`. Always use the `override` keyword (C++11+) so the compiler checks.

---

## 10. SOLID Overview (Deeper Coverage in 06-)

| Letter | Principle | One-Liner |
|--------|-----------|-----------|
| **S** | Single Responsibility | One class = one reason to change |
| **O** | Open/Closed | Open for extension, closed for modification |
| **L** | Liskov Substitution | Subtypes must honor base contracts |
| **I** | Interface Segregation | Many specific interfaces > one general |
| **D** | Dependency Inversion | Depend on abstractions, not concretions |

See **`06-solid-principles/README.md`** for the full treatment with C++ examples.

---

## 11. Design Patterns Overview

The "Gang of Four" (GoF) book classifies 23 patterns into 3 categories:

### Creational — Object creation

| Pattern | Idea |
|---------|------|
| **Singleton** | Only one instance |
| **Factory Method** | Subclass decides which class to instantiate |
| **Abstract Factory** | Family of related objects |
| **Builder** | Step-by-step construction |
| **Prototype** | Clone instead of building |

### Structural — Class/object composition

| Pattern | Idea |
|---------|------|
| **Adapter** | Bridge incompatible interfaces |
| **Decorator** | Wrap to add behavior |
| **Facade** | Simplified front to a complex subsystem |
| **Proxy** | Surrogate controlling access |
| **Composite** | Tree of objects, treat uniformly |
| **Bridge** | Decouple abstraction from impl |
| **Flyweight** | Share many small objects to save memory |

### Behavioral — Communication

| Pattern | Idea |
|---------|------|
| **Observer** | Notify dependents of state change |
| **Strategy** | Swap algorithms at runtime |
| **State** | Behavior changes with internal state |
| **Template Method** | Skeleton in base, steps in subclasses |
| **Command** | Encapsulate a request as an object |
| **Iterator** | Sequential access without exposing internals |
| **Mediator** | Centralize complex communication |
| **Memento** | Capture/restore state (undo) |
| **Visitor** | Add ops to objects without modifying them |
| **Chain of Responsibility** | Pass request along a chain of handlers |

Full C++ examples: **`07-design-patterns/README.md`**.

---

## 12. UML Class Diagram — Interview Quick Reference

UML class diagrams are common in design rounds. The essentials:

```
            +-----------------------+
            |        Order          |
            +-----------------------+
            | - id: int             |   - private
            | # total: double       |   # protected
            | + status: Status      |   + public
            +-----------------------+
            | + addItem(Item)       |
            | + checkout(): bool    |
            +-----------------------+
                       |
                  1  0..*    (one Order has many Items)
                       ▼
            +-----------------------+
            |        Item           |
            +-----------------------+
```

### Relationship Notations

| Relationship | Notation | Code hint |
|--------------|----------|-----------|
| Association | `————>` | pointer/reference passed as parameter |
| Aggregation | `——◇>` | pointer/weak_ptr owned externally |
| Composition | `——◆>` | value/`unique_ptr` member |
| Inheritance (extends) | `————▷` | `class D : public B` |
| Realization (implements) | `————▷` (dashed) | `class D : public I` |
| Dependency | ` - - >` (dashed) | Function parameter/local var |

### Multiplicity

`1`, `0..*`, `1..*`, `n..m` — show how many of each.

---

## 13. Top Interview Q&A (35 entries)

**Q1: What is OOP?**
Programming paradigm using **objects** that encapsulate state (fields) and behavior (methods), modeled after real-world entities, communicating via messages.

**Q2: List the 4 pillars.**
Encapsulation, Abstraction, Inheritance, Polymorphism.

**Q3: Encapsulation vs abstraction?**
Encapsulation = bundling state and controlling access (private + public API). Abstraction = exposing only essential behavior at the interface, hiding implementation details.

**Q4: Compile-time vs run-time polymorphism?**
Compile-time = templates, overloading — resolved at compile time. Run-time = virtual functions — resolved via vtable at run time.

**Q5: What is a vtable?**
A table of function pointers the compiler generates per class with virtual functions. Each instance has a hidden pointer (`vptr`) to its class's vtable.

**Q6: What is the slicing problem?**
Passing a derived object by value to a function taking a base — only the base subobject is copied; derived-specific data is lost.

**Q7: Why must polymorphic base destructors be virtual?**
Otherwise `delete basePtr` (where basePtr points to derived) only calls `~Base`, leaking derived resources.

**Q8: Why is `override` important?**
Catches typos at compile time (`virtual void draw() const` vs `draw()`), documents intent, lets refactorers update the base without silently breaking overrides.

**Q9: Multiple inheritance — when is it OK?**
For **interfaces** (no state). Avoid for classes with data to dodge the diamond problem and fragile base class.

**Q10: Diamond problem and its fix?**
Two paths inherit from the same base → duplicate subobjects. Fix: `virtual` inheritance so only one subobject exists.

**Q11: Abstract class vs interface in C++?**
C++ has no `interface` keyword. By convention: an abstract class with **only** pure virtual functions is treated as an interface. Abstract class can have data members + implementations; "interface" cannot.

**Q12: Association vs aggregation vs composition?**
**Association:** uses-a; both independent.
**Aggregation:** has-a; whole can exist without part (hollow diamond).
**Composition:** part-of; part dies with whole (filled diamond).

**Q13: Why prefer composition over inheritance?**
Avoids fragile base class, swaps behavior at runtime, easier testing, decouples lifecycles.

**Q14: When IS inheritance the right tool?**
Clear "is-a" relationship, stable hierarchy, polymorphic substitution is needed.

**Q15: What is `final` (C++11)?**
`virtual void f() final;` — prevents further overriding in derived classes. Class-level: `class Foo final {};` — prevents derivation.

**Q16: What is `abstract` (Java) vs `= 0` (C++)?**
Java: `abstract` method has no body; class with abstract methods is abstract. C++: pure virtual `= 0` makes a class abstract.

**Q17: What is `dynamic_cast`?**
Safely converts a base pointer/reference to a derived type. Returns `nullptr` (pointer) or throws `std::bad_cast` (reference) if the cast is invalid.

**Q18: What is a friend function/class?**
A non-member function (or class) granted access to the private/protected members of another class. Use sparingly — breaks encapsulation.

**Q19: What is a virtual base class?**
A base specified with `virtual` inheritance to ensure only **one** subobject in the derived class. Used to solve the diamond.

**Q20: What is the NVI idiom?**
Non-Virtual Interface — public non-virtual function calls private virtual functions. Lets the base control the algorithm skeleton while subclasses customize steps.

**Q21: What is the Curiously Recurring Template Pattern (CRTP)?**
Derived class inherits from a template instantiated with itself: `class D : public B<D>`. Static polymorphism without vtable cost.

```cpp
template <class Derived>
class Addable {
public:
    Derived operator+(const Derived& other) const {
        Derived result(static_cast<const Derived&>(*this));
        result += other;
        return result;
    }
};
```

**Q22: What is a "pure virtual" function?**
Declared with `= 0`. No implementation in the base. Makes the class abstract. Subclasses must override.

**Q23: What is the difference between overriding and overloading?**
**Overriding:** same signature in derived class (subtype polymorphism).
**Overloading:** same name, different signatures, in same scope (ad-hoc polymorphism).

**Q24: What is polymorphism in OOP?**
Same interface (e.g., a base pointer's method call) yielding different behavior depending on the actual object's type.

**Q25: Can a constructor be virtual?**
No. Constructors aren't inherited. Use a virtual `clone()` or factory function for polymorphic construction.

**Q26: What is the virtual function call inside a constructor problem?**
During base construction, the object's dynamic type is "being constructed as base". Virtual calls resolve to the **base** version, ignoring the derived override.

**Q27: Can static methods be virtual?**
No. Static methods aren't tied to an instance; there's no dynamic dispatch.

**Q28: What is an abstract data type (ADT)?**
A theoretical concept: a type defined by its behavior (operations) rather than its implementation. Examples: stack, queue, list.

**Q29: What is the difference between an interface and a type?**
A **type** is the set of values and operations. An **interface** is the set of method signatures — a contract. A type may have multiple interfaces.

**Q30: What is a `protected` member?**
Accessible to the class itself, its friends, and any subclass. Used when subclasses genuinely need access but external code should not.

**Q31: What is the Liskov Substitution Principle (LSP)?**
Subtypes must be **behaviorally substitutable** for their base types. Code using a base must work with any subclass without surprises.

**Q32: What is the Open/Closed Principle?**
Classes should be open for **extension** (new subclasses, new strategies) but closed for **modification** (existing code stable).

**Q33: What is a callback in OOP terms?**
A reference to a function passed to another function, called back when an event happens. In OOP, often a std::function or an interface (Observer pattern).

**Q34: What is the difference between early and late binding?**
**Early binding** (compile-time): call resolved by compiler (non-virtual, overloaded).
**Late binding** (run-time): call resolved at execution via vtable (virtual).

**Q35: What is the difference between "is-a" and "is implemented in terms of"?**
"Is-a" → inheritance. "Is implemented in terms of" → composition or private inheritance (you expose no public type relationship but reuse internals).

---

> **Next:** continue with `05-cpp-specific/README.md` for C++-specific mechanics (vtable, smart pointers, move semantics, RAII) used to implement OOP.
