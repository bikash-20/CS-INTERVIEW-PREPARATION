# SOLID Principles — In Depth

The **SOLID** principles are 5 design principles for object-oriented software, introduced by Robert C. Martin ("Uncle Bob"). They make code more **maintainable, flexible, and scalable**.

---

## S — Single Responsibility Principle (SRP)

> *"A class should have one, and only one, reason to change."*

A class should have **one job** — one primary responsibility.

### Bad Example
```cpp
class User {
public:
 void saveToDatabase() {} // persistence
 void sendEmail() {} // communication
 string generateReport() {} // reporting
};
```
Three reasons to change → violates SRP.

### Good Example
```cpp
class UserRepository { public: void save(const User&) {} };
class EmailService { public: void send(const User&) {} };
class ReportGenerator { public: string generate(const User&) {} };
```

---

## O — Open/Closed Principle (OCP)

> *"Software entities should be open for extension, but closed for modification."*

Add new behavior **without changing existing code**.

### Example
```cpp
class Shape { public: virtual double area() const = 0; virtual ~Shape() {} };
class Circle : public Shape {
 double r;
public:
 Circle(double r) : r(r) {}
 double area() const override { return 3.14159 * r * r; }
};
class Square : public Shape {
 double s;
public:
 Square(double s) : s(s) {}
 double area() const override { return s * s; }
};

// Adding a new shape doesn't require modifying existing classes.
```

---

## L — Liskov Substitution Principle (LSP)

> *"Objects of a superclass shall be replaceable with objects of a subclass without breaking the application."*

Subtypes must honor the contracts of their base types.

### Classic Violation: Square/Rectangle
```cpp
class Rectangle {
public:
 virtual void setWidth(double w) { width = w; }
 virtual void setHeight(double h) { height = h; }
 // ...
};
class Square : public Rectangle {
public:
 void setWidth(double w) override { width = height = w; } // breaks expectation
 void setHeight(double h) override { width = height = h; }
};
```
Code that does `Rectangle r; r.setWidth(2); r.setHeight(3);` expects width != height, but for Square they would be equal.

### Fix
- Don't have Square inherit Rectangle with mutable dimensions, OR
- Make them immutable, OR
- Use separate abstractions.

---

## I — Interface Segregation Principle (ISP)

> *"Many client-specific interfaces are better than one general-purpose interface."*

Don't force clients to depend on methods they don't use.

### Bad Example
```cpp
class IMachine {
public:
 virtual void print() = 0;
 virtual void scan() = 0;
 virtual void fax() = 0;
};
class SimplePrinter : public IMachine {
public:
 void print() override {}
 void scan() override {} // not needed!
 void fax() override {} // not needed!
};
```

### Good Example
```cpp
class IPrinter { public: virtual void print() = 0; virtual ~IPrinter() {} };
class IScanner { public: virtual void scan() = 0; virtual ~IScanner() {} };
class MultiFunction : public IPrinter, public IScanner { /* ... */ };
```

---

## D — Dependency Inversion Principle (DIP)

> *"Depend on abstractions, not on concretions."*

High-level modules should not depend on low-level modules. Both should depend on abstractions.

### Bad Example
```cpp
class OrderService {
 MySQLDatabase db; // depends on concrete class
};
```

### Good Example
```cpp
class IDatabase {
public:
 virtual void save(string data) = 0;
 virtual ~IDatabase() {}
};
class MySQLDatabase : public IDatabase { /* ... */ };
class MongoDatabase : public IDatabase { /* ... */ };

class OrderService {
 IDatabase& db;
public:
 OrderService(IDatabase& db) : db(db) {}
 void checkout() { db.save("order..."); }
};
```
Now you can swap database implementations and mock for testing.

---

## Practical Benefits

- **Testability:** Mockable dependencies (DIP), small focused classes (SRP).
- **Maintainability:** Change one area without rippling through the codebase.
- **Flexibility:** Easier to extend (OCP) and swap implementations.
- **Readability:** Small, focused classes are easier to understand.

---

## Common Interview Q&A

**Q: Are SOLID principles always applicable?**
No. Sometimes pragmatism wins (e.g., tiny scripts, performance-critical code, prototypes).

**Q: How does SOLID relate to design patterns?**
Patterns are concrete solutions; SOLID are guiding principles. Patterns help follow SOLID.

**Q: SRP vs separation of concerns?**
SRP is one form of SoC — at the class level. SoC can apply to functions, modules, layers too.

**Q: How does DI differ from DIP?**
DIP is the principle (depend on abstractions). DI is a technique to achieve it (inject dependencies rather than creating them).
