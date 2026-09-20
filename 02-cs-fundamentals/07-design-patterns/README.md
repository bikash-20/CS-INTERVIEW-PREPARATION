# Design Patterns — Theory with C++ Examples

## Creational Patterns

### 1. Singleton
Ensures a class has only one instance, provides global access.

```cpp
class Singleton {
 static Singleton* instance;
 Singleton() = default;
public:
 static Singleton* getInstance() {
 if (!instance) instance = new Singleton();
 return instance;
 }
};
// Thread-safe (C++11): use 'static Singleton instance;' inside getInstance.
```

**Use when:** DB connection pool, logger, config manager.
**Pitfall:** Considered anti-pattern for testing.

### 2. Factory Method
Defer object creation to subclasses.
```cpp
class Shape {
public:
 virtual void draw() = 0;
 virtual ~Shape() {}
};
class Circle : public Shape { public: void draw() override { /*...*/ } };

class ShapeFactory {
public:
 static Shape* create(const string& type) {
 if (type == "circle") return new Circle();
 // ...
 }
};
```

### 3. Abstract Factory
Family of related objects without specifying concrete classes.
Example: GUI toolkit — create button + checkbox matching a theme (Windows/Mac/Linux).

### 4. Builder
Step-by-step construction of complex objects.
```cpp
class PizzaBuilder {
 string dough, sauce, topping;
public:
 PizzaBuilder& setDough(string d) { dough = d; return *this; }
 PizzaBuilder& setSauce(string s) { sauce = s; return *this; }
 Pizza build() { return Pizza(dough, sauce, topping); }
};
Pizza p = PizzaBuilder().setDough("thin").setSauce("tomato").build();
```

### 5. Prototype
Clone an existing object instead of creating from scratch.
`virtual Prototype* clone() = 0;`

---

## Structural Patterns

### 6. Adapter
Bridge between incompatible interfaces.
```cpp
class OldPrinter { void printOld(string s) {} };
class Adapter {
 OldPrinter* old;
public:
 void print(string s) { old->printOld(s); }
};
```

### 7. Decorator
Add behavior dynamically by wrapping.
```cpp
class Coffee { public: virtual double cost() = 0; };
class BasicCoffee : public Coffee { double cost() override { return 5; } };
class MilkDecorator : public Coffee {
 Coffee* c;
public:
 MilkDecorator(Coffee* c) : c(c) {}
 double cost() override { return c->cost() + 1; }
};
```

### 8. Facade
Simplified interface to a complex subsystem.

### 9. Proxy
Placeholder controlling access (lazy loading, remote, protection).

### 10. Composite
Tree structure; treat individual objects and compositions uniformly.
Example: File system (File + Directory containing Files/Directories).

---

## Behavioral Patterns

### 11. Observer
Notify dependents on state change.
```cpp
class Subject {
 vector<Observer*> obs;
public:
 void attach(Observer* o) { obs.push_back(o); }
 void notify() { for (auto o : obs) o->update(); }
};
```

### 12. Strategy
Family of algorithms, choose at runtime.
```cpp
class Sorter { public: virtual void sort(vector<int>&) = 0; };
class QuickSort : public Sorter { /* ... */ };
class SorterCtx {
 Sorter* s;
public:
 void setStrategy(Sorter* s) { this->s = s; }
 void execute(vector<int>& v) { s->sort(v); }
};
```

### 13. State
Object behavior changes based on internal state.

### 14. Template Method
Define skeleton, let subclasses fill in steps.

### 15. Command
Encapsulate a request as an object (queue, undo/redo).

### 16. Iterator
Sequential access without exposing internals. C++ has built-in iterators.

---

## Top Interview Q&A

**Q: Singleton vs Static class?**
Singleton can implement interfaces, be passed as object, lazily initialized, polymorphic. Static class can't.

**Q: When to use Strategy vs State?**
Strategy: external algorithm choice, doesn't know about context. State: internal state transitions drive behavior.

**Q: Difference between Decorator and Proxy?**
Decorator adds behavior; Proxy controls access (no behavior change).

**Q: Why is Singleton controversial?**
Hidden dependencies, hard to test, global state, threading concerns.
