# Object-Oriented Programming — Concepts

## 1. The Four Pillars

### Encapsulation
Bundle data + methods; hide internals; expose via accessors.
```cpp
class Account {
private:
 double balance; // hidden
public:
 void deposit(double amt) { /* validate */ balance += amt; }
 double getBalance() const { return balance; }
};
```

### Abstraction
Show **what** an object does, hide **how**.
```cpp
class Shape {
public:
 virtual double area() const = 0; // pure virtual
 virtual ~Shape() {}
};
```

### Inheritance
Reuse code by deriving classes.
```cpp
class Animal { public: void eat() {} };
class Dog : public Animal { public: void bark() {} };
```

### Polymorphism
Same interface, different behavior.
```cpp
Shape* s = new Circle(5.0);
cout << s->area(); // calls Circle::area()
```

Two kinds:
- **Compile-time** (static): function/operator overloading, templates.
- **Run-time** (dynamic): virtual functions via vtable.

---

## 2. SOLID Principles

### S — Single Responsibility
A class should have **one** reason to change. One job.

### O — Open/Closed
Open for **extension**, closed for **modification**.
Use inheritance/composition to add behavior without changing existing code.

### L — Liskov Substitution
Subclasses must be substitutable for base classes. Don't violate base class invariants.

```cpp
// Bad: Square extends Rectangle but breaks setWidth/setHeight independence
```

### I — Interface Segregation
Many specific interfaces > one general interface.
Don't force clients to depend on methods they don't use.

### D — Dependency Inversion
Depend on **abstractions**, not concretions.
```cpp
class OrderService {
 PaymentProcessor* pp; // abstraction
public:
 OrderService(PaymentProcessor* p) : pp(p) {}
 void checkout() { pp->pay(...); }
};
```

---

## 3. Composition vs Inheritance

| Use Composition When | Use Inheritance When |
|----------------------|----------------------|
| "has-a" relationship | Clear "is-a" relationship |
| Behavior might change | Stable hierarchy |
| Avoid tight coupling | Polymorphic dispatch needed |

**Favor composition over inheritance** in most cases (more flexible, avoids the fragile base class problem).

---

## 4. Method Overloading vs Overriding

| Overloading | Overriding |
|-------------|-----------|
| Same name, **different params** | Same name, **same params** |
| Compile-time polymorphism | Run-time polymorphism |
| Within same class | In derived class |

---

## 5. Abstract Class vs Interface (C++)

C++ doesn't have `interface` keyword, but you simulate it with **pure virtual functions**:
```cpp
class IDrawable { // interface
public:
 virtual void draw() const = 0;
 virtual ~IDrawable() {}
};
```

**Abstract class:** has at least one pure virtual method. Cannot be instantiated.

---

## 6. Common C++ OOP Pitfalls

- **Slicing:** passing by value to a function expecting a base class reference.
- **Forgetting virtual destructor** in a polymorphic base class → memory leak.
- **Public inheritance for code reuse only** — composition is usually better.
- **Diamond problem** — solve with `virtual` inheritance.

```cpp
class A {};
class B : virtual public A {};
class C : virtual public A {};
class D : public B, public C {}; // OK, single A subobject
```

---

## 7. Top Interview Q&A

**Q: Difference between abstract class and interface in C++?**
C++ has no interface keyword. A class with all pure virtual methods = interface. Abstract class = has at least one pure virtual method (and possibly implementation/data members).

**Q: What is a virtual function?**
A member function declared with `virtual` that supports run-time polymorphism. Implementation chosen by the actual object type, not the pointer type.

**Q: What is vtable?**
A table of function pointers maintained per class with virtual functions. Each instance has a hidden vptr pointing to its class's vtable.

**Q: What is multiple inheritance? When to use it?**
A class inheriting from more than one base. Use sparingly — risk of diamond problem.

**Q: Difference between association, aggregation, composition?**
- **Association:** uses-a (weak). Both can live independently.
- **Aggregation:** has-a (weaker whole-part). Whole can exist without part.
- **Composition:** part-of (strong). Part dies with whole.

**Q: Why prefer composition over inheritance?**
Flexibility, easier testing, avoids fragile base class, swap behavior at runtime.
