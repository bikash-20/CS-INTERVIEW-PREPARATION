# Backend Interview Preparation

This file covers the most-asked questions in backend developer interviews. It is split into three sections:

1. **Node.js + Express** — most popular for JavaScript-based backends.
2. **Java + Spring Boot** — enterprise standard.
3. **Python + Django / FastAPI** — popular for APIs, data, and ML backends.

Common topics (REST, databases, auth, caching, queues, scaling) are covered in each section with examples.

---

## Table of Contents

- [Part 1 — Common Backend Concepts](#part-1--common-backend-concepts)
- [Part 2 — Node.js + Express](#part-2--nodejs--express)
- [Part 3 — Java + Spring Boot](#part-3--java--spring-boot)
- [Part 4 — Python + Django / FastAPI](#part-4--python--django--fastapi)
- [Part 5 — Coding Challenges](#part-5--coding-challenges)

---

## Part 1 — Common Backend Concepts

These appear in every backend interview regardless of stack.

### Q1: What is REST? What makes an API RESTful?
**Answer:** REST (Representational State Transfer) is an architectural style for designing networked applications. A RESTful API:
- Uses **HTTP methods** meaningfully (`GET`, `POST`, `PUT`, `DELETE`).
- Uses **stateless** requests.
- Returns data in formats like **JSON** or **XML**.
- Has clear **resource URLs** (`/users/123`).

```http
GET    /users         → list users
GET    /users/123     → get user 123
POST   /users         → create user
PUT    /users/123     → update user 123
DELETE /users/123     → delete user 123
```

### Q2: What is the difference between SQL and NoSQL?
**Answer:**
| Aspect | SQL | NoSQL |
|--------|-----|-------|
| Schema | Fixed | Flexible |
| Scaling | Vertical mostly | Horizontal |
| ACID | Yes | Often eventual |
| Query | SQL language | API or limited |
| Best for | Transactions, joins | Scale, flexible data |

Examples: PostgreSQL, MySQL (SQL); MongoDB, Redis, Cassandra (NoSQL).

### Q3: What is ACID?
**Answer:**
- **Atomicity** — all or nothing.
- **Consistency** — DB stays in a valid state.
- **Isolation** — concurrent transactions don't interfere.
- **Durability** — committed data survives crashes.

### Q4: What is indexing? How does it work?
**Answer:** An index is a separate data structure (usually B-Tree) that speeds up lookups. Trade-off: faster reads, slower writes, extra storage.

```sql
CREATE INDEX idx_users_email ON users(email);
```

### Q5: What is N+1 query problem? How to fix it?
**Answer:** You run 1 query to fetch a list, then N more queries to fetch related data for each item. Fix by **eager loading** or **joining**.

```js
// Bad: 1 + N queries
users.forEach(u => getPosts(u.id));
// Good: 1 query with join
SELECT u.*, p.* FROM users u JOIN posts p ON p.user_id = u.id;
```

### Q6: What is JWT? How does it work?
**Answer:** JSON Web Token. A self-contained token with three parts (header, payload, signature), separated by dots. Used for stateless authentication.

```js
// Header.Payload.Signature
eyJhbGciOiJIUzI1NiJ9.eyJ1c2VyIjoiYWxpY2UifQ.signature
```

Flow: User logs in → server returns JWT → client sends JWT in `Authorization` header on every request → server verifies signature.

### Q7: What is OAuth 2.0?
**Answer:** An authorization framework. Lets a user grant a third-party app limited access to their resources without sharing passwords. Roles: resource owner, client, authorization server, resource server.

### Q8: What is caching? Where can you cache?
**Answer:** Caching stores frequently-used data in a fast layer to reduce load.
- **Browser cache** — static assets.
- **CDN** — content at the edge.
- **Application cache** — in-memory (Caffeine, Guava).
- **Distributed cache** — Redis, Memcached.
- **Database cache** — query cache, materialized views.

### Q9: What is a message queue? Why use one?
**Answer:** Async communication between services. Producer sends messages, consumer processes them later. Benefits: decoupling, retries, buffering, async work.

Examples: RabbitMQ, Kafka, AWS SQS.

### Q10: What is the difference between authentication and authorization?
**Answer:**
- **Authentication** — Who are you? (Login, verify identity)
- **Authorization** — What can you do? (Permissions, roles)

### Q11: What is a load balancer?
**Answer:** A server that distributes incoming requests across multiple backend servers. Algorithms: round-robin, least connections, IP hash.

### Q12: What is horizontal vs vertical scaling?
**Answer:**
- **Vertical** — add more power (CPU/RAM) to one server.
- **Horizontal** — add more servers.

### Q13: What is database sharding?
**Answer:** Splitting data across multiple databases by some key (e.g., user_id). Each shard holds a subset of the data. Used for very large datasets.

### Q14: What is a microservice?
**Answer:** An architectural style where the app is split into small, independent services. Each owns its own data, can be deployed independently, and communicates via HTTP or messaging.

### Q15: What is the CAP theorem?
**Answer:** In a distributed system, you can have at most 2 of 3:
- **C**onsistency — every read sees latest write.
- **A**vailability — every request gets a response.
- **P**artition tolerance — works despite network failures.

Since network partitions happen, you choose between CP and AP.

### Q16: What is an idempotent API?
**Answer:** An API that produces the same result when called multiple times as it does when called once. Important for retries.

`PUT` and `DELETE` are idempotent by design. `POST` is not.

### Q17: What is a rate limiter? Common algorithms?
**Answer:** Limits the number of requests a user can make in a time window.
- **Token bucket** — tokens refill at a rate; consume 1 per request.
- **Leaky bucket** — process at a fixed rate.
- **Fixed window** — count per window.
- **Sliding window** — more accurate.

### Q18: What is CORS?
**Answer:** Cross-Origin Resource Sharing. Browser security that blocks requests to a different origin unless the server allows it via response headers.

```
Access-Control-Allow-Origin: https://mysite.com
Access-Control-Allow-Methods: GET, POST
```

### Q19: What is HTTPS? What does TLS do?
**Answer:** HTTPS is HTTP over TLS. TLS provides:
- **Encryption** — data is private.
- **Authentication** — server is who it claims (via certificates).
- **Integrity** — data is not tampered with.

### Q20: What is a CDN?
**Answer:** Content Delivery Network. A geographically distributed set of servers that cache static content close to users. Reduces latency and load on the origin.

---

## Part 2 — Node.js + Express

### 2.1 Fundamentals

### Q1: What is Node.js?
**Answer:** A runtime that lets you run JavaScript outside the browser, on the server. Built on **V8** (Chrome's JS engine) and uses an **event-driven, non-blocking I/O model**.

### Q2: What is the event loop?
**Answer:** The mechanism Node.js uses to handle async operations despite being single-threaded. Phases:
1. Timers (`setTimeout`, `setInterval`)
2. Pending callbacks
3. Poll (I/O)
4. Check (`setImmediate`)
5. Close callbacks

Microtasks (Promises) run between each phase.

### Q3: What is `process.nextTick` vs `setImmediate`?
**Answer:**
- `process.nextTick(fn)` — runs **after** current operation, **before** the next event loop phase. Highest priority.
- `setImmediate(fn)` — runs in the **check** phase of the next loop. Lower priority than `nextTick`.

### Q4: What is the difference between `require` and `import`?
**Answer:**
- `require` — CommonJS, synchronous, dynamic.
- `import` — ES Modules, static, supports tree-shaking.

### Q5: What is `npm`? What is `npx`?
**Answer:**
- **npm** — package manager for Node.js.
- **npx** — runs a package without installing it globally.

### Q6: What is a `package.json`?
**Answer:** A file that describes a Node project: name, version, dependencies, scripts.

```json
{
  "name": "myapp",
  "version": "1.0.0",
  "scripts": { "start": "node index.js" },
  "dependencies": { "express": "^4.18.0" }
}
```

### Q7: What is a middleware in Express?
**Answer:** A function that runs during the request-response cycle. It has access to `req`, `res`, and `next`.

```js
app.use((req, res, next) => {
  console.log(req.method, req.url);
  next();
});
```

### Q8: How do you handle errors in Express?
**Answer:** Use an error-handling middleware (4 args).

```js
app.use((err, req, res, next) => {
  console.error(err);
  res.status(500).json({ error: "Server error" });
});
```

### Q9: What is the difference between `app.use` and `app.get`?
**Answer:**
- `app.use(path, handler)` — runs for **any HTTP method** matching the path.
- `app.get(path, handler)` — runs only for **GET**.

### Q10: How do you parse JSON in Express?
**Answer:**

```js
app.use(express.json());
```

### 2.2 Async & I/O

### Q11: How do you read a file in Node.js?
**Answer:**

```js
// Async (preferred)
const fs = require("fs/promises");
const data = await fs.readFile("file.txt", "utf-8");

// Callback style
fs.readFile("file.txt", "utf-8", (err, data) => { /* ... */ });
```

### Q12: What is the difference between `fs.readFile` and `fs.createReadStream`?
**Answer:**
- `readFile` reads the whole file into memory.
- `createReadStream` reads it in chunks. Better for large files.

### Q13: What is `async/await`?
**Answer:** A cleaner way to write Promise-based code.

```js
async function getUser(id) {
  const res = await fetch(`/api/users/${id}`);
  return res.json();
}
```

### Q14: How do you handle unhandled promise rejections?
**Answer:**

```js
process.on("unhandledRejection", err => {
  console.error(err);
  process.exit(1);
});
```

### Q15: What is backpressure?
**Answer:** When data comes in faster than it can be processed. Node streams let you pause/resume to manage flow.

### 2.3 Building APIs

### Q16: Build a basic Express server.
```js
const express = require("express");
const app = express();
app.use(express.json());

app.get("/api/users", (req, res) => {
  res.json([{ id: 1, name: "Alice" }]);
});

app.post("/api/users", (req, res) => {
  const user = req.body;
  res.status(201).json(user);
});

app.listen(3000, () => console.log("Server running"));
```

### Q17: How do you validate request data?
**Answer:** Use a library like Joi, Zod, or express-validator.

```js
const Joi = require("joi");
const schema = Joi.object({ name: Joi.string().required() });
const { error } = schema.validate(req.body);
```

### Q18: How do you implement authentication with JWT?
```js
const jwt = require("jsonwebtoken");

function auth(req, res, next) {
  const token = req.headers.authorization?.split(" ")[1];
  try {
    req.user = jwt.verify(token, process.env.JWT_SECRET);
    next();
  } catch {
    res.status(401).json({ error: "Unauthorized" });
  }
}

app.get("/me", auth, (req, res) => res.json(req.user));
```

### Q19: How do you connect to a database?
**Answer:** Using a driver or ORM (mongoose for MongoDB, pg for PostgreSQL, prisma, typeorm, sequelize).

```js
const mongoose = require("mongoose");
await mongoose.connect(process.env.MONGO_URL);
```

### Q20: How do you handle file uploads?
**Answer:** Use `multer`.

```js
const multer = require("multer");
const upload = multer({ dest: "uploads/" });
app.post("/upload", upload.single("file"), (req, res) => res.send("ok"));
```

### 2.4 Advanced

### Q21: What is clustering in Node.js?
**Answer:** Use multiple processes to utilize multi-core CPUs. The `cluster` module lets you fork the main process.

```js
const cluster = require("cluster");
if (cluster.isMaster) {
  for (let i = 0; i < 4; i++) cluster.fork();
} else {
  // worker
  app.listen(3000);
}
```

### Q22: What is a worker thread?
**Answer:** A thread for CPU-heavy work without blocking the event loop. Use the `worker_threads` module.

### Q23: What is `process.env`?
**Answer:** An object containing environment variables. Use `dotenv` to load a `.env` file in development.

### Q24: How do you handle CORS in Express?
**Answer:**

```js
const cors = require("cors");
app.use(cors({ origin: "https://mysite.com" }));
```

### Q25: How do you log in a Node app?
**Answer:** Use `winston` or `pino`. Avoid `console.log` in production.

### Q26: How do you monitor a Node app?
**Answer:** Tools: PM2, New Relic, Datadog, Prometheus. Metrics: event loop lag, memory, CPU, request duration.

### Q27: What is the difference between `process.exit(0)` and `process.exit(1)`?
**Answer:** `0` is success, `1` is failure. Used to tell the process manager about the exit reason.

### Q28: How do you write tests in Node.js?
**Answer:** Use Jest or Mocha + Chai. Supertest is popular for HTTP testing.

```js
const request = require("supertest");
test("GET / returns 200", async () => {
  const res = await request(app).get("/");
  expect(res.status).toBe(200);
});
```

### Q29: What is a graceful shutdown?
**Answer:** When the process receives `SIGTERM`, it stops accepting new requests, finishes current ones, then exits.

```js
process.on("SIGTERM", async () => {
  server.close();
  await mongoose.disconnect();
  process.exit(0);
});
```

### Q30: What is `npm` vs `pnpm` vs `yarn`?
**Answer:** All are package managers. pnpm is faster and saves disk space; yarn is fast and Facebook-born; npm is the default.

---

## Part 3 — Java + Spring Boot

### 3.1 Core Java for Interviews

### Q1: What is the difference between `==` and `.equals()`?
**Answer:**
- `==` compares **references** (memory address).
- `.equals()` compares **values**.

```java
String a = new String("hi");
String b = new String("hi");
System.out.println(a == b);       // false
System.out.println(a.equals(b));  // true
```

### Q2: What is the difference between `String`, `StringBuilder`, and `StringBuffer`?
**Answer:**
- `String` is **immutable**. Each modification creates a new object.
- `StringBuilder` is mutable and **not thread-safe**. Fast.
- `StringBuffer` is mutable and **thread-safe** (synchronized). Slower.

### Q3: What are the access modifiers?
**Answer:**
- `private` — only same class.
- `default` (no modifier) — same package.
- `protected` — same package + subclasses.
- `public` — everyone.

### Q4: What is the difference between abstract class and interface?
**Answer:**
- **Abstract class** can have implemented methods, constructors, fields. Single inheritance.
- **Interface** (pre-Java 8) only had abstract methods. Java 8+ allows `default` and `static` methods. A class can implement many interfaces.

### Q5: What is polymorphism?
**Answer:** Same interface, different behavior. Two types:
- **Compile-time** (overloading) — same name, different params.
- **Run-time** (overriding) — subclass changes parent's method.

### Q6: What is the difference between `HashMap` and `Hashtable`?
**Answer:**
- `HashMap` — not synchronized, allows null keys/values, faster.
- `Hashtable` — synchronized, no nulls, legacy class.

### Q7: What is the difference between `ArrayList` and `LinkedList`?
**Answer:**
- `ArrayList` — backed by array. Fast random access (`get(i)`), slow insert/delete in middle.
- `LinkedList` — doubly linked list. Fast insert/delete, slow random access.

### Q8: What is the difference between `final`, `finally`, and `finalize`?
**Answer:**
- `final` — constant or prevents overriding/inheritance.
- `finally` — block that always runs after try/catch.
- `finalize` — old method called by GC before object is destroyed. Deprecated.

### Q9: What is exception handling?
**Answer:**

```java
try {
  int x = 10 / 0;
} catch (ArithmeticException e) {
  System.out.println(e.getMessage());
} finally {
  System.out.println("always runs");
}
```

### Q10: Difference between checked and unchecked exceptions?
**Answer:**
- **Checked** — must be caught or declared (e.g., `IOException`).
- **Unchecked** — extend `RuntimeException` (e.g., `NullPointerException`).

### 3.2 Multithreading & Concurrency

### Q11: How do you create a thread in Java?
**Answer:** Two ways:
```java
// Extend Thread
class MyThread extends Thread {
  public void run() { /* ... */ }
}
new MyThread().start();

// Implement Runnable
Thread t = new Thread(() -> { /* ... */ });
t.start();
```

### Q12: What is `synchronized`?
**Answer:** A keyword that locks an object so only one thread can execute the block at a time.

```java
public synchronized void increment() { count++; }
```

### Q13: What is `volatile`?
**Answer:** Tells the JVM that a variable is shared across threads. Reads/writes go directly to main memory, no caching. Does not provide atomicity.

### Q14: What is the difference between `wait` and `sleep`?
**Answer:**
- `wait()` — releases the lock, called on an object, used for thread communication.
- `sleep()` — does not release the lock, pauses the thread for a time.

### Q15: What is a `ThreadPool` and `ExecutorService`?
**Answer:** Manages a pool of reusable threads instead of creating a new one every time.

```java
ExecutorService pool = Executors.newFixedThreadPool(4);
pool.submit(() -> doWork());
pool.shutdown();
```

### Q16: What is a deadlock?
**Answer:** Two threads each hold a lock the other needs, so both wait forever. Avoid by consistent lock ordering and using `tryLock`.

### 3.3 Spring Boot

### Q17: What is Spring Boot?
**Answer:** A framework for building production-ready Spring applications with minimal config. Provides embedded servers (Tomcat), auto-configuration, starter dependencies.

### Q18: What is the difference between Spring and Spring Boot?
**Answer:**
- **Spring** — full framework with manual configuration.
- **Spring Boot** — opinionated Spring with auto-config, embedded server, starter deps.

### Q19: What is dependency injection?
**Answer:** A pattern where dependencies are provided to a class rather than created inside it. Spring manages object creation.

```java
@Service
public class UserService {
  private final UserRepository repo;
  public UserService(UserRepository repo) { this.repo = repo; }
}
```

### Q20: What are the bean scopes?
**Answer:**
- `singleton` (default) — one instance per container.
- `prototype` — new instance every time.
- `request` — one per HTTP request.
- `session` — one per HTTP session.

### Q21: What is the difference between `@Component`, `@Service`, `@Repository`, and `@Controller`?
**Answer:** All are stereotypes that register a bean. They differ in intent:
- `@Component` — generic.
- `@Service` — business logic.
- `@Repository` — data access (adds exception translation).
- `@Controller` — web layer (Spring MVC).

### Q22: How do you build a REST API in Spring Boot?
**Answer:**

```java
@RestController
@RequestMapping("/api/users")
public class UserController {
  @GetMapping
  public List<User> getAll() { return userService.findAll(); }

  @GetMapping("/{id}")
  public User getById(@PathVariable Long id) { return userService.findById(id); }

  @PostMapping
  public User create(@RequestBody User user) { return userService.save(user); }

  @DeleteMapping("/{id}")
  public void delete(@PathVariable Long id) { userService.delete(id); }
}
```

### Q23: What is `@SpringBootApplication`?
**Answer:** A meta-annotation combining:
- `@Configuration` — defines beans.
- `@EnableAutoConfiguration` — turns on auto-config.
- `@ComponentScan` — scans for components in the package.

### Q24: How do you connect to a database?
**Answer:** Add `spring-boot-starter-data-jpa` and configure `application.properties`.

```properties
spring.datasource.url=jdbc:postgresql://localhost:5432/mydb
spring.datasource.username=postgres
spring.datasource.password=secret
```

### Q25: What is Spring Data JPA?
**Answer:** A Spring abstraction over JPA (Java Persistence API) that auto-generates repository implementations.

```java
public interface UserRepository extends JpaRepository<User, Long> {
  List<User> findByName(String name); // auto-generated query
}
```

### Q26: What is the difference between `JpaRepository`, `CrudRepository`, and `PagingAndSortingRepository`?
**Answer:**
- `CrudRepository` — basic CRUD.
- `PagingAndSortingRepository` — adds pagination/sorting.
- `JpaRepository` — adds JPA-specific features (flush, batch, etc.).

### Q27: How do you handle transactions?
**Answer:** Use `@Transactional`.

```java
@Transactional
public void transferMoney(Account from, Account to, BigDecimal amount) {
  from.withdraw(amount);
  to.deposit(amount);
}
```

### Q28: What is `@Transactional` propagation?
**Answer:** Defines how transactions relate to each other. Modes:
- `REQUIRED` (default) — join existing or create new.
- `REQUIRES_NEW` — always create new.
- `NESTED` — savepoint inside existing.

### Q29: What is AOP?
**Answer:** Aspect-Oriented Programming. Lets you add behavior (logging, security, transactions) without modifying business code. Uses **aspects**, **pointcuts**, and **advice**.

### Q30: How do you handle exceptions globally?
**Answer:** Use `@ControllerAdvice` and `@ExceptionHandler`.

```java
@ControllerAdvice
public class GlobalExceptionHandler {
  @ExceptionHandler(ResourceNotFoundException.class)
  public ResponseEntity<String> handleNotFound(ResourceNotFoundException e) {
    return ResponseEntity.status(404).body(e.getMessage());
  }
}
```

### Q31: What is `@RequestBody` vs `@RequestParam` vs `@PathVariable`?
**Answer:**
- `@RequestBody` — entire JSON body.
- `@RequestParam` — query string parameter (`?id=1`).
- `@PathVariable` — part of the URL path (`/users/{id}`).

### Q32: How do you secure a Spring Boot app?
**Answer:** Use **Spring Security**.

```java
@Configuration
@EnableWebSecurity
public class SecurityConfig {
  @Bean
  SecurityFilterChain filterChain(HttpSecurity http) throws Exception {
    http.authorizeHttpRequests(auth -> auth
      .requestMatchers("/public/**").permitAll()
      .anyRequest().authenticated()
    );
    return http.build();
  }
}
```

### Q33: What is JWT authentication in Spring?
**Answer:** Use `jjwt` or `nimbus-jose-jwt` to issue and verify tokens. Create a filter that validates tokens on each request.

### Q34: How do you cache in Spring?
**Answer:** Use `@Cacheable`.

```java
@Cacheable("users")
public User findById(Long id) { return repo.findById(id).orElseThrow(); }
```

Enable caching with `@EnableCaching`.

### Q35: What is Spring Boot Actuator?
**Answer:** Provides production-ready endpoints for health, metrics, info. Add `spring-boot-starter-actuator`.

```
GET /actuator/health
GET /actuator/metrics
```

### Q36: How do you configure logging?
**Answer:** Use `logback-spring.xml` or via `application.properties`.

```properties
logging.level.root=INFO
logging.level.com.myapp=DEBUG
```

### Q37: What is the difference between `@RestController` and `@Controller`?
**Answer:** `@RestController` = `@Controller` + `@ResponseBody`. The latter returns view names; the former returns data (JSON) directly.

### Q38: How do you handle pagination?
**Answer:** Spring Data provides `Pageable`.

```java
@GetMapping
public Page<User> list(Pageable pageable) {
  return repo.findAll(pageable);
}
```

### Q39: How do you deploy a Spring Boot app?
**Answer:** Build a fat JAR and run it.

```bash
mvn clean package
java -jar target/myapp.jar
```

### Q40: What is `@Profile`?
**Answer:** Marks a bean to be created only when a specific profile is active (e.g., `dev`, `prod`).

---

## Part 4 — Python + Django / FastAPI

### 4.1 Core Python

### Q1: What is Python? What are its key features?
**Answer:** A high-level, interpreted, dynamically typed, multi-paradigm language. Features: easy syntax, batteries included, huge ecosystem, used for web, data, ML, scripting.

### Q2: What are lists, tuples, sets, and dictionaries?
**Answer:**
- **List** — ordered, mutable, allows duplicates. `[1, 2, 3]`
- **Tuple** — ordered, **immutable**. `(1, 2, 3)`
- **Set** — unordered, no duplicates. `{1, 2, 3}`
- **Dict** — key-value pairs. `{"a": 1}`

### Q3: What is a list comprehension?
**Answer:** A short way to create a list.

```python
squares = [x * x for x in range(10)]
even = [x for x in range(10) if x % 2 == 0]
```

### Q4: What is the difference between `==` and `is`?
**Answer:**
- `==` checks **value equality**.
- `is` checks **identity** (same object in memory).

### Q5: What are decorators?
**Answer:** Functions that wrap another function to add behavior.

```python
def logger(fn):
  def wrapper(*args, **kwargs):
    print(f"Calling {fn.__name__}")
    return fn(*args, **kwargs)
  return wrapper

@logger
def greet(name): print(f"Hello {name}")
```

### Q6: What is the GIL?
**Answer:** Global Interpreter Lock. Only one thread executes Python bytecode at a time. This makes single-threaded programs fast but limits multi-threaded CPU work. Workarounds: `multiprocessing`, `asyncio`, native extensions.

### Q7: What is the difference between `async/await` and threading?
**Answer:**
- **Threading** — multiple OS threads, limited by GIL for CPU work. Good for I/O.
- **asyncio** — single-threaded cooperative multitasking. Best for high-concurrency I/O.

```python
async def fetch(url):
  async with aiohttp.ClientSession() as s:
    return await s.get(url)
```

### Q8: What are `*args` and `**kwargs`?
**Answer:**
- `*args` — variable positional arguments (tuple).
- `**kwargs` — variable keyword arguments (dict).

```python
def fn(*args, **kwargs):
  print(args, kwargs)
```

### Q9: What is a generator?
**Answer:** A function that yields values one at a time, lazily.

```python
def counter(n):
  for i in range(n): yield i
```

### Q10: What is the difference between `deepcopy` and `copy`?
**Answer:** `copy` copies the object but shares nested objects. `deepcopy` copies everything recursively.

```python
import copy
b = copy.copy(a)      # shallow
c = copy.deepcopy(a)  # deep
```

### 4.2 Django

### Q11: What is Django?
**Answer:** A high-level Python web framework that encourages rapid development. Includes ORM, admin, auth, templating, and follows the **MVT** (Model-View-Template) pattern.

### Q12: What is Django ORM?
**Answer:** An abstraction over SQL. You define models in Python; Django generates tables and queries.

```python
class User(models.Model):
  name = models.CharField(max_length=100)
  email = models.EmailField()

User.objects.filter(name="Alice")
User.objects.get(id=1)
User.objects.create(name="Bob", email="bob@example.com")
```

### Q13: What is the Django request/response cycle?
**Answer:**
1. URL dispatcher maps URL to a view.
2. Middleware process the request.
3. View runs business logic.
4. Template renders HTML (or JSON via DRF).
5. Response goes back through middleware.

### Q14: What are Django migrations?
**Answer:** Files that describe changes to the database schema. Generated with `makemigrations`, applied with `migrate`.

```bash
python manage.py makemigrations
python manage.py migrate
```

### Q15: How do you create a REST API in Django?
**Answer:** Use **Django REST Framework (DRF)**.

```python
from rest_framework import serializers, viewsets

class UserSerializer(serializers.ModelSerializer):
  class Meta:
    model = User
    fields = ["id", "name", "email"]

class UserViewSet(viewsets.ModelViewSet):
  queryset = User.objects.all()
  serializer_class = UserSerializer
```

### Q16: How do you authenticate in DRF?
**Answer:** Use built-in auth: Token, JWT, or SessionAuthentication.

```python
REST_FRAMEWORK = {
  "DEFAULT_AUTHENTICATION_CLASSES": [
    "rest_framework.authentication.TokenAuthentication",
  ]
}
```

### Q17: What is a Django middleware?
**Answer:** A hook that runs before/after each request. Used for auth, logging, CORS, etc.

### Q18: What is the `n+1` problem in Django?
**Answer:** Same as in SQL. Fix with `select_related` (JOIN) or `prefetch_related` (separate query).

```python
User.objects.select_related("profile").all()  # JOIN
User.objects.prefetch_related("posts").all()  # separate query
```

### Q19: How do you run tests in Django?
**Answer:**

```bash
python manage.py test
```

### Q20: How do you deploy Django?
**Answer:** Common stack: Gunicorn + Nginx + PostgreSQL. Or use Docker. Add `gunicorn` to requirements and run `gunicorn myproject.wsgi`.

### 4.3 FastAPI

### Q21: What is FastAPI?
**Answer:** A modern, fast (high-performance) Python web framework based on **type hints**. Uses `Starlette` (async) and `Pydantic` (data validation). Generates automatic OpenAPI docs.

### Q22: Build a basic FastAPI app.
**Answer:**

```python
from fastapi import FastAPI
app = FastAPI()

@app.get("/")
def root():
  return {"message": "Hello"}

@app.get("/users/{id}")
def get_user(id: int):
  return {"id": id}
```

### Q23: What is Pydantic?
**Answer:** A data validation library using Python type hints.

```python
from pydantic import BaseModel

class User(BaseModel):
  name: str
  email: str
  age: int = 0
```

### Q24: How do you do async in FastAPI?
**Answer:** Use `async def` and `await`.

```python
@app.get("/items")
async def get_items():
  items = await fetch_from_db()
  return items
```

### Q25: How do you handle dependencies (e.g., DB session)?
**Answer:** Use `Depends`.

```python
from fastapi import Depends
def get_db():
  db = SessionLocal()
  try: yield db
  finally: db.close()

@app.get("/users")
def list_users(db = Depends(get_db)):
  return db.query(User).all()
```

### Q26: How do you handle authentication in FastAPI?
**Answer:** Use OAuth2 + JWT with `OAuth2PasswordBearer`.

```python
from fastapi.security import OAuth2PasswordBearer
oauth2 = OAuth2PasswordBearer(tokenUrl="token")

@app.get("/me")
def read_me(token: str = Depends(oauth2)):
  return decode(token)
```

### Q27: How do you validate query parameters?
**Answer:** Use type hints with `Optional`, `Query`, etc.

```python
from typing import Optional
from fastapi import Query

@app.get("/items")
def list_items(skip: int = 0, limit: int = Query(10, le=100)):
  return {"skip": skip, "limit": limit}
```

### Q28: How do you test FastAPI?
**Answer:** Use `TestClient` from `fastapi.testclient`.

```python
from fastapi.testclient import TestClient
client = TestClient(app)
def test_root():
  r = client.get("/")
  assert r.status_code == 200
```

### Q29: What is the difference between Flask, Django, and FastAPI?
**Answer:**
| Feature | Flask | Django | FastAPI |
|---------|-------|--------|---------|
| Type | Micro | Full | Modern |
| Async | Limited | Limited | Native |
| Docs | Manual | Manual | Automatic |
| ORM | None | Built-in | None |
| Best for | Small APIs | Big apps | Async APIs |

### Q30: How do you deploy FastAPI?
**Answer:** Use **Uvicorn** (ASGI server).

```bash
uvicorn main:app --host 0.0.0.0 --port 8000
```

Or with Gunicorn workers:

```bash
gunicorn -k uvicorn.workers.UvicornWorker main:app
```

---

## Part 5 — Coding Challenges

These are common live-coding questions for backend interviews.

### Q1: Reverse a string.
```python
def reverse(s): return s[::-1]
```

### Q2: Check if a string is a palindrome.
```python
def is_pal(s):
  s = s.lower().replace(" ", "")
  return s == s[::-1]
```

### Q3: Find the most frequent element in an array.
```python
from collections import Counter
def most_freq(arr):
  return Counter(arr).most_common(1)[0][0]
```

### Q4: Fibonacci.
```python
def fib(n, memo={}):
  if n in memo: return memo[n]
  if n < 2: return n
  memo[n] = fib(n-1, memo) + fib(n-2, memo)
  return memo[n]
```

### Q5: Two Sum.
```python
def two_sum(nums, target):
  seen = {}
  for i, n in enumerate(nums):
    if target - n in seen: return [seen[target - n], i]
    seen[n] = i
```

### Q6: FizzBuzz.
```python
for i in range(1, 101):
  if i % 15 == 0: print("FizzBuzz")
  elif i % 3 == 0: print("Fizz")
  elif i % 5 == 0: print("Buzz")
  else: print(i)
```

### Q7: Flatten a nested list.
```python
def flatten(lst):
  out = []
  for x in lst:
    if isinstance(x, list): out.extend(flatten(x))
    else: out.append(x)
  return out
```

### Q8: Detect a cycle in a linked list (Floyd's).
```python
def has_cycle(head):
  slow = fast = head
  while fast and fast.next:
    slow = slow.next
    fast = fast.next.next
    if slow == fast: return True
  return False
```

### Q9: Implement a rate limiter (token bucket).
```python
import time
class RateLimiter:
  def __init__(self, capacity, refill_rate):
    self.cap = capacity
    self.rate = refill_rate
    self.tokens = capacity
    self.last = time.time()

  def allow(self):
    now = time.time()
    self.tokens = min(self.cap, self.tokens + (now - self.last) * self.rate)
    self.last = now
    if self.tokens >= 1:
      self.tokens -= 1
      return True
    return False
```

### Q10: URL shortener design.
- Use base62 encoding of an auto-incrementing ID.
- Store `short → long` mapping in a DB.
- Cache hot URLs in Redis.
- Use a counter (Snowflake or DB) for IDs.

### Q11: Implement a simple LRU cache.
```python
from collections import OrderedDict
class LRUCache:
  def __init__(self, capacity):
    self.cap = capacity
    self.cache = OrderedDict()

  def get(self, key):
    if key not in self.cache: return -1
    self.cache.move_to_end(key)
    return self.cache[key]

  def put(self, key, value):
    if key in self.cache: self.cache.move_to_end(key)
    self.cache[key] = value
    if len(self.cache) > self.cap:
      self.cache.popitem(last=False)
```

### Q12: Validate a JWT signature.
- Split token by `.`.
- Base64-decode header and payload.
- Verify signature using the secret.
- Check `exp` (expiration).

### Q13: Build a simple REST endpoint in Express.
```js
app.get("/api/users/:id", async (req, res) => {
  const user = await db.user.find(req.params.id);
  if (!user) return res.status(404).json({ error: "Not found" });
  res.json(user);
});
```

### Q14: Build a Spring Boot REST endpoint.
```java
@RestController
@RequestMapping("/api/users")
public class UserController {
  @GetMapping("/{id}")
  public ResponseEntity<User> get(@PathVariable Long id) {
    return userService.findById(id)
      .map(ResponseEntity::ok)
      .orElse(ResponseEntity.notFound().build());
  }
}
```

### Q15: Build a FastAPI endpoint with validation.
```python
from fastapi import FastAPI, HTTPException
from pydantic import BaseModel, EmailStr

app = FastAPI()

class User(BaseModel):
  name: str
  email: EmailStr

users = {}

@app.post("/users")
def create(user: User):
  users[user.email] = user
  return user

@app.get("/users/{email}")
def read(email: str):
  if email not in users: raise HTTPException(404)
  return users[email]
```

---

## Quick Reference Card

| Concept | Definition |
|---------|------------|
| REST | Architectural style for APIs |
| JWT | Stateless auth token |
| Idempotent | Same result on multiple calls |
| Idempotency Key | Unique ID for safe retries |
| CORS | Browser security for cross-origin |
| TLS | Encryption for HTTP |
| CDN | Edge cache for content |
| Sharding | Split DB across machines |
| CAP | Consistency / Availability / Partition |
| ACID | DB transaction guarantees |
| HATEOAS | Hypermedia in REST |
| OAuth 2.0 | Auth delegation framework |
| Webhook | Server-to-server callback |
| Idempotent producer | Kafka producer that doesn't duplicate |
| Saga | Distributed transaction pattern |

---

Good luck with your backend interviews!
