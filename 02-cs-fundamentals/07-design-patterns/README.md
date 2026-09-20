# Design Patterns — Comprehensive Guide with C++ Examples

> **The 23 Gang-of-Four (GoF) patterns** are reusable solutions to recurring object-oriented design problems. This guide covers all three categories — **Creational, Structural, Behavioral** — with full C++17 examples, usage notes, trade-offs, and 50+ Q&A.

---

## Table of Contents

1. [What Are Design Patterns](#1-what-are-design-patterns)
2. [Creational Patterns](#2-creational-patterns)
   - [Singleton](#singleton)
   - [Factory Method](#factory-method)
   - [Abstract Factory](#abstract-factory)
   - [Builder](#builder)
   - [Prototype](#prototype)
3. [Structural Patterns](#3-structural-patterns)
   - [Adapter](#adapter)
   - [Decorator](#decorator)
   - [Facade](#facade)
   - [Proxy](#proxy)
   - [Composite](#composite)
   - [Bridge](#bridge)
   - [Flyweight](#flyweight)
4. [Behavioral Patterns](#4-behavioral-patterns)
   - [Observer](#observer)
   - [Strategy](#strategy)
   - [State](#state)
   - [Template Method](#template-method)
   - [Command](#command)
   - [Iterator](#iterator)
   - [Mediator](#mediator)
   - [Memento](#memento)
   - [Visitor](#visitor)
   - [Chain of Responsibility](#chain-of-responsibility)
5. [Concurrency Patterns (Modern Bonus)](#5-concurrency-patterns-modern-bonus)
6. [Anti-Patterns to Avoid](#6-anti-patterns-to-avoid)
7. [Quick Reference Card](#7-quick-reference-card)
8. [Top Interview Q&A (50 entries)](#8-top-interview-qa-50-entries)

---

## 1. What Are Design Patterns

A design pattern is a **named, reusable solution** to a problem that appears repeatedly in software design. They are not finished code — they are **templates** for how to solve a problem.

### The GoF Catalog

The 1994 book *Design Patterns: Elements of Reusable Object-Oriented Software* by Gamma, Helm, Johnson, Vlissides introduced 23 patterns grouped into 3 categories.

| Category | Concern |
|----------|---------|
| **Creational** | Object creation mechanisms |
| **Structural** | Class/object composition |
| **Behavioral** | Communication between objects |

### Why Patterns Matter in Interviews

- They reveal whether you **recognize recurring problems**.
- They let you talk about **trade-offs**, not just code.
- They connect to **SOLID principles** (often a follow-up).

> **Interview wisdom:** never name-drop a pattern without saying **why** and **what trade-off** it accepts.

---

## 2. Creational Patterns

## Singleton

**Intent:** Ensure a class has only **one instance** and provide a global point of access.

**When to use:** DB connection pools, loggers, config managers, device drivers.

```cpp
class Singleton {
public:
    static Singleton& getInstance() {
        static Singleton instance;        // thread-safe (C++11 magic statics)
        return instance;
    }

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

private:
    Singleton() = default;
    ~Singleton() = default;
};
```

**Pros:** Single instance; global access; lazy init.

**Cons:**
- Considered an **anti-pattern** in many modern codebases — hidden dependencies, hard to test, global state.
- Makes threading subtle.

**Modern alternatives:** **dependency injection** of a single shared instance (still a singleton, but without globals).

### Variants

- **Eager singleton:** instance created at startup.
- **Lazy singleton:** instance on first use (preferred).
- **Monostate:** all static members, multiple instances allowed.

---

## Factory Method

**Intent:** Define an interface for creating an object, but let **subclasses decide** which class to instantiate.

```cpp
class Shape {
public:
    virtual void draw() const = 0;
    virtual ~Shape() = default;
};

class Circle : public Shape { public: void draw() const override { std::cout << "○\n"; } };
class Square : public Shape { public: void draw() const override { std::cout << "□\n"; } };

class ShapeFactory {
public:
    virtual std::unique_ptr<Shape> create() const = 0;
    virtual ~ShapeFactory() = default;
    void render() const { create()->draw(); }
};

class CircleFactory : public ShapeFactory {
public:
    std::unique_ptr<Shape> create() const override { return std::make_unique<Circle>(); }
};

class SquareFactory : public ShapeFactory {
public:
    std::unique_ptr<Shape> create() const override { return std::make_unique<Square>(); }
};
```

**Use when:** you don't know ahead of time which exact subtype you need, or want to delegate the choice to subclasses.

---

## Abstract Factory

**Intent:** Provide an interface for creating **families of related objects** without specifying concrete classes.

```cpp
struct IButton { virtual void paint() const = 0; virtual ~IButton() = default; };
struct ICheckbox { virtual void paint() const = 0; virtual ~ICheckbox() = default; };

struct WinButton : IButton   { void paint() const override { /* ... */ } };
struct MacButton : IButton   { void paint() const override { /* ... */ } };

struct IGUIFactory {
    virtual std::unique_ptr<IButton> createButton() const = 0;
    virtual std::unique_ptr<ICheckbox> createCheckbox() const = 0;
    virtual ~IGUIFactory() = default;
};

struct WinFactory : IGUIFactory {
    std::unique_ptr<IButton>   createButton()   const override { return std::make_unique<WinButton>(); }
    std::unique_ptr<ICheckbox> createCheckbox() const override { /* ... */ }
};
```

**Use when:** GUI toolkits, cross-platform rendering, theme systems, mock implementations for tests.

---

## Builder

**Intent:** Separate the construction of a complex object from its representation, allowing the same construction process to create different representations.

```cpp
class Pizza {
    std::string dough_, sauce_, topping_;
    friend class PizzaBuilder;
public:
    void show() const { std::cout << dough_ << ", " << sauce_ << ", " << topping_ << "\n"; }
};

class PizzaBuilder {
    Pizza p_;
public:
    PizzaBuilder& setDough(std::string d)   { p_.dough_  = std::move(d); return *this; }
    PizzaBuilder& setSauce(std::string s)   { p_.sauce_  = std::move(s); return *this; }
    PizzaBuilder& setTopping(std::string t) { p_.topping_ = std::move(t); return *this; }
    Pizza build() { return std::move(p_); }
};

Pizza p = PizzaBuilder()
            .setDough("thin")
            .setSauce("tomato")
            .setTopping("pepperoni")
            .build();
```

**Use when:** object has many optional parameters; many configurations; construction requires multiple steps.

### Director (Optional)

```cpp
class Cook {
public:
    Pizza makeMargherita() {
        return PizzaBuilder().setDough("thin").setSauce("tomato").setTopping("cheese").build();
    }
};
```

---

## Prototype

**Intent:** Create new objects by **cloning** an existing prototype instead of building from scratch.

```cpp
class Shape {
public:
    virtual std::unique_ptr<Shape> clone() const = 0;
    virtual ~Shape() = default;
};

class Circle : public Shape {
    double r_;
public:
    explicit Circle(double r) : r_(r) {}
    std::unique_ptr<Shape> clone() const override { return std::make_unique<Circle>(*this); }
    double radius() const { return r_; }
};
```

**Use when:** object construction is expensive; you only know the type at runtime.

**Modern C++:** prefer `std::make_unique<T>(*this)` or `T(*this)` copy constructor.

---

## 3. Structural Patterns

## Adapter

**Intent:** Convert the interface of a class into another interface the client expects. Lets classes work together that otherwise couldn't.

```cpp
// Legacy: uses a string to identify a payment method
class LegacyPayment {
public:
    void payWithToken(const std::string& token, double amount) { /* ... */ }
};

// New interface the rest of the codebase uses
class IPayment {
public:
    virtual void pay(double amount) = 0;
    virtual ~IPayment() = default;
};

class PaymentAdapter : public IPayment {
    LegacyPayment legacy_;
    std::string token_;
public:
    PaymentAdapter(LegacyPayment l, std::string token)
        : legacy_(std::move(l)), token_(std::move(token)) {}
    void pay(double amount) override { legacy_.payWithToken(token_, amount); }
};
```

**Variants:**
- **Object adapter** (composition, shown above) — preferred.
- **Class adapter** (multiple inheritance) — less common.

**Use when:** integrating legacy code, third-party APIs, normalizing interfaces.

---

## Decorator

**Intent:** Attach additional responsibilities to an object **dynamically**, providing a flexible alternative to subclassing for extending behavior.

```cpp
class Coffee {
public:
    virtual double cost() const = 0;
    virtual std::string description() const = 0;
    virtual ~Coffee() = default;
};

class BasicCoffee : public Coffee {
public:
    double cost() const override { return 5.0; }
    std::string description() const override { return "coffee"; }
};

class CoffeeDecorator : public Coffee {
protected:
    std::unique_ptr<Coffee> inner_;
public:
    explicit CoffeeDecorator(std::unique_ptr<Coffee> inner) : inner_(std::move(inner)) {}
    double cost() const override { return inner_->cost(); }
    std::string description() const override { return inner_->description(); }
};

class Milk : public CoffeeDecorator {
public:
    using CoffeeDecorator::CoffeeDecorator;
    double cost() const override { return inner_->cost() + 1.0; }
    std::string description() const override { return inner_->description() + ", milk"; }
};

class Sugar : public CoffeeDecorator {
public:
    using CoffeeDecorator::CoffeeDecorator;
    double cost() const override { return inner_->cost() + 0.5; }
    std::string description() const override { return inner_->description() + ", sugar"; }
};

auto c = std::make_unique<Sugar>(
            std::make_unique<Milk>(
              std::make_unique<BasicCoffee>()));
std::cout << c->description() << " = $" << c->cost();   // coffee, milk, sugar = $6.5
```

**Use when:** I/O streams, GUI window decorations, middleware chains.

**Gotcha:** many small classes; ordering matters; debugging can be confusing.

---

## Facade

**Intent:** Provide a **unified, higher-level interface** to a set of interfaces in a subsystem, making it easier to use.

```cpp
class CPU { public: void freeze(); void execute(); };
class Memory { public: void load(long pos, const std::vector<char>&); };
class Disk { public: std::vector<char> read(long pos, int sz); };

class ComputerFacade {
    CPU cpu_;
    Memory ram_;
    Disk disk_;
public:
    void start() {
        cpu_.freeze();
        ram_.load(0, disk_.read(0, 1024));
        cpu_.execute();
    }
};
```

**Use when:** simplifying complex subsystems, hiding implementation, providing entry points.

**Facade vs Adapter:** Facade simplifies a *whole subsystem*; Adapter makes *one* class fit an interface.

---

## Proxy

**Intent:** Provide a **surrogate** for another object to control access to it.

### Variants

| Variant | Intent |
|---------|--------|
| **Virtual proxy** | Lazy creation of expensive object |
| **Protection proxy** | Access control based on caller |
| **Remote proxy** | Local representative for a remote object |
| **Logging / caching proxy** | Side-effect wrappers |

```cpp
class IService {
public:
    virtual void request() = 0;
    virtual ~IService() = default;
};

class RealService : public IService {
public:
    void request() override { /* expensive */ }
};

class CachingProxy : public IService {
    std::unique_ptr<RealService> real_;
    std::optional<std::string> cached_;
public:
    void request() override {
        if (!real_) real_ = std::make_unique<RealService>();
        if (!cached_) cached_ = "computed";
        std::cout << "result: " << *cached_ << "\n";
    }
};
```

**Use when:** lazy load, security, logging, distributed RPC, ORM lazy loading.

**Decorator vs Proxy:** Decorator **adds** behavior; Proxy **controls access** (typically same interface, often no behavior change).

---

## Composite

**Intent:** Compose objects into **tree structures** to represent part-whole hierarchies. Let clients treat individual objects and compositions uniformly.

```cpp
class FileSystemNode {
public:
    virtual ~FileSystemNode() = default;
    virtual size_t size() const = 0;
    virtual void print(int indent = 0) const = 0;
};

class File : public FileSystemNode {
    std::string name_;
    size_t bytes_;
public:
    File(std::string n, size_t b) : name_(std::move(n)), bytes_(b) {}
    size_t size() const override { return bytes_; }
    void print(int indent) const override {
        std::cout << std::string(indent, ' ') << name_ << " (" << bytes_ << "B)\n";
    }
};

class Directory : public FileSystemNode {
    std::string name_;
    std::vector<std::unique_ptr<FileSystemNode>> children_;
public:
    explicit Directory(std::string n) : name_(std::move(n)) {}
    Directory& add(std::unique_ptr<FileSystemNode> c) {
        children_.push_back(std::move(c));
        return *this;
    }
    size_t size() const override {
        size_t total = 0;
        for (const auto& c : children_) total += c->size();
        return total;
    }
    void print(int indent) const override {
        std::cout << std::string(indent, ' ') << name_ << "/\n";
        for (const auto& c : children_) c->print(indent + 2);
    }
};
```

**Use when:** file systems, GUI trees, organizational charts, ASTs.

---

## Bridge

**Intent:** Decouple an **abstraction** from its **implementation** so they can vary independently.

```cpp
class IDrawingAPI {
public:
    virtual void drawCircle(double x, double y, double r) = 0;
    virtual ~IDrawingAPI() = default;
};

class OpenGLAPI : public IDrawingAPI {
public:
    void drawCircle(double x, double y, double r) override { /* ... */ }
};

class DirectXAPI : public IDrawingAPI {
public:
    void drawCircle(double x, double y, double r) override { /* ... */ }
};

class Shape {
protected:
    IDrawingAPI& api_;
public:
    explicit Shape(IDrawingAPI& api) : api_(api) {}
    virtual void draw() = 0;
    virtual ~Shape() = default;
};

class Circle : public Shape {
    double x_, y_, r_;
public:
    Circle(IDrawingAPI& api, double x, double y, double r)
        : Shape(api), x_(x), y_(y), r_(r) {}
    void draw() override { api_.drawCircle(x_, y_, r_); }
};
```

**Use when:** multiple dimensions of variation (Shape × RenderingAPI, Persistence × Storage).

**Bridge vs Strategy:** Bridge decouples abstraction **and** implementation; Strategy lets you **swap** an algorithm at runtime.

---

## Flyweight

**Intent:** Use **sharing** to support large numbers of fine-grained objects efficiently.

```cpp
class TreeType {
    std::string name_;
    std::string color_;
    std::string texture_;          // large bitmap
public:
    TreeType(std::string n, std::string c, std::string t)
        : name_(std::move(n)), color_(std::move(c)), texture_(std::move(t)) {}
    void draw(int x, int y) const { /* uses intrinsic state shared across many trees */ }
};

class TreeFactory {
    std::unordered_map<std::string, std::shared_ptr<TreeType>> cache_;
public:
    std::shared_ptr<TreeType> get(const std::string& name, const std::string& color) {
        std::string key = name + ":" + color;
        auto it = cache_.find(key);
        if (it == cache_.end()) {
            cache_[key] = std::make_shared<TreeType>(name, color, "huge-bitmap");
        }
        return cache_[key];
    }
};

class Tree {
    int x_, y_;                    // extrinsic state
    std::shared_ptr<TreeType> type_;
public:
    Tree(int x, int y, std::shared_ptr<TreeType> t) : x_(x), y_(y), type_(std::move(t)) {}
    void draw() const { type_->draw(x_, y_); }
};
```

**Use when:** millions of similar objects; in-memory caches; particle systems; glyphs in editors.

---

## 4. Behavioral Patterns

## Observer

**Intent:** Define a **one-to-many dependency** so that when one object changes state, all dependents are notified.

```cpp
class IObserver {
public:
    virtual void update(int value) = 0;
    virtual ~IObserver() = default;
};

class Subject {
    int value_ = 0;
    std::vector<IObserver*> observers_;
public:
    void attach(IObserver* o) { observers_.push_back(o); }
    void setValue(int v) {
        value_ = v;
        for (auto* o : observers_) o->update(value_);
    }
};

class Printer : public IObserver {
public:
    void update(int v) override { std::cout << "new value: " << v << "\n"; }
};
```

**Use when:** event listeners, model-view-controller, pub/sub systems, reactive streams.

**Modern C++ alternative:** `std::function` callbacks, or use **signals2** (Boost.Signals2) for thread-safe pub/sub.

### Threading Notes

- Multi-threaded subscribers → use a queue per observer or a thread-safe queue.
- Subject needs to manage observer lifetime (often raw pointers + explicit detach).

---

## Strategy

**Intent:** Define a family of algorithms, encapsulate each one, and make them **interchangeable**.

```cpp
class ISortStrategy {
public:
    virtual void sort(std::vector<int>& v) = 0;
    virtual ~ISortStrategy() = default;
};

class QuickSort : public ISortStrategy {
public:
    void sort(std::vector<int>& v) override { /* ... */ }
};

class MergeSort : public ISortStrategy {
public:
    void sort(std::vector<int>& v) override { /* ... */ }
};

class Sorter {
    std::unique_ptr<ISortStrategy> strategy_;
public:
    explicit Sorter(std::unique_ptr<ISortStrategy> s) : strategy_(std::move(s)) {}
    void sort(std::vector<int>& v) { strategy_->sort(v); }
};
```

**Use when:** algorithm choice at runtime; multiple ways to perform an operation; conditional logic.

**Strategy vs State:** Strategy = external algorithm choice; State = internal state transitions drive behavior.

### Modern Alternative

```cpp
using SortFn = std::function<void(std::vector<int>&)>;
void sortWith(std::vector<int>& v, SortFn fn) { fn(v); }
```

---

## State

**Intent:** Allow an object to **alter its behavior when its internal state changes** — the object will appear to change its class.

```cpp
class VendingMachine;     // forward

class IState {
public:
    virtual void insertCoin(VendingMachine& vm, int cents) = 0;
    virtual void selectItem(VendingMachine& vm) = 0;
    virtual ~IState() = default;
};

class VendingMachine {
    std::unique_ptr<IState> state_;
    int balance_ = 0;
public:
    explicit VendingMachine(std::unique_ptr<IState> s) : state_(std::move(s)) {}
    void setState(std::unique_ptr<IState> s) { state_ = std::move(s); }
    void insertCoin(int c) { state_->insertCoin(*this, c); }
    void selectItem()      { state_->selectItem(*this); }
    int  balance() const   { return balance_; }
    void addBalance(int c) { balance_ += c; }
};

class NoCoinState : public IState {
public:
    void insertCoin(VendingMachine& vm, int c) override {
        vm.addBalance(c);
        vm.setState(std::make_unique<HasCoinState>());
    }
    void selectItem(VendingMachine&) override {
        std::cout << "insert a coin first\n";
    }
};

class HasCoinState : public IState {
public:
    void insertCoin(VendingMachine& vm, int c) override { vm.addBalance(c); }
    void selectItem(VendingMachine& vm) override {
        std::cout << "dispensing (balance=" << vm.balance() << ")\n";
        vm.setState(std::make_unique<NoCoinState>());
    }
};
```

**Use when:** parsers, network connections, document workflows, UI controllers.

**State vs Strategy:** Both are substitution at runtime. State changes are **driven by internal transitions**; Strategy is **chosen by client**.

---

## Template Method

**Intent:** Define the **skeleton of an algorithm** in a base class, letting subclasses override specific steps without changing the structure.

```cpp
class DataParser {
public:
    void parse() {
        openFile();
        std::string raw = readData();
        auto rows = parseData(raw);  // customization point
        analyze(rows);
        closeFile();
    }
    virtual ~DataParser() = default;
protected:
    virtual std::vector<Row> parseData(const std::string& raw) = 0;
    void openFile()   { /* default impl */ }
    void readData()   { /* default impl */ }
    void analyze(const std::vector<Row>&) { /* default impl */ }
    void closeFile()  { /* default impl */ }
};

class CSVParser : public DataParser {
protected:
    std::vector<Row> parseData(const std::string& raw) override {
        // split by commas
    }
};
```

**Modern alternative:** NVI (Non-Virtual Interface) — public non-virtual, private virtual hooks.

---

## Command

**Intent:** Encapsulate a **request as an object**, thereby allowing parameterization, queuing, logging, and undo.

```cpp
class ICommand {
public:
    virtual void execute() = 0;
    virtual void undo()    = 0;
    virtual ~ICommand() = default;
};

class TextEditor {
    std::string text_;
public:
    void insert(const std::string& s, size_t pos) { text_.insert(pos, s); }
    void erase(size_t pos, size_t n) { text_.erase(pos, n); }
    const std::string& get() const { return text_; }
};

class InsertCommand : public ICommand {
    TextEditor& editor_;
    std::string s_;
    size_t pos_;
public:
    InsertCommand(TextEditor& e, std::string s, size_t p)
        : editor_(e), s_(std::move(s)), pos_(p) {}
    void execute() override { editor_.insert(s_, pos_); }
    void undo()    override { editor_.erase(pos_, s_.size()); }
};
```

**Use when:** undo/redo, transaction queues, task schedulers, GUI actions, macro recording.

---

## Iterator

**Intent:** Provide a way to access elements of an aggregate object **sequentially** without exposing its underlying representation.

The C++ STL does this natively via iterators. Container types define their own iterator categories.

```cpp
class IntRange {
    int cur_, end_;
public:
    IntRange(int start, int end) : cur_(start), end_(end) {}
    class iterator {
        int v_;
    public:
        explicit iterator(int v) : v_(v) {}
        int  operator*() const { return v_; }
        iterator& operator++() { ++v_; return *this; }
        bool operator!=(const iterator& o) const { return v_ != o.v_; }
    };
    iterator begin() const { return iterator(cur_); }
    iterator end()   const { return iterator(end_); }
};

for (int x : IntRange(1, 5)) std::cout << x;   // 1 2 3 4
```

**Use when:** traversal without exposing structure; multiple simultaneous traversals; uniform interface across containers.

---

## Mediator

**Intent:** Define an object that **encapsulates how a set of objects interact**, promoting loose coupling by keeping objects from referring to each other explicitly.

```cpp
class IColleague;     // forward

class IMediator {
public:
    virtual void send(const std::string& msg, IColleague* sender) = 0;
    virtual ~IMediator() = default;
};

class IColleague {
protected:
    IMediator* med_;
public:
    explicit IColleague(IMediator* m) : med_(m) {}
    void send(const std::string& msg) { med_->send(msg, this); }
    virtual void receive(const std::string& msg) = 0;
    virtual ~IColleague() = default;
};

class ChatMediator : public IMediator {
    std::vector<IColleague*> peers_;
public:
    void add(IColleague* p) { peers_.push_back(p); }
    void send(const std::string& msg, IColleague* sender) override {
        for (auto* p : peers_) if (p != sender) p->receive(msg);
    }
};
```

**Use when:** GUI dialogs (Form is the mediator between widgets), chat rooms, air traffic control.

**Mediator vs Observer:** Mediator **centralizes** communication; Observer **broadcasts** to subscribers.

---

## Memento

**Intent:** Without violating encapsulation, **capture and externalize** an object's internal state so it can be restored later.

```cpp
class EditorMemento {
    std::string state_;
public:
    explicit EditorMemento(std::string s) : state_(std::move(s)) {}
    const std::string& state() const { return state_; }
};

class Editor {
    std::string content_;
public:
    void type(const std::string& s) { content_ += s; }
    const std::string& content() const { return content_; }
    EditorMemento save()    const { return EditorMemento(content_); }
    void restore(const EditorMemento& m) { content_ = m.state(); }
};

class History {
    std::vector<EditorMemento> snaps_;
public:
    void push(const EditorMemento& m) { snaps_.push_back(m); }
    EditorMemento pop() { auto m = snaps_.back(); snaps_.pop_back(); return m; }
};
```

**Use when:** undo/redo, snapshots, transactional rollback.

---

## Visitor

**Intent:** Represent an operation to be performed on elements of an object structure. Visitor lets you **define a new operation without changing the classes** of the elements.

```cpp
class Circle, Square;       // forward

class IShape {
public:
    virtual void accept(class IVisitor& v) = 0;
    virtual ~IShape() = default;
};

class IVisitor {
public:
    virtual void visit(class Circle& c) = 0;
    virtual void visit(class Square& s) = 0;
    virtual ~IVisitor() = default;
};

class Circle : public IShape {
    double r_;
public:
    explicit Circle(double r) : r_(r) {}
    double radius() const { return r_; }
    void accept(IVisitor& v) override { v.visit(*this); }
};

class Square : public IShape {
    double s_;
public:
    explicit Square(double s) : s_(s) {}
    double side() const { return s_; }
    void accept(IVisitor& v) override { v.visit(*this); }
};

class AreaVisitor : public IVisitor {
public:
    double total = 0;
    void visit(Circle& c) override { total += 3.14159 * c.radius() * c.radius(); }
    void visit(Square& s) override { total += s.side() * s.side(); }
};

class DrawVisitor : public IVisitor {
public:
    void visit(Circle&) override { /* render circle */ }
    void visit(Square&) override { /* render square */ }
};
```

**Use when:** compilers (AST visitors), serializers, multiple operations over a stable object hierarchy.

**Trade-off:** adding a new **element type** requires updating every visitor.

---

## Chain of Responsibility

**Intent:** Avoid coupling the sender of a request to its receiver by giving **multiple objects a chance to handle** the request. Chain the receiving objects and pass the request along until one handles it.

```cpp
class Handler {
protected:
    std::unique_ptr<Handler> next_;
public:
    explicit Handler(std::unique_ptr<Handler> n = nullptr) : next_(std::move(n)) {}
    virtual ~Handler() = default;
    void handle(int level) {
        if (process(level)) return;
        if (next_) next_->handle(level);
    }
protected:
    virtual bool process(int level) = 0;
};

class InfoHandler : public Handler {
public:
    using Handler::Handler;
protected:
    bool process(int level) override {
        if (level >= 1) { std::cout << "info\n"; return true; }
        return false;
    }
};

class WarnHandler : public Handler {
public:
    using Handler::Handler;
protected:
    bool process(int level) override {
        if (level >= 3) { std::cout << "warning\n"; return true; }
        return false;
    }
};

auto chain = std::make_unique<InfoHandler>(
                std::make_unique<WarnHandler>());
chain->handle(2);     // info
chain->handle(5);     // info (caught at first level) or warning depending on flow
```

**Use when:** logging frameworks, middleware chains, event bubbling, exception handling.

---

## 5. Concurrency Patterns (Modern Bonus)

### Producer-Consumer

```cpp
#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class BlockingQueue {
    std::queue<T> q_;
    std::mutex m_;
    std::condition_variable cv_;
public:
    void push(T v) {
        {
            std::lock_guard lk(m_);
            q_.push(std::move(v));
        }
        cv_.notify_one();
    }
    T pop() {
        std::unique_lock lk(m_);
        cv_.wait(lk, [&]{ return !q_.empty(); });
        T v = std::move(q_.front());
        q_.pop();
        return v;
    }
};
```

### Active Object (Actor)

A thread owns a queue; all operations enqueue tasks to it. No shared state.

### Thread Pool

```cpp
class ThreadPool {
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::mutex m_;
    std::condition_variable cv_;
    bool stop_ = false;
public:
    explicit ThreadPool(size_t n) {
        for (size_t i = 0; i < n; ++i) {
            workers_.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock lk(m_);
                        cv_.wait(lk, [&]{ return stop_ || !tasks_.empty(); });
                        if (stop_ && tasks_.empty()) return;
                        task = std::move(tasks_.front());
                        tasks_.pop();
                    }
                    task();
                }
            });
        }
    }
    // enqueue / dtor
};
```

---

## 6. Anti-Patterns to Avoid

| Anti-Pattern | Symptom | Fix |
|--------------|---------|-----|
| **God Object** | One class does everything | Apply SRP, split |
| **Object-Relational Mapping everywhere** | Anemic domain models | Mix behavior with data |
| **Singleton Abuse** | Hidden globals | Inject a single shared instance |
| **Spaghetti Inheritance** | Deep, rigid hierarchies | Prefer composition |
| **Inner Platform Effect** | Re-implementing language features | Use the language |
| **Magic Numbers/Strings** | Hardcoded constants | Named constants |
| **Copy-Paste Programming** | Duplicated logic | Extract method |
| **Premature Optimization** | Complex code for no measured win | Profile first |
| **Speculative Generality** | YAGNI violations | Delete it |

---

## 7. Quick Reference Card

| Category | Pattern | Intent |
|----------|---------|--------|
| Creational | Singleton | One instance, global access |
| Creational | Factory Method | Subclass picks class to instantiate |
| Creational | Abstract Factory | Family of related objects |
| Creational | Builder | Step-by-step construction |
| Creational | Prototype | Clone existing instance |
| Structural | Adapter | Bridge incompatible interfaces |
| Structural | Decorator | Add behavior dynamically |
| Structural | Facade | Simplified subsystem entry |
| Structural | Proxy | Surrogate controlling access |
| Structural | Composite | Tree of objects, uniform |
| Structural | Bridge | Decouple abstraction from impl |
| Structural | Flyweight | Share many small objects |
| Behavioral | Observer | Notify dependents on change |
| Behavioral | Strategy | Swap algorithms at runtime |
| Behavioral | State | Behavior changes with state |
| Behavioral | Template Method | Skeleton in base, steps in subclass |
| Behavioral | Command | Request as object |
| Behavioral | Iterator | Sequential access |
| Behavioral | Mediator | Centralize complex comms |
| Behavioral | Memento | Capture/restore state |
| Behavioral | Visitor | Add ops without changing elements |
| Behavioral | Chain of Responsibility | Pass request along a chain |

---

## 8. Top Interview Q&A (50 entries)

**Q1: Singleton vs static class?**
Singleton can implement interfaces, be passed as object, lazily initialized, polymorphic. Static class can't.

**Q2: When to use Strategy vs State?**
Strategy: external algorithm choice, doesn't know context. State: internal transitions drive behavior.

**Q3: Decorator vs Proxy?**
Decorator **adds** behavior; Proxy **controls access** (often with no behavior change).

**Q4: Why is Singleton controversial?**
Hidden dependencies, hard to test, global state, threading concerns. Modern preference is to inject a single shared instance.

**Q5: Difference between Adapter and Facade?**
Adapter makes **one** class fit an existing interface (wraps one). Facade simplifies a **whole subsystem** (wraps many).

**Q6: Factory Method vs Abstract Factory?**
Factory Method: one product, subclass picks the concrete class. Abstract Factory: family of products (multiple methods), often one factory per family.

**Q7: Builder vs telescoping constructor?**
Builder avoids ambiguous, unreadable constructors with many optional parameters and supports partial construction.

**Q8: Composite vs Decorator?**
Both use recursive composition. Composite emphasizes **treating parts and wholes uniformly** (no new behavior). Decorator emphasizes **adding behavior** to specific objects.

**Q9: Bridge vs Adapter?**
Adapter retrofits an existing class to fit a client's interface (after-the-fact). Bridge is designed upfront to keep abstraction and implementation independent.

**Q10: When is Flyweight worth it?**
Thousands/millions of similar objects where most fields are shared. The shared part becomes the flyweight; per-instance state stays extrinsic.

**Q11: When would you use Prototype over Factory?**
When construction is expensive, you don't know the exact type at compile time, or you want to avoid parallel class hierarchies.

**Q12: Visitor's main trade-off?**
Easy to add **new operations**, hard to add **new element types** (every visitor must change).

**Q13: Memento vs Snapshot?**
Memento is the GoF pattern; "snapshot" is a generic term. Memento typically has a narrow API (only originator can read state).

**Q14: Chain of Responsibility vs Decorator?**
CoR chooses **which handler** runs; Decorator always runs all wrappers. Both link objects in a chain.

**Q15: How does Observer compare to pub/sub?**
Observer is **synchronous** by default (subject calls observers directly). Pub/sub is **decoupled** via a broker/event bus (often async, often cross-process).

**Q16: Template Method vs Strategy?**
Template Method: skeleton in base class, subclass overrides steps (compile-time choice). Strategy: choose algorithm at runtime via composition.

**Q17: How do you break the Singleton testability problem?**
Inject the same instance via DI; provide an `IDatabase` interface and a default singleton-backed impl. Tests use a mock.

**Q18: Is Singleton thread-safe in C++?**
Function-local `static` is thread-safe in C++11+. Class-static still requires `std::call_once` or `std::atomic`.

**Q19: How does Mediator differ from Facade?**
Facade **simplifies** (one-way, no back-objects) — clients call into Facade. Mediator **coordinates** (multi-way, colleagues call each other through it).

**Q20: What's an iterator category and why does it matter?**
Tags (`input_iterator_tag`, `random_access_iterator_tag`) let generic algorithms pick the right strategy. `vector::iterator` is random access; `list::iterator` is bidirectional.

**Q21: When is Command overkill?**
For a single one-shot operation; for GUI actions it's perfect, but for "log this string" it's noise.

**Q22: How do you undo with Command?**
Each `ICommand` implements `execute()` and `undo()`. A history stack stores executed commands; undo pops and calls `undo()`.

**Q23: Bridge: real example?**
GUI toolkit providing windows on different OSes. Abstraction (`Window`) holds a reference to the implementation (`WindowImpl`); both hierarchies vary independently.

**Q24: What is the Open/Closed principle's relation to Decorator?**
Decorator is a textbook OCP — new behavior is added without modifying the original class.

**Q25: Memento: when to use?**
Editor undo/redo, transactional rollback, snapshot creation, game save states.

**Q26: Composite: file system example?**
```cpp
class Node { virtual size_t size() const = 0; };
class File : public Node { size_t size() const override { return bytes_; } };
class Directory : public Node {
    vector<unique_ptr<Node>> children;
    size_t size() const override { return sum of children; }
};
```

**Q27: How does Visitor implement double dispatch?**
Calling `element.accept(visitor)` triggers `visitor.visit(element)`. The runtime type of the element and the static type of the visitor both determine behavior — that's double dispatch.

**Q28: What is the "magic static" in C++?**
A function-local `static T x;` initialized on first call. Thread-safe since C++11.

**Q29: When to use Flyweight?**
Particle systems, character glyphs in editors, game trees, large graphs with repeated nodes.

**Q30: Adapter pattern: class vs object?**
Class adapter uses multiple inheritance to bind to two types; object adapter composes one and forwards calls. Object adapter is more flexible.

**Q31: Real-world Proxy examples?**
- Virtual proxy: `std::shared_ptr` lazy loads.
- Remote proxy: gRPC stub.
- Protection: `std::unique_ptr` with deleter.
- Caching: ORM lazy loading.

**Q32: When to use Prototype?**
Document editors copy-paste, game object spawning, when classes differ only by state.

**Q33: What's an "abstract factory of builders"?**
Abstract Factory returns a Builder for the chosen family. Used for very complex object creation with multiple variants.

**Q34: How does Strategy differ from a function pointer?**
Function pointer is a **single** signature; Strategy is a class that may carry **state** and implement an **interface** with multiple related methods.

**Q35: When NOT to use Singleton?**
When you need multiple instances for tests or in different contexts, when state must be parameterized, when lifecycle is unclear. Use a regular class instance with DI.

**Q36: What is the "Hollywood Principle"?**
"Don't call us, we'll call you." Inversion of control — frameworks call your code, not the other way around. Template Method, Observer, DI are all flavors.

**Q37: What is the Liskov Substitution Principle's link to LSP?**
Patterns like Template Method can violate LSP if subclass overrides change the algorithm's meaning. Subclasses must extend, not contradict.

**Q38: When is the Visitor pattern inappropriate?**
When the element hierarchy **changes often** — every new element requires updating every visitor.

**Q39: Can you combine Observer and Mediator?**
Yes. Mediator broadcasts events through an Observer-style subscription; observers receive messages and update UI.

**Q40: State: real-world example?**
Vending machine (NoCoin → HasCoin → Dispensing → NoCoin), TCP connection states, document workflow (Draft → Review → Approved).

**Q41: What's a "smart pointer" pattern?**
Proxy pattern providing scoped ownership; in C++ via RAII and `unique_ptr`/`shared_ptr`.

**Q42: How do you implement undo/redo cleanly?**
Command pattern: every action is an object with `execute()` and `undo()`. History stacks store commands; replay to redo.

**Q43: What's the Null Object pattern?**
Provide a no-op default implementation so you never need `nullptr` checks. e.g., `NullLogger` that does nothing instead of branching on `if (logger)`.

**Q44: Difference between State and Polymorphism?**
State is **about transitions**; ordinary polymorphism is **just substitution**. State machine models the **transitions** explicitly.

**Q45: Composite: when does it backfire?**
When leaf and composite semantics differ a lot. e.g., trying to model "restaurants" and "menus" with the same operations may force awkward APIs.

**Q46: How does Memento preserve encapsulation?**
Only the originator can create and restore the memento's internal state. External code holds the memento but cannot read its contents.

**Q47: What's a "design smell" related to Visitor?**
If you find yourself adding `if (type == ...) { ... }` blocks, you might need Visitor or a Strategy.

**Q48: What pattern fits "many middlewares"?**
Chain of Responsibility (e.g., HTTP middleware pipeline: auth → logging → rate-limit → handler).

**Q49: Adapter: name a real-world use?**
Wrapping a legacy C API to expose a modern C++ interface. Wrapping a third-party library that doesn't match your project's interfaces.

**Q50: Design pattern vs architectural pattern?**
Design patterns are class/object-level. Architectural patterns are system-level (microservices, event-driven, layered). Examples: MVC, hexagonal, CQRS.

---

> **End of CS fundamentals.** With these 7 files — OS, DBMS, Networks, OOP, C++ Specifics, SOLID, and Patterns — you have a thorough foundation for any CS round. Move on to **`03-system-design/`** to practice scaling, sharding, and classic problems.