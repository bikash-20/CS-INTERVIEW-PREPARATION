# SOLID Principles — In-Depth with C++ Examples

> **SOLID** are five principles introduced by Robert C. Martin ("Uncle Bob") that, when followed, produce codebases that are **maintainable, flexible, testable, and resilient to change**. This file gives you the theory, deep C++ examples, common violations, fixes, and 30+ interview Q&A.

---

## Table of Contents

1. [Why SOLID](#1-why-solid)
2. [S — Single Responsibility Principle (SRP)](#2-s--single-responsibility-principle-srp)
3. [O — Open/Closed Principle (OCP)](#3-o--openclosed-principle-ocp)
4. [L — Liskov Substitution Principle (LSP)](#4-l--liskov-substitution-principle-lsp)
5. [I — Interface Segregation Principle (ISP)](#5-i--interface-segregation-principle-isp)
6. [D — Dependency Inversion Principle (DIP)](#6-d--dependency-inversion-principle-dip)
7. [Beyond SOLID — Packaging Principles (Quick Note)](#7-beyond-solid--packaging-principles-quick-note)
8. [Common Violations & Refactorings](#8-common-violations--refactorings)
9. [When NOT to Apply SOLID](#9-when-not-to-apply-solid)
10. [Quick Reference Card](#10-quick-reference-card)
11. [Top Interview Q&A (30 entries)](#11-top-interview-qa-30-entries)

---

## 1. Why SOLID

Before SOLID, the same issues kept showing up in OOP codebases:

- **Rigidity** — change one area, break others.
- **Fragility** — tests pass, production breaks in surprising ways.
- **Immobility** — can't reuse a class because it pulls the world with it.
- **Viscosity** — easier to hack than to follow good design.

SOLID is the antidote. The five letters map directly to design smells and fixes.

| Letter | Namesake | Solves |
|--------|----------|--------|
| **S** | Single Responsibility | Classes doing too much |
| **O** | Open/Closed | Changes ripple across the codebase |
| **L** | Liskov Substitution | Subclasses that surprise you |
| **I** | Interface Segregation | Interfaces that force empty methods |
| **D** | Dependency Inversion | Tight coupling to concretions |

---

## 2. S — Single Responsibility Principle (SRP)

> *"A class should have one, and only one, reason to change."* — Uncle Bob

A **reason to change** = an *actor* (a stakeholder, group, or change source).

### Bad Example — Three Reasons to Change

```cpp
class User {
public:
    void saveToDatabase() { /* SQL ... */ }   // persistence team
    void sendWelcomeEmail() { /* SMTP */ }    // mail team
    std::string generateInvoice() { /* ... */ } // billing team
};
```

### Good Example — One Reason Each

```cpp
class User {                     // identity & auth state only
public:
    std::string id;
    std::string email;
    std::string name;
};

class UserRepository {           // persistence
public:
    void save(const User& u);
    std::optional<User> findById(const std::string& id);
};

class EmailService {             // communication
public:
    void sendWelcome(const User& u);
};

class InvoiceGenerator {         // reporting / billing
public:
    std::string generate(const User& u);
};
```

### Spotting SRP Violations

- Class name has "And" or "Manager" or "Helper" or "Utils".
- Methods that touch **unrelated subsystems** (DB + email + logging).
- Different **rate of change** in different methods.
- Methods used by different actors.

### SRP vs Separation of Concerns

SRP is one face of **separation of concerns** at the class level. SoC can also be applied to functions, modules, layers, packages.

---

## 3. O — Open/Closed Principle (OCP)

> *"Software entities should be **open for extension**, but **closed for modification**."* — Bertrand Meyer

Add new behavior **without changing existing, tested code**.

### Classic Shape Example

```cpp
class Shape {
public:
    virtual double area() const = 0;
    virtual ~Shape() = default;
};

class Circle : public Shape {
    double r_;
public:
    explicit Circle(double r) : r_(r) {}
    double area() const override { return 3.14159265358979 * r_ * r_; }
};

class Square : public Shape {
    double s_;
public:
    explicit Square(double s) : s_(s) {}
    double area() const override { return s_ * s_; }
};

// Adding Triangle doesn't touch existing classes.
class Triangle : public Shape {
    double b_, h_;
public:
    Triangle(double b, double h) : b_(b), h_(h) {}
    double area() const override { return 0.5 * b_ * h_; }
};

double totalArea(const std::vector<std::unique_ptr<Shape>>& shapes) {
    double sum = 0;
    for (const auto& s : shapes) sum += s->area();
    return sum;
}
```

### OCP via Strategy (function pointer / `std::function`)

```cpp
using PricingStrategy = std::function<double(double)>;

PricingStrategy regular  = [](double p) { return p; };
PricingStrategy premium  = [](double p) { return p * 1.10; };

double quote(PricingStrategy s, double price) { return s(price); }
```

### OCP via Decorator (Wrap Behavior)

```cpp
class Coffee { public: virtual double cost() const = 0; virtual ~Coffee() = default; };
class BasicCoffee : public Coffee { public: double cost() const override { return 5.0; } };

class CoffeeDecorator : public Coffee {
    std::unique_ptr<Coffee> inner_;
public:
    explicit CoffeeDecorator(std::unique_ptr<Coffee> inner) : inner_(std::move(inner)) {}
};

class MilkDecorator : public CoffeeDecorator {
public:
    using CoffeeDecorator::CoffeeDecorator;
    double cost() const override { return inner_->cost() + 1.0; }
};

class SugarDecorator : public CoffeeDecorator {
public:
    using CoffeeDecorator::CoffeeDecorator;
    double cost() const override { return inner_->cost() + 0.5; }
};
```

### Spotting OCP Violations

- Long `if/else` or `switch` on type — replace with polymorphism.
- Adding new variants forces editing existing core code.
- Conditional logic that grows over time.

### Closure Is Selective

- Don't *over*-OCP. Speculative generality (YAGNI) is a worse sin.
- Apply OCP in the places change actually happens.

---

## 4. L — Liskov Substitution Principle (LSP)

> *"Objects of a superclass shall be replaceable with objects of a subclass **without breaking the application**."* — Barbara Liskov

Subtypes must honor their base **contracts** — preconditions, postconditions, invariants.

### Classic Violation: Square ⊆ Rectangle?

```cpp
class Rectangle {
public:
    virtual void setWidth(double w)  { width_  = w; }
    virtual void setHeight(double h) { height_ = h; }
    double area() const { return width_ * height_; }
protected:
    double width_ = 0, height_ = 0;
};

class Square : public Rectangle {
public:
    void setWidth(double w) override {
        width_ = w; height_ = w;       // surprising! breaks expectation
    }
    void setHeight(double h) override {
        width_ = h; height_ = h;
    }
};

void test(Rectangle& r) {
    r.setWidth(2);
    r.setHeight(3);
    assert(r.area() == 6);              // FAILS for Square
}
```

### Fixes

1. **Make `Rectangle` and `Square` immutable** (set in constructor).
2. **Use separate abstractions** (`Shape` base with derived `Rectangle` and `Square`).
3. **Don't make `Square` extend `Rectangle`** if the geometry contract genuinely differs.

### LSP Rules in Practice

A subtype:

- Must accept **at least** the base's inputs (no stricter preconditions).
- Must produce **at least** the base's outputs (no weaker postconditions).
- Must preserve **invariants** of the base.
- Must not throw new exceptions the base doesn't.

| Violation | Symptom |
|-----------|---------|
| Stricter preconditions in override | `Override::f(nullptr)` requires non-null where base allows null |
| Weaker postconditions | Override returns narrower range than base |
| New exceptions | Override throws where base doesn't |
| Changed state invariants | Override leaves object in state base can't reach |

### Real-World Examples of LSP

- `std::stack` is NOT a `std::vector` — peek/pop semantics differ; they shouldn't be in a stack-is-a-vector relationship.
- `LinkedList` as `ArrayList` in Java: would expose indexed semantics that break invariants.
- File systems: read-only vs read-write file handles should be separate types.

### LSP and OCP Together

LSP is what makes OCP **work**: when you substitute a derived type in place of the base, behavior remains valid. If LSP fails, OCP becomes a trap — extensions behave incorrectly.

---

## 5. I — Interface Segregation Principle (ISP)

> *"Many client-specific interfaces are better than one general-purpose interface."*

Don't force clients to depend on methods they don't use.

### Bad — Fat Interface

```cpp
class IMachine {
public:
    virtual void print(std::string_view doc) = 0;
    virtual void scan(std::string_view dst)  = 0;
    virtual void fax(std::string_view num, std::string_view doc) = 0;
};

class SimplePrinter : public IMachine {
public:
    void print(std::string_view) override { /* works */ }
    void scan(std::string_view)  override { throw std::runtime_error("no scanner!"); }
    void fax(std::string_view, std::string_view) override { throw std::runtime_error("no fax!"); }
};
```

### Good — Segregated Interfaces

```cpp
class IPrinter { public: virtual void print(std::string_view) = 0; virtual ~IPrinter() = default; };
class IScanner { public: virtual void scan(std::string_view)  = 0; virtual ~IScanner() = default; };
class IFax     { public: virtual void fax(std::string_view, std::string_view) = 0; virtual ~IFax() = default; };

class SimplePrinter : public IPrinter { public: void print(std::string_view) override {} };
class MultiFunction : public IPrinter, public IScanner, public IFax { /* ... */ };
```

### C++-Specific ISP Tips

- **Multiple inheritance of pure-virtual interfaces** is cheap and clean.
- A class implementing an interface should **use** all of its methods (no empty/default bodies without good reason).
- Use **CRTP / static interfaces** when no virtual dispatch is needed.

### Spotting ISP Violations

- Empty or `throw` method bodies in overrides.
- "Bundled" interfaces that no single client fully uses.
- Clients depending on methods they don't call.

### Role Interfaces vs Header Interfaces

| Header Interface | Role Interface |
|------------------|----------------|
| Describes the **whole object** (e.g., `IUser`) | Describes a **role** (`IAuthenticatable`) |
| Lots of methods | Few methods, narrow use |
| Often bloated | Always small |

**Prefer role interfaces.** A `User` may implement `IAuthenticatable`, `ISerializable`, `IProfile` — each describing a different role it plays.

---

## 6. D — Dependency Inversion Principle (DIP)

> *"High-level modules should not depend on low-level modules. Both should depend on abstractions."*
> *"Abstractions should not depend on details. Details should depend on abstractions."* — Uncle Bob

**DIP is what makes IoC, DI containers, and testable code possible.**

### Bad — Direct Dependence

```cpp
class OrderService {
    MySQLDatabase db_;        // depends on concrete implementation
public:
    void checkout(/* order */) {
        db_.exec("INSERT INTO orders ...");
    }
};
```

### Good — Depend on Abstraction

```cpp
class IOrderRepository {
public:
    virtual void save(const Order& o) = 0;
    virtual ~IOrderRepository() = default;
};

class MySQLOrderRepository : public IOrderRepository {
public:
    void save(const Order& o) override { /* SQL */ }
};

class OrderService {
    IOrderRepository& repo_;
public:
    explicit OrderService(IOrderRepository& repo) : repo_(repo) {}
    void checkout(const Order& o) { repo_.save(o); /* business logic */ }
};
```

Now you can:
- Mock `IOrderRepository` for unit tests.
- Swap to `PostgresOrderRepository` without touching `OrderService`.

### DIP & Ownership

DIP says "depend on abstractions" — but ownership and lifetime are still concerns. Common patterns:

| Lifetime | Typical Approach |
|----------|------------------|
| Singleton | Constructor injection of a singleton reference |
| Per-request | Pass via constructor at request scope |
| Shared | Inject a `shared_ptr<IInterface>` |
| Owned created | Factory inside the high-level module (still abstract) |

### DIP Techniques

- **Constructor injection** — preferred: dependencies needed for life of object.
- **Setter injection** — optional / late-bound dependencies.
- **Interface injection** — provide an injector that hands the impl to the consumer.

### Dependency Injection Containers

Libraries like [Boost.DI](https://www.boost.org/doc/libs/release/libs/di/) or [Hypodermic](https://github.com/ybastide/hypodermic) build dependency graphs for you. Useful in large codebases, but understanding manual injection first is critical.

---

## 7. Beyond SOLID — Packaging Principles (Quick Note)

SOLID is mostly **class-level**. Other principles guide the architecture:

- **Common Closure Principle** (CCP): classes that change together go in the same package.
- **Common Reuse Principle** (CRP): classes used together go in the same package.
- **Acyclic Dependencies Principle** (ADP): package dependencies form a DAG.
- **Stable Dependencies Principle** (SDP): depend in the direction of stability.
- **Stable Abstractions Principle** (SAP): stable packages are abstract; unstable are concrete.

These come from Uncle Bob's *Clean Architecture* and package-level guidance.

---

## 8. Common Violations & Refactorings

### Violation → Refactoring Cheatsheet

| Smell | Likely Violation | Fix |
|-------|------------------|-----|
| God class (does everything) | SRP | Extract classes |
| Long `switch`/`if-else` on type | OCP | Polymorphism / Strategy |
| Subclass surprises callers | LSP | Redesign hierarchy |
| Empty overrides / `throw` | ISP | Split the interface |
| Tightly coupled to a class | DIP | Introduce abstraction + inject |
| Hard to unit-test (DBs, sockets) | DIP | Inject mocks |
| Adding features forces editing | OCP | Strategy / Decorator |
| Inheritance misuse | LSP / Composition | Prefer composition |
| Singleton everywhere | DIP | Inject the dependency |

### Real Refactoring Workflow

1. Add tests for the current behavior.
2. Extract interface / inject dependency (DIP).
3. Move logic to a new class (SRP).
4. Replace conditionals with polymorphism (OCP).
5. Split interfaces (ISP).
6. Fix or redesign the hierarchy (LSP).
7. Keep tests green at every step.

---

## 9. When NOT to Apply SOLID

**Pragmatism over dogma.**

| Scenario | Why Pragmatism Wins |
|----------|---------------------|
| Quick prototype / spike | Over-design slows you down |
| Tiny scripts | SRP/OCP overkill |
| Performance-critical inner loops | Virtual dispatch + indirection cost |
| Tight resource constraints (embedded) | Abstractions add bloat |
| One-off batch job | SRP rarely worth the indirection |
| YAGNI zone | Speculative generality creates dead abstractions |

**Rule of thumb:** Apply SOLID when **change is expected** in the area; otherwise wait for the smell.

---

## 10. Quick Reference Card

| Principle | One-Liner | Symptom if Violated | Fix |
|-----------|-----------|---------------------|-----|
| SRP | One class = one reason to change | God class | Extract classes |
| OCP | Open for extension, closed for change | Adding feature touches everywhere | Polymorphism, Strategy |
| LSP | Subtypes substitutable for base | Subclass surprises callers | Redesign hierarchy |
| ISP | Many specific interfaces | Empty or `throw` overrides | Split interfaces |
| DIP | Depend on abstractions | Tight coupling, no tests | Inject abstractions |

**Mnemonic:** "**S**olid **O**bjects **L**everage **I**nterface **D**esign"

---

## 11. Top Interview Q&A (30 entries)

**Q1: What does S in SOLID stand for? What does it mean?**
Single Responsibility Principle. A class should have **one, and only one, reason to change** — one primary responsibility tied to one actor.

**Q2: What is OCP?**
Open/Closed Principle. Software entities should be **open for extension** but **closed for modification**. Add behavior through new classes, not edits to existing tested code.

**Q3: What is LSP?**
Liskov Substitution Principle. Subtypes must be **behaviorally substitutable** for their base types without breaking correctness.

**Q4: What is ISP?**
Interface Segregation Principle. Many **small, role-specific interfaces** are better than one large one. Don't force clients to depend on methods they don't use.

**Q5: What is DIP?**
Dependency Inversion Principle. High-level modules should depend on **abstractions**, not concrete implementations. Enables dependency injection and mocking.

**Q6: How does DIP relate to Dependency Injection?**
DIP is the **principle** (depend on abstractions). DI is the **technique** (inject the abstraction) — constructor, setter, or interface.

**Q7: How is OCP related to LSP?**
OCP says "open for extension" — usually means adding derived classes. LSP guarantees those derived classes are **safe substitutes**. LSP is what makes OCP safe in practice.

**Q8: How is SRP different from Separation of Concerns?**
SRP is **one specific form** of separation of concerns at the class level. SoC is a broader principle that applies to functions, modules, layers, packages.

**Q9: Are SOLID principles always applicable?**
No. For prototypes, tiny utilities, or performance-critical code, pragmatism wins. Apply SOLID where **change** is expected.

**Q10: How does SOLID relate to design patterns?**
Patterns are **concrete solutions** for recurring problems. SOLID are **guidelines** that many patterns follow. Patterns help satisfy SOLID without violating it.

**Q11: How does Singleton violate DIP?**
Singleton is often both **created and used** by callers, so callers depend on a concrete class. Testability suffers.

**Q12: Show a God-class refactor (SRP).**
See §2 — split `User` into `User`, `UserRepository`, `EmailService`, `InvoiceGenerator`.

**Q13: Show an LSP fix for Square/Rectangle.**
Three options: make both immutable; separate `Shape` hierarchy with `Rectangle` and `Square` siblings; or use composition.

**Q14: Why are interface segregation violations subtle?**
Empty or `throw` overrides look "harmless" but signal that the type relationship is wrong. They also propagate as runtime errors instead of compile errors.

**Q15: Give an example of DIP using constructor injection.**
```cpp
class OrderService {
    IOrderRepository& repo_;
public:
    explicit OrderService(IOrderRepository& repo) : repo_(repo) {}
    void checkout(const Order& o) { repo_.save(o); }
};
```

**Q16: Can you have LSP without inheritance?**
Yes — it's about substitutability of any abstraction (interfaces, type classes, generics). Subtyping is one mechanism; structural conformance is another.

**Q17: What's the relationship between SOLID and testing?**
- **DIP** lets you inject mocks → unit testable.
- **SRP** keeps classes small → easier tests.
- **OCP** lets you extend with test doubles.
- **ISP** keeps test doubles small.
- **LSP** keeps tests of base type valid for derived types.

**Q18: What is a "role interface"?**
A small interface describing a **capability** rather than a whole type. e.g., `ISerializable`, `IAuditable`, `IComparable`. Promotes ISP.

**Q19: How do you spot OCP violations?**
- `switch` / `if-else` chains on type.
- Adding a new case forces editing core code.
- Old tests break when you add a feature.

**Q20: How do you fix OCP violations?**
- Replace conditionals with **polymorphism**.
- Use **Strategy / Decorator / Visitor / Template Method**.
- Often preceded by DIP (abstract the variant).

**Q21: What's the difference between OCP and "open source"?**
OCP is unrelated to open source. It means **extensible without modifying** existing code.

**Q22: Why does the Square/Rectangle problem violate LSP?**
`Square::setWidth` and `setHeight` set **both** dimensions, violating the base's postcondition ("`area == width * height` with independent setters").

**Q23: Does SRP mean one method per class?**
No. SRP means **one actor / reason to change** — a class can have many cohesive methods serving the same actor.

**Q24: What is a "stable abstraction"?**
A principle (SAP) saying stable packages should be **abstract** so they don't depend on concrete change; concrete packages are unstable.

**Q25: What's the difference between ISP and "small interfaces"?**
ISP says interfaces should match **client needs** (role interfaces). Small is one possible heuristic; some interfaces are small but still wrong (still don't match a role).

**Q26: How does DIP interact with RAII?**
DIP talks about abstraction. RAII still owns concrete resources (files, sockets) inside the leaf. The high-level class depends only on the abstract interface; concrete objects manage their own lifetime.

**Q27: When is dependency injection overkill?**
- For one-off scripts.
- For classes with one trivial dependency.
- When the alternative (e.g., global config) is fine and DI just adds ceremony.

**Q28: Show pure virtual vs abstract class in C++ for ISP.**
Pure-virtual class with only abstract methods is the idiomatic interface. Promote **role** interfaces; prefer multiple small interfaces to one big.

**Q29: How do you assess if SOLID is over-applied?**
Symptoms:
- Dozens of single-method interfaces for no reason.
- 10 indirections to perform a 2-line task.
- Test doubles outnumber production classes.
- Time-to-feature has slowed to a crawl.

**Q30: How does SOLID fit with functional programming?**
Many ideas translate:
- **SRP** → small pure functions.
- **OCP** → new functions added without modifying existing ones.
- **LSP** → subtyping replaced by parametric polymorphism (generics).
- **ISP** → minimal, composable APIs.
- **DIP** → depend on functions/closures, not concretions.

---

> **Next:** continue with `07-design-patterns/README.md` where each SOLID principle gets applied to concrete patterns.