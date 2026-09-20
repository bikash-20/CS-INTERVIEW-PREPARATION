# Frontend Interview Preparation — Comprehensive Guide

A complete, plain-English guide to frontend developer interviews covering **HTML, CSS, JavaScript, TypeScript, React (18+), Next.js, state management, performance, accessibility, security, testing, build tools, browser internals, and modern frontend architecture**. Every section has theory first, then interview-style Q&A, then small runnable code snippets.

---

## Table of Contents

1. [HTML & CSS](#1-html--css)
2. [JavaScript — Core](#2-javascript--core)
3. [JavaScript — Async & Event Loop](#3-javascript--async--event-loop)
4. [JavaScript — Modern ES2020+ Features](#4-javascript--modern-es2020-features)
5. [TypeScript Essentials](#5-typescript-essentials)
6. [TypeScript Advanced](#6-typescript-advanced)
7. [React Fundamentals](#7-react-fundamentals)
8. [React Hooks (Deep Dive)](#8-react-hooks-deep-dive)
9. [React 18+ Modern Features](#9-react-18-modern-features)
10. [React Advanced Concepts](#10-react-advanced-concepts)
11. [React Patterns](#11-react-patterns)
12. [State Management](#12-state-management)
13. [Routing in React](#13-routing-in-react)
14. [Forms and Validation](#14-forms-and-validation)
15. [Server-Side Rendering (Next.js)](#15-server-side-rendering-nextjs)
16. [Styling in Modern Frontend](#16-styling-in-modern-frontend)
17. [Performance Optimization](#17-performance-optimization)
18. [Testing](#18-testing)
19. [Accessibility (a11y)](#19-accessibility-a11y)
20. [Build Tools & Bundlers](#20-build-tools--bundlers)
21. [Networking & APIs](#21-networking--apis)
22. [Browser Storage](#22-browser-storage)
23. [Security Basics](#23-security-basics)
24. [Web Performance Metrics (Web Vitals)](#24-web-performance-metrics-web-vitals)
25. [Browser Rendering Internals](#25-browser-rendering-internals)
26. [PWA, Service Workers, WebSockets](#26-pwa-service-workers-websockets)
27. [Coding Challenges (Frontend)](#27-coding-challenges-frontend)
28. [Behavioral & System Design for Frontend](#28-behavioral--system-design-for-frontend)
29. [Quick Reference Card](#29-quick-reference-card)

---

## 1. HTML & CSS

### Theory

HTML (HyperText Markup Language) describes the **structure** of a page; CSS describes the **presentation**. Modern frontend work relies on:
- Semantic HTML for accessibility and SEO.
- Flexbox and Grid for layout.
- Responsive design with media queries and container queries.
- CSS custom properties (variables) for theming.

### Q&A

**Q1: Block vs inline vs inline-block?**
- `block` — new line, full width (`<div>`).
- `inline` — flows with text, only content width (`<span>`).
- `inline-block` — inline but respects width/height/padding.

**Q2: Semantic HTML — what and why?**
Tags that describe meaning: `<header>`, `<nav>`, `<main>`, `<article>`, `<section>`, `<aside>`, `<footer>`, `<figure>`, `<time>`. Benefits: SEO, accessibility (screen readers), clearer code.

**Q3: CSS box model.**
Four layers from inside out: **content → padding → border → margin**.
With `box-sizing: border-box`, `width` includes padding + border.

```css
* { box-sizing: border-box; } /* recommended reset */
```

**Q4: `display: none` vs `visibility: hidden` vs `opacity: 0`?**
| | Takes space | Clickable | In layout |
|---|---|---|---|
| `display: none` | No | No | Removed |
| `visibility: hidden` | Yes | No | Reserved |
| `opacity: 0` | Yes | No | Transparent (still painted) |

**Q5: Flexbox — when and how?**
One-dimensional layout (row or column). Key properties on container: `flex-direction`, `justify-content`, `align-items`, `gap`. On items: `flex-grow`, `flex-shrink`, `flex-basis`.

```css
.row { display: flex; gap: 1rem; justify-content: space-between; align-items: center; }
.item { flex: 1 1 200px; } /* grow, shrink, basis */
```

**Q6: CSS Grid — when?**
Two-dimensional layout (rows AND columns).

```css
.grid { display: grid; grid-template-columns: 1fr 2fr 1fr; gap: 1rem; }
```

**Q7: CSS specificity order.**
Inline (1000) > ID (100) > class/attribute/pseudo-class (10) > element/pseudo-element (1). Same specificity → last wins. `!important` overrides everything (avoid).

**Q8: Position types.**
- `static` — default, normal flow.
- `relative` — shifted from normal position.
- `absolute` — relative to nearest positioned ancestor.
- `fixed` — relative to viewport.
- `sticky` — relative until scroll threshold, then fixed.

**Q9: Responsive design patterns.**
- Mobile-first.
- Media queries: `@media (min-width: 768px)`.
- Modern: container queries `@container (min-width: 400px)`.
- `clamp()` for fluid sizing: `font-size: clamp(1rem, 2vw, 2rem)`.

**Q10: CSS units.**
`px`, `em` (relative to parent font size), `rem` (relative to root), `%`, `vw`/`vh`, `fr` (grid).

**Q11: Pseudo-class vs pseudo-element?**
- **Pseudo-class** (`::hover`, `:focus`, `:nth-child(2)`) — state of an element.
- **Pseudo-element** (`::before`, `::after`, `::placeholder`) — a "virtual" sub-element.

**Q12: CSS variables and theming.**
```css
:root { --color-bg: white; --color-text: black; }
.dark { --color-bg: black; --color-text: white; }
body { background: var(--color-bg); color: var(--color-text); }
```

**Q13: Reflow vs repaint.**
- **Reflow (layout):** geometry changes — expensive. Avoid by batching DOM reads/writes.
- **Repaint:** visual style changes (color, background). Cheap.

**Q14: How does CSS load and block rendering?**
Stylesheets in `<head>` block first paint. Use `rel="preload"` for critical CSS, defer non-critical.

---

## 2. JavaScript — Core

### Theory

JavaScript is a **single-threaded**, **interpreted** (JIT compiled) language with:
- Dynamic typing.
- Prototype-based inheritance (pre-ES6 classes).
- First-class functions and closures.
- Event loop for async.

### Q&A

**Q1: `var` vs `let` vs `const`?**
- `var` — function-scoped, hoisted, redeclarable.
- `let` — block-scoped, hoisted but in TDZ, reassignable.
- `const` — block-scoped, hoisted but in TDZ, not reassignable (object contents can still change).

**Q2: Hoisting?**
Variable and function declarations are moved to the top of their scope. `var` initializes to `undefined`; `let`/`const` stay in TDZ (uninitialized).

**Q3: TDZ (Temporal Dead Zone)?**
The period between the start of a scope and the actual `let`/`const` declaration — accessing the variable throws a ReferenceError.

**Q4: `==` vs `===`?**
`==` coerces. `===` strict — same type AND value. Always prefer `===`.

```js
0 == "" // true (both coerce to 0)
0 === "" // false
```

**Q5: Closures?**
A function that "remembers" variables from its outer scope.

```js
function counter() {
 let n = 0;
 return () => ++n;
}
const c = counter();
c(); c(); c(); // 1, 2, 3
```

**Q6: Prototypes and inheritance?**
Every object has a hidden `[[Prototype]]`. Property lookup walks the chain.

```js
const animal = { eats: true };
const dog = Object.create(animal);
dog.barks = true;
dog.eats; // true (inherited)
```

ES6 `class` is **syntactic sugar** over prototypes.

**Q7: `this` keyword?**
- Inside a method → the object before the dot.
- Standalone → `undefined` in strict mode, global object in sloppy.
- Arrow function → inherits `this` from lexical scope.
- `Function.prototype.call/apply/bind` → set `this` explicitly.

**Q8: Call, apply, bind?**
```js
function greet(greeting) { return `${greeting}, ${this.name}`; }
const user = { name: "Alice" };
greet.call(user, "Hi");      // "Hi, Alice"
greet.apply(user, ["Hi"]);   // "Hi, Alice"
const fn = greet.bind(user); // returns new function
fn("Hello");                  // "Hello, Alice"
```

**Q9: `map`, `filter`, `reduce`?**
```js
[1, 2, 3].map(x => x * 2);            // [2, 4, 6]
[1, 2, 3].filter(x => x > 1);         // [2, 3]
[1, 2, 3].reduce((a, b) => a + b, 0); // 6
```

**Q10: Shallow vs deep copy?**
```js
const a = { x: 1, y: { z: 2 } };
const shallow = { ...a };        // y is shared
const deep = structuredClone(a); // full copy (modern)
```

`JSON.parse(JSON.stringify(a))` works but loses functions, dates, undefined, Map, Set.

**Q11: `null` vs `undefined`?**
- `undefined` — value not assigned yet.
- `null` — explicitly "no value".
Use `null` to mean "intentionally empty".

**Q12: `Array.from` and spread?**
```js
Array.from({ length: 5 }, (_, i) => i); // [0,1,2,3,4]
const [a, ...rest] = [1, 2, 3];         // a=1, rest=[2,3]
```

---

## 3. JavaScript — Async & Event Loop

### Theory

JavaScript runs on **one thread** for the call stack. Async work is delegated to browser APIs (timers, fetch, DOM events), which use **callback queues** and a **microtask queue** (for promises) to feed back into the stack via the **event loop**.

### Q&A

**Q1: How does the event loop work?**
1. Execute code in the call stack.
2. When the stack is empty, drain the **microtask queue** (Promise callbacks, queueMicrotask, MutationObserver).
3. Then take one task from the **task queue** (macrotask — setTimeout, setInterval, I/O, UI events).
4. Repeat.

Microtasks always run before the next macrotask.

**Q2: Promise states?**
- `pending` → `fulfilled` (with value) or `rejected` (with reason).
- Once settled, state is **immutable**.

**Q3: `Promise.all` vs `Promise.allSettled` vs `Promise.race` vs `Promise.any`?**
```js
Promise.all([p1, p2])        // resolves when ALL resolve; rejects if any rejects
Promise.allSettled([p1, p2]) // resolves with status array; never rejects
Promise.race([p1, p2])       // resolves/rejects with the first to settle
Promise.any([p1, p2])        // resolves with first to FULFILL; rejects if all reject (AggregateError)
```

**Q4: `async`/`await`?**
```js
async function fetchUsers() {
 try {
 const res = await fetch("/api/users");
 if (!res.ok) throw new Error(res.statusText);
 return await res.json();
 } catch (e) {
 console.error(e);
 }
}
```

**Q5: Common mistakes with await?**
```js
// BAD: serial
const a = await fetchA();
const b = await fetchB();

// GOOD: parallel
const [a, b] = await Promise.all([fetchA(), fetchB()]);
```

**Q6: Callback hell and how to escape?**
Nested callbacks → unmaintainable. Use Promises or `async/await`.

**Q7: `setTimeout` is not exact — why?**
It waits at least the given ms, but only when the call stack is free. Heavy work delays it. For animations, use `requestAnimationFrame`.

**Q8: What are microtasks vs macrotasks?**
- Microtasks (Promises, queueMicrotask, MutationObserver) — drained fully before the next macrotask.
- Macrotasks (setTimeout, I/O, UI events) — one at a time.

**Q9: How to cancel a fetch?**
Use `AbortController`.

```js
const ctrl = new AbortController();
fetch("/api", { signal: ctrl.signal });
ctrl.abort();
```

**Q10: `fetch` vs `XMLHttpRequest`?**
`fetch` is the modern Promise-based API. It does **not** reject on HTTP error statuses — only on network failures. Check `res.ok`.

---

## 4. JavaScript — Modern ES2020+ Features

### Q&A

**Q1: Optional chaining `?.` and nullish coalescing `??`?**
```js
const name = user?.profile?.name ?? "Anonymous";
```
`?.` short-circuits on null/undefined. `??` falls back only on null/undefined (not on 0 or "").

**Q2: Logical assignment operators?**
```js
a ||= b;   // a = a || b
a &&= b;   // a = a && b
a ??= b;   // a = a ?? b
```

**Q3: `Promise.allSettled`?**
Always resolves with a list of `{status, value|reason}` for each promise.

**Q4: Dynamic import?**
```js
const module = await import(`./locales/${locale}.js`);
```

**Q5: BigInt?**
```js
const huge = 9007199254740993n;
huge + 1n; // works; can't mix with regular Number
```

**Q6: `globalThis`?**
Universal way to access the global object — works in browser, Node, workers.

**Q7: Top-level `await` (ES modules)?**
```js
const data = await fetch("/api"); // allowed in modules
```

**Q8: `Array.prototype.flat` / `flatMap`?**
```js
[1, [2, 3]].flat();          // [1, 2, 3]
[1, 2].flatMap(x => [x, x*2]);// [1, 2, 2, 4]
```

**Q9: `Object.fromEntries`?**
```js
const entries = [["a", 1], ["b", 2]];
const obj = Object.fromEntries(entries); // { a: 1, b: 2 }
```

**Q10: WeakRef and FinalizationRegistry?**
References that don't prevent GC. Use sparingly — for caches and similar. Not for general use.

---

## 5. TypeScript Essentials

### Theory

TypeScript is JavaScript with **static types**. It compiles to JS (no runtime overhead). Types disappear at runtime.

### Q&A

**Q1: Why TypeScript?**
- Catch bugs at compile time.
- Better IDE autocomplete.
- Self-documenting.
- Easier refactoring.

**Q2: Basic types?**
```ts
let name: string = "Alice";
let age: number = 30;
let active: boolean = true;
let items: string[] = [];
let tuple: [string, number] = ["hi", 1];
let anything: any = 4;        // avoid
let unknown: unknown = 4;     // safer alternative
```

**Q3: `any` vs `unknown`?**
- `any` — disables type checking.
- `unknown` — must be narrowed before use (type guard).

**Q4: `interface` vs `type`?**
Most things interchangeable. Interfaces are open (declaration merging) and preferred for objects; types are better for unions, intersections.

```ts
interface User { id: string; name: string; }
type ID = string | number;
```

**Q5: Generics?**
```ts
function first<T>(arr: T[]): T | undefined {
 return arr[0];
}
first<number>([1, 2, 3]);
```

**Q6: Union and intersection types?**
```ts
type A = { a: string };
type B = { b: number };
type AB = A & B;              // intersection
type StringOrNumber = string | number;  // union
```

**Q7: Type narrowing?**
```ts
function print(x: string | number) {
 if (typeof x === "string") x.toUpperCase(); // narrowed to string
 if ("length" in x) ...                      // for objects
}
```

**Q8: Discriminated unions?**
```ts
type Result =
 | { ok: true; value: string }
 | { ok: false; error: Error };

if (result.ok) result.value; else result.error;
```

**Q9: `as const`?**
```ts
const colors = ["red", "green", "blue"] as const;
// readonly ["red", "green", "blue"]
```

**Q10: Non-null assertion `!`?**
Tells TS you know it's not null. Use sparingly.

```ts
const el = document.getElementById("app")!;
```

**Q11: `tsconfig.json` essentials?**
```json
{
 "compilerOptions": {
 "target": "ES2022",
 "strict": true,
 "noImplicitAny": true,
 "strictNullChecks": true,
 "jsx": "react-jsx",
 "moduleResolution": "bundler",
 "esModuleInterop": true,
 "skipLibCheck": true
 }
}
```

**Q12: Utility types?**
```ts
Partial<T>    // all properties optional
Required<T>   // all properties required
Pick<T, K>    // subset of properties
Omit<T, K>    // all except K
Record<K, V>  // { [K]: V }
Readonly<T>   // all readonly
ReturnType<F> // return type of function
```

---

## 6. TypeScript Advanced

### Q&A

**Q1: Conditional types?**
```ts
type IsString<T> = T extends string ? true : false;
type X = IsString<"hi">; // true
```

**Q2: Mapped types?**
```ts
type Nullable<T> = { [K in keyof T]: T[K] | null };
```

**Q3: Template literal types?**
```ts
type EventName = `on${Capitalize<string>}`;
const e: EventName = "onClick";
```

**Q4: `satisfies` operator (TS 4.9)?**
Validates that a value matches a type **without widening** it.
```ts
const palette = {
 red: [255, 0, 0],
 green: "#00ff00",
} satisfies Record<string, string | number[]>;
palette.red.length; // OK (still typed as number[])
```

**Q5: Declaration files (`.d.ts`)?**
Describe the types of a JS library so TS can use it.

**Q6: `infer` keyword?**
```ts
type ReturnPromise<T> = T extends Promise<infer U> ? U : T;
```

**Q7: Branded types?**
Make nominally different types even if structurally same.
```ts
type UserId = string & { __brand: "UserId" };
function makeUserId(s: string): UserId { return s as UserId; }
```

**Q8: `unknown` vs `never`?**
- `unknown` — top type; can hold anything, must be narrowed.
- `never` — bottom type; no value can exist.

**Q9: `readonly` arrays?**
```ts
const nums: readonly number[] = [1, 2, 3];
nums.push(4); // Error
```

---

## 7. React Fundamentals

### Q&A

**Q1: What is React?**
A JavaScript library for building user interfaces using a **declarative**, **component-based** model. React only handles the view; routing, state, etc. come from libraries.

**Q2: JSX?**
A syntax extension that lets you write HTML-like code inside JS. Compiles to `React.createElement(...)` calls.

```jsx
const el = <h1 className="title">Hello</h1>;
```

**Q3: Virtual DOM?**
An in-memory representation of the UI. React compares new and previous trees (**reconciliation**) and applies minimum DOM updates.

**Q4: Components — function vs class?**
Modern React is **function-first**. Hooks replaced most class features. Classes only for legacy or `Error Boundary`.

**Q5: Props?**
Read-only inputs from parent.

```jsx
function Greeting({ name }: { name: string }) {
 return <h1>Hi, {name}</h1>;
}
```

**Q6: State?**
Component-owned mutable data. Updates trigger re-renders.

```jsx
const [count, setCount] = useState(0);
```

**Q7: Conditional rendering?**
```jsx
{isLoggedIn ? <Dashboard /> : <Login />}
{items.length > 0 && <List items={items} />}
```

**Q8: Lists and keys?**
```jsx
{items.map(item => <Item key={item.id} {...item} />)}
```
Keys help React identify which items changed. Use stable IDs, **not** array indexes (for mutable lists).

**Q9: Controlled vs uncontrolled components?**
- **Controlled** — React holds the input value (`value` + `onChange`).
- **Uncontrolled** — DOM holds the value; read with `ref`.

**Q10: Lifting state up?**
If two siblings need to share state, move it to the nearest common ancestor.

**Q11: Composition vs inheritance?**
React prefers composition. Pass children or specific props instead of inheriting.

```jsx
function Card({ children }) { return <div className="card">{children}</div>; }
<Card><h1>Title</h1></Card>
```

**Q12: `React.Fragment`?**
Return multiple elements without a wrapper node.

```jsx
return <><h1 /><p /></>;
```

**Q13: `children` prop?**
Whatever JSX is between the opening and closing tags of a component.

**Q14: How does React decide to re-render?**
- Parent re-renders → child re-renders (unless memoized).
- State in the component changes → component re-renders.
- Context value changes → consumers re-render.

**Q15: Reconciliation?**
React's diffing algorithm — uses keys to match children between renders and minimizes DOM operations.

---

## 8. React Hooks (Deep Dive)

### Q&A

**Q1: Rules of Hooks.**
- Only call at top level (no loops, conditions, nested functions).
- Only call from React functions (components or custom hooks).

**Q2: `useState` deep dive.**
- Returns `[value, setter]`.
- Updates are **batched** and **async**.
- Functional updates use the latest state: `setX(x => x + 1)`.
- If new state === old state (Object.is), no re-render.

**Q3: `useEffect` deep dive.**
Runs after render. Optional cleanup.

```jsx
useEffect(() => {
 const id = setInterval(tick, 1000);
 return () => clearInterval(id);
}, []);
```

Dependency array:
- `[]` — mount/unmount only.
- `[a]` — when `a` changes.
- no array — every render.

**Q4: `useLayoutEffect`?**
Same as `useEffect` but fires **synchronously after DOM mutations**, before browser paints. Use for DOM measurements.

**Q5: `useContext`?**
Access context value without prop drilling.

```jsx
const ThemeContext = createContext("light");
function App() {
 return (
 <ThemeContext.Provider value="dark">
 <Header />
 </ThemeContext.Provider>
 );
}
function Header() {
 const theme = useContext(ThemeContext);
 return <div className={theme}>...</div>;
}
```

**Q6: `useReducer`?**
Complex state with multiple sub-values or next state depending on previous.

```jsx
function reducer(state, action) {
 switch (action.type) {
 case "inc": return { count: state.count + 1 };
 case "dec": return { count: state.count - 1 };
 default: return state;
 }
}
const [state, dispatch] = useReducer(reducer, { count: 0 });
dispatch({ type: "inc" });
```

**Q7: `useMemo` and `useCallback`?**
- `useMemo(fn, deps)` — memo a computed value.
- `useCallback(fn, deps)` — memo a function reference.

```jsx
const sorted = useMemo(() => items.sort(), [items]);
const handleClick = useCallback(() => doThing(id), [id]);
```

Use sparingly — over-use hurts perf too.

**Q8: `useRef`?**
Holds a mutable value across renders without triggering re-renders.

```jsx
const inputRef = useRef<HTMLInputElement>(null);
useEffect(() => { inputRef.current?.focus(); }, []);
```

**Q9: `useImperativeHandle`?**
Customize the ref value a parent gets from a child.

```jsx
useImperativeHandle(ref, () => ({ focus: () => inputRef.current?.focus() }));
```

**Q10: `forwardRef` (legacy, replaced by `ref` as prop in 19)?**
Pass refs to function components.

```jsx
const MyInput = forwardRef<HTMLInputElement, Props>((props, ref) => (
 <input ref={ref} {...props} />
));
```

**Q11: `useId`?**
Generate stable unique IDs for SSR-safe accessibility attributes.

**Q12: Custom hooks?**
Reuse stateful logic across components.

```jsx
function useLocalStorage<T>(key: string, initial: T) {
 const [v, setV] = useState<T>(() => JSON.parse(localStorage.getItem(key) ?? JSON.stringify(initial)));
 useEffect(() => localStorage.setItem(key, JSON.stringify(v)), [key, v]);
 return [v, setV] as const;
}
```

**Q13: Stale closures?**
A common bug — `useEffect` captures variables from the render it was created in. Use functional state updates or refs to avoid.

```jsx
// Buggy
useEffect(() => {
 const id = setInterval(() => setCount(count + 1), 1000);
 return () => clearInterval(id);
}, []); // count is always 0 here

// Fixed
useEffect(() => {
 const id = setInterval(() => setCount(c => c + 1), 1000);
 return () => clearInterval(id);
}, []);
```

---

## 9. React 18+ Modern Features

### Q&A

**Q1: Automatic batching.**
React 18 batches state updates inside Promises, setTimeout, and native event handlers automatically.

**Q2: `createRoot` API.**
```jsx
import { createRoot } from "react-dom/client";
createRoot(document.getElementById("root")!).render(<App />);
```

**Q3: Concurrent rendering?**
React can pause, abort, or resume a render to keep the UI responsive.

**Q4: `useTransition`.**
Mark state updates as non-urgent. The UI stays interactive.

```jsx
const [isPending, startTransition] = useTransition();
function handleChange(e) {
 setInput(e.target.value); // urgent
 startTransition(() => setQuery(e.target.value)); // can be deferred
}
```

**Q5: `useDeferredValue`.**
Defer a value update (like a debounce but tied to React's scheduler).

```jsx
const deferred = useDeferredValue(query);
const results = useMemo(() => search(deferred), [deferred]);
```

**Q6: Suspense for data fetching.**
Show a fallback while a child is loading.

```jsx
<Suspense fallback={<Spinner />}>
 <Profile />
</Suspense>
```

Combine with React Query, SWR, Relay, or Server Components.

**Q7: Server Components (RSC).**
Components that render on the server only — bundle size zero on the client.

```jsx
// ServerComponent.server.jsx
async function ServerComponent() {
 const data = await db.query("...");
 return <List data={data} />;
}
```

**Q8: Actions (form mutations in RSC).**
Server functions that mutate state from the client.

**Q9: New `use` hook.**
Read resources (Promises, context) inside render.

```jsx
function Message({ promise }) {
 const data = use(promise); // suspends until resolved
 return <p>{data.text}</p>;
}
```

**Q10: React 19 highlights.**
- `ref` is a regular prop (no `forwardRef`).
- Server Actions stable.
- New compiler that auto-memoizes.
- `<form>` actions built-in.
- `useOptimistic` for optimistic updates.

---

## 10. React Advanced Concepts

### Q&A

**Q1: Error Boundaries.**
Catch errors in render.

```jsx
class ErrorBoundary extends React.Component {
 state = { hasError: false };
 static getDerivedStateFromError() { return { hasError: true }; }
 componentDidCatch(error, info) { logToService(error, info); }
 render() { return this.state.hasError ? <Fallback /> : this.props.children; }
}
```

**Q2: Higher-Order Components (HOC) — legacy.**
Function that takes a component and returns a new component.
```jsx
const withAuth = (Comp) => (props) => props.user ? <Comp {...props} /> : <Login />;
```

**Q3: Render props — legacy.**
Pass a function as a prop to share logic.
```jsx
<Mouse render={({ x, y }) => <p>{x},{y}</p>} />
```

**Q4: Context performance pitfall.**
Every consumer re-renders when the context value changes. Split contexts or memo the value.

```jsx
const value = useMemo(() => ({ user, setUser }), [user]);
```

**Q5: Refs to DOM vs components?**
- DOM ref: `useRef<HTMLInputElement>(null)` → `inputRef.current` is the element.
- Class component ref: instance.
- Function component: use `forwardRef` (or `ref` as a regular prop in React 19).

**Q6: Portal?**
Render a child into a different DOM node.

```jsx
createPortal(<Modal />, document.body);
```

**Q7: Suspense and lazy loading?**
```jsx
const Settings = React.lazy(() => import("./Settings"));
<Suspense fallback={<Spinner />}><Settings /></Suspense>
```

**Q8: StrictMode?**
Dev-only tool that double-invokes some functions to surface side-effect bugs.

**Q9: Profiler API?**
Measure render performance.

```jsx
<Profiler id="List" onRender={(id, phase, actual) => log(id, phase, actual)}>
 <List />
</Profiler>
```

**Q10: Controlled inputs and why?**
React owns the value. Easier validation, consistent state, no DOM-read race conditions.

**Q11: How to handle forms?**
Use libraries like React Hook Form, Formik, or Final Form — they handle validation, dirty state, touched, async submit, and reduce re-renders.

---

## 11. React Patterns

### Q&A

**Q1: Compound components?**
Components that share implicit state.

```jsx
<Tabs>
 <Tabs.List>
 <Tabs.Trigger value="a">A</Tabs.Trigger>
 <Tabs.Trigger value="b">B</Tabs.Trigger>
 </Tabs.List>
 <Tabs.Panel value="a">Content A</Tabs.Panel>
</Tabs>
```

**Q2: Provider pattern?**
Wrap a tree in a context provider for shared services/config.

**Q3: Container / Presentational split?**
- **Container** — handles data fetching & state.
- **Presentational** — receives data via props; pure render.

Less common now; replaced by custom hooks + small components.

**Q4: Headless component pattern?**
Logic without UI; consumer provides UI. Libraries: React Table, Downshift, Radix.

**Q5: Custom hook + reducer for complex forms?**
Encapsulate form state, validation, and submit in a hook.

**Q6: Lifting state vs context?**
- Lift state if only 2-3 components need it.
- Use context for many components or deeply nested.

**Q7: Slot pattern?**
Pass JSX as named props instead of `children`.

```jsx
<Modal title={<h2>Hi</h2>} body={<p>Body</p>} footer={<Button>OK</Button>} />
```

**Q8: Polymorphic components?**
Components that accept an `as` prop to change the rendered element.

```jsx
<Text as="h1">Title</Text>
```

---

## 12. State Management

### Theory

State lives in many places:
- **Local** — `useState`/`useReducer` in a component.
- **Lifted** — shared with siblings via parent.
- **Context** — for deeply shared values.
- **External store** — Redux, Zustand, Jotai, Recoil, MobX.
- **Server cache** — React Query, SWR, RTK Query.

Pick the smallest tool that solves the problem.

### Q&A

**Q1: Local vs global state?**
Start local. Lift when needed. Reach for global only when many unrelated components care.

**Q2: Redux — what and why?**
A predictable state container. Three principles: single store, state is read-only, changes via pure reducers.

```js
const counter = (state = 0, action) => {
 switch (action.type) {
 case "INC": return state + 1;
 case "DEC": return state - 1;
 default: return state;
 }
};
```

**Q3: Redux Toolkit (RTK)?**
The modern, simpler Redux. Uses `createSlice`, immer for immutable updates, and includes RTK Query for data fetching.

**Q4: Zustand?**
Tiny store (~1 KB) with hooks. Simple API.

```js
const useStore = create((set) => ({
 count: 0,
 inc: () => set((s) => ({ count: s.count + 1 })),
}));
```

**Q5: Jotai / Recoil?**
**Atomic** state — each piece of state is a separate atom. Fine-grained subscriptions and re-renders.

**Q6: React Query / SWR?**
**Server-state** management — caching, revalidation, retries, pagination.

```jsx
const { data, isLoading } = useQuery({ queryKey: ["todos"], queryFn: fetchTodos });
```

**Q7: Server state vs client state?**
- **Server state** — fetched from API; cache, dedupe, invalidate.
- **Client state** — UI toggles, form drafts, theme.
Don't mix them in one global store.

**Q8: When to pick what?**
- Local UI: `useState`/`useReducer`.
- Theme/auth: context.
- Complex local: reducer.
- App-wide client state: Zustand or Redux Toolkit.
- Server cache: React Query / SWR.

---

## 13. Routing in React

### Q&A

**Q1: React Router basics.**
```jsx
<BrowserRouter>
 <Routes>
 <Route path="/" element={<Home />} />
 <Route path="/users/:id" element={<User />} />
 <Route path="*" element={<NotFound />} />
 </Routes>
</BrowserRouter>
```

**Q2: `Link` vs `<a>`?**
`<Link>` does client-side navigation without full page reload. `<a>` triggers a full request.

**Q3: Nested routes.**
```jsx
<Route path="/dashboard" element={<Dashboard />}>
 <Route path="stats" element={<Stats />} />
 <Route path="settings" element={<Settings />} />
</Route>
```

Use `<Outlet />` inside `Dashboard` to render the child.

**Q4: Protected routes.**
```jsx
<Route element={<RequireAuth />}>
 <Route path="/profile" element={<Profile />} />
</Route>
```
`RequireAuth` checks auth and renders `<Outlet />` or redirects.

**Q5: Programmatic navigation.**
```jsx
const navigate = useNavigate();
navigate("/home");
navigate(-1); // back
```

**Q6: Search params.**
```jsx
const [params] = useSearchParams();
const id = params.get("id");
```

**Q7: Lazy loading routes.**
```jsx
const Profile = lazy(() => import("./Profile"));
<Route path="/profile" element={
 <Suspense fallback={<Spinner />}>
 <Profile />
 </Suspense>
 } />
```

**Q8: History modes.**
- `BrowserRouter` — clean URLs (`/about`), needs server config.
- `HashRouter` — uses `#/about`, no server config.
- `MemoryRouter` — in-memory, used in tests.

---

## 14. Forms and Validation

### Q&A

**Q1: Controlled vs uncontrolled — when?**
- **Controlled** — React owns the value. Best for validation, conditional fields, dynamic forms.
- **Uncontrolled** — DOM owns it. Best for simple forms or file inputs.

**Q2: Form libraries — which?**
- **React Hook Form** — minimal re-renders, easy validation.
- **Formik** — older, popular.
- **Final Form** — subscription model.

**Q3: React Hook Form example.**
```jsx
const { register, handleSubmit, formState: { errors } } = useForm();
const onSubmit = (data) => console.log(data);
return (
 <form onSubmit={handleSubmit(onSubmit)}>
 <input {...register("email", { required: "Required", pattern: /^\S+@\S+$/ })} />
 {errors.email && <p>{errors.email.message}</p>}
 <button>Submit</button>
 </form>
);
```

**Q4: Validation — client and server?**
Always **both**. Client gives UX; server is the source of truth.

**Q5: Async validation?**
Use `react-hook-form`'s `validate` returning a Promise, or libraries like `yup`, `zod`, `joi` with async refinements.

**Q6: Handling file uploads?**
- Use `FormData` + `fetch`.
- Show progress with `XMLHttpRequest` (which has `upload.onprogress`).
- For large files: chunked uploads.

**Q7: Optimistic updates?**
Apply the change locally before the server confirms; rollback on error.

```jsx
const mutation = useMutation({
 mutationFn: updateTodo,
 onMutate: async (newTodo) => {
 await qc.cancelQueries(["todos"]);
 const prev = qc.getQueryData(["todos"]);
 qc.setQueryData(["todos"], old => [...old, newTodo]);
 return { prev };
 },
 onError: (_e, _v, ctx) => qc.setQueryData(["todos"], ctx.prev),
 onSettled: () => qc.invalidateQueries(["todos"]),
});
```

---

## 15. Server-Side Rendering (Next.js)

### Q&A

**Q1: CSR vs SSR vs SSG vs ISR?**
- **CSR** — render in browser.
- **SSR** — render on each request.
- **SSG** — render at build time.
- **ISR** — like SSG but revalidate after a period.

**Q2: Why SSR?**
- Better SEO for crawlers that don't run JS.
- Faster first paint.
- Better social previews.

**Q3: Next.js App Router (13+).**
File-based routing with `app/` folder. Each route is a folder with `page.tsx`. Layouts with `layout.tsx`. Server Components by default; mark a file `"use client"` for client components.

**Q4: Server vs Client Components?**
- **Server** — run only on server; can `await` data, talk to DB, no bundle.
- **Client** — interactive, hooks, browser APIs.

```jsx
// Server Component (default)
async function Page() {
 const data = await db.posts.find();
 return <List posts={data} />;
}
```

**Q5: Data fetching in Next.js?**
- Server Component — `await` directly.
- Route Handler (`route.ts`) — REST API.
- Server Action — form mutation.
- Client — React Query.

**Q6: `loading.tsx`, `error.tsx`, `not-found.tsx`?**
Special files for Suspense fallback, error boundary, and 404.

**Q7: Static Generation with `generateStaticParams`?**
Pre-render dynamic routes at build time.

**Q8: Incremental Static Regeneration?**
```jsx
export const revalidate = 60; // seconds
```

**Q9: Hydration mismatch?**
SSR HTML doesn't match client first render. Causes React to throw. Fix by avoiding browser-only APIs in server components.

**Q10: When NOT to use SSR?**
Highly interactive dashboards behind login — CSR is fine.

---

## 16. Styling in Modern Frontend

### Q&A

**Q1: Styling approaches?**
- **Plain CSS / Sass** — global, simple.
- **CSS Modules** — scoped, file-local class names.
- **Tailwind** — utility classes.
- **CSS-in-JS** (Styled Components, Emotion) — JS, dynamic.
- **CSS variables** for theming.

**Q2: Tailwind CSS — pros and cons?**
**Pros:** fast to write, consistent design tokens, purged in prod.
**Cons:** HTML can be noisy; learning curve; designers need it too.

```jsx
<button className="bg-blue-500 hover:bg-blue-700 text-white px-4 py-2 rounded">
 Click me
</button>
```

**Q3: CSS Modules example?**
```css
/* Button.module.css */
.button { background: var(--bg); }
```

```jsx
import s from "./Button.module.css";
<button className={s.button}>...</button>;
```

**Q4: Styled Components (CSS-in-JS)?**
```jsx
const Button = styled.button`
 background: ${p => p.primary ? "blue" : "white"};
`;
```

**Q5: Theming with CSS variables?**
Define `:root { --color-primary: ... }`. Change on `.dark` or `[data-theme="dark"]`.

**Q6: Responsive design with Tailwind?**
`sm:`, `md:`, `lg:`, `xl:` prefixes: `text-sm md:text-lg`.

**Q7: Design tokens?**
Centralized variables for colors, spacing, typography — used across components, themes, and platforms.

**Q8: When to pick what?**
- Tailwind — fast iteration, solo or small team.
- CSS Modules — small to medium apps with strong type-checking.
- CSS-in-JS — heavy dynamic theming (less common now).
- Component libraries (MUI, Chakra, Mantine) — quick UI prototypes.

---

## 17. Performance Optimization

### Q&A

**Q1: React.memo?**
Memoize a component — only re-renders if props change (shallow compare).

```jsx
const Row = React.memo(function Row({ item }) { ... });
```

**Q2: When does memo help?**
- Component is expensive.
- Renders often with same props.
- Don't memo every component — overhead.

**Q3: `useMemo`/`useCallback` — when?**
Only when:
- The computed value is expensive.
- The function is passed to a memoized child.
- The function is a dependency of an effect.

**Q4: Virtualize long lists.**
Render only visible rows. Libraries: `react-window`, `react-virtual`.

```jsx
import { FixedSizeList as List } from "react-window";
```

**Q5: Code splitting.**
```jsx
const Heavy = lazy(() => import("./Heavy"));
<Suspense fallback={<Spinner />}><Heavy /></Suspense>
```

**Q6: Image optimization.**
- `<img loading="lazy">`.
- `<img decoding="async">`.
- Use modern formats (WebP, AVIF).
- Specify width/height to avoid layout shift.
- Use `srcset` for responsive.

**Q7: Reduce bundle size.**
- Tree-shake.
- Dynamic imports for routes.
- Replace big libs with smaller alternatives (`date-fns` over `moment`).
- `webpack-bundle-analyzer`.

**Q8: Avoid reconciliation work.**
- Stable, unique `key` on lists.
- Memo expensive subtrees.
- Don't mutate state — make new objects/arrays.

**Q9: Debounce / throttle hot events.**
```jsx
const onChange = useMemo(
 () => debounce(setQuery, 300),
 []
);
```

**Q10: Profile before optimizing.**
Use React DevTools Profiler and `performance.mark()` to find real bottlenecks.

**Q11: Avoid prop drilling large objects.**
Use context for cross-cutting data; keep components narrow.

---

## 18. Testing

### Q&A

**Q1: Testing pyramid.**
- **Unit tests** — many. Individual functions/components.
- **Integration tests** — fewer. Components together.
- **E2E tests** — few. Full flows in real browser.

**Q2: Tools?**
- **Unit/integration:** Jest + React Testing Library.
- **E2E:** Cypress, Playwright.
- **API mocking:** MSW (Mock Service Worker).

**Q3: React Testing Library philosophy.**
Test the **user**, not implementation details. Query by role, label, text — not by class or test id.

**Q4: Example test.**
```jsx
import { render, screen } from "@testing-library/react";
import userEvent from "@testing-library/user-event";

test("increments count on click", async () => {
 render(<Counter />);
 await userEvent.click(screen.getByRole("button", { name: /increment/i }));
 expect(screen.getByText("Count: 1")).toBeInTheDocument();
});
```

**Q5: Mocking modules.**
```jsx
jest.mock("./api", () => ({
 fetchUser: () => Promise.resolve({ name: "Alice" }),
}));
```

**Q6: Testing async.**
```jsx
test("loads user", async () => {
 render(<User id="1" />);
 expect(await screen.findByText("Alice")).toBeInTheDocument();
});
```

**Q7: E2E with Playwright?**
```js
test("login flow", async ({ page }) => {
 await page.goto("/login");
 await page.getByLabel("Email").fill("a@b.com");
 await page.getByLabel("Password").fill("pw");
 await page.getByRole("button", { name: "Sign in" }).click();
 await expect(page).toHaveURL("/dashboard");
});
```

**Q8: Snapshot testing?**
Compares rendered output to a saved file. Brittle for large outputs; use sparingly or for stable components.

**Q9: MSW for API mocking.**
```js
import { http, HttpResponse } from "msw";
import { setupServer } from "msw/node";

const server = setupServer(
 http.get("/api/user", () => HttpResponse.json({ name: "Alice" })),
);
beforeAll(() => server.listen());
afterEach(() => server.resetHandlers());
afterAll(() => server.close());
```

**Q10: Test isolation.**
Reset mocks between tests; don't share state across tests.

---

## 19. Accessibility (a11y)

### Q&A

**Q1: Why accessibility?**
- Legal requirement in many countries.
- Better for all users (e.g., captions help in noisy places).
- ~15% of users have a disability.

**Q2: Semantic HTML first.**
Use `<button>` not `<div onClick>`, `<a>` not `<span>`, `<nav>`, `<main>`, `<header>`, etc.

**Q3: ARIA roles.**
When no native element exists, use roles:
- `role="alert"`, `role="dialog"`, `role="tablist"`, `role="menu"`, etc.

**Q4: Keyboard navigation.**
Tab order must make sense. Visible focus styles. Custom widgets must implement arrow-key navigation.

**Q5: Forms.**
- Every input needs a `<label>`.
- Error messages linked with `aria-describedby`.
- `aria-invalid="true"` when invalid.

```jsx
<label htmlFor="email">Email</label>
<input id="email" aria-invalid={!!errors.email} aria-describedby="email-err" />
{errors.email && <p id="email-err">{errors.email.message}</p>}
```

**Q6: Focus management.**
On modal open, focus inside; on close, return focus to trigger.

**Q7: Color contrast.**
WCAG AA: 4.5:1 for body text, 3:1 for large text. Use tools like axe DevTools.

**Q8: Screen reader testing.**
VoiceOver (Mac), NVDA (Windows). Test important flows.

**Q9: Live regions?**
`aria-live="polite"` or `"assertive"` for dynamic content.

**Q10: Skip links?**
"Skip to main content" link for keyboard users.

**Q11: Accessible names for icons.**
```jsx
<button aria-label="Close"><X /></button>
```

**Q12: Reducing motion?**
```css
@media (prefers-reduced-motion: reduce) {
 * { animation: none !important; transition: none !important; }
}
```

**Q13: Lighthouse / axe / pa11y.**
Automated scanners catch ~30-50% of issues. Manual testing matters too.

---

## 20. Build Tools & Bundlers

### Q&A

**Q1: What does a bundler do?**
Resolves the dependency graph, transforms (TS, JSX), and bundles modules for the browser.

**Q2: Webpack vs Vite vs esbuild vs Turbopack?**
- **Webpack** — most popular, slowest, most plugins.
- **Vite** — dev server with native ESM; Rollup for prod build. Fast.
- **esbuild** — Go-based, super fast transformer.
- **Turbopack** — Rust-based, Next.js's new bundler.

**Q3: Tree shaking.**
Remove unused exports. Works when the code uses ES modules and is side-effect-free.

**Q4: Code splitting strategies.**
- **Entry points** — manual.
- **Dynamic import** — automatic per route or component.
- **Webpack splitChunks** — by size, vendor, async.

**Q5: Source maps.**
Map production bundle back to source for debugging. Set `devtool: "source-map"` (prod) or `"eval"` (dev).

**Q6: What is HMR?**
Hot Module Replacement — replace modules in the running app without a full reload.

**Q7: Babel — what and why?**
JS transpiler. Lets you use newer syntax in older browsers. With Vite/esbuild it's optional.

**Q8: Polyfills?**
Code that adds missing features to older browsers (e.g., `Array.prototype.flat` for IE).

**Q9: Package managers — npm vs yarn vs pnpm vs bun?**
- **npm** — standard.
- **yarn** — workspaces, plug'n'play.
- **pnpm** — fast, disk-efficient via content-addressable store.
- **bun** — fast, all-in-one runtime + package manager.

**Q10: Monorepo tools?**
Turborepo, Nx, Lerna, pnpm workspaces.

---

## 21. Networking & APIs

### Q&A

**Q1: REST, GraphQL, gRPC?**
- **REST** — resource-oriented HTTP, JSON.
- **GraphQL** — single endpoint, client specifies shape.
- **gRPC** — strongly typed, HTTP/2 + Protobuf, great for service-to-service.

**Q2: HTTP methods.**
`GET` (read), `POST` (create), `PUT` (replace), `PATCH` (partial), `DELETE` (remove), `OPTIONS` (CORS preflight).

**Q3: Status codes.**
- `1xx` info.
- `2xx` success (`200`, `201`, `204`).
- `3xx` redirect (`301`, `302`, `304`).
- `4xx` client error (`400`, `401`, `403`, `404`, `409`, `429`).
- `5xx` server error (`500`, `502`, `503`).

**Q4: CORS?**
Cross-Origin Resource Sharing. Browser blocks JS from reading responses from a different origin unless the server sends `Access-Control-Allow-*` headers.

**Q5: Preflight?**
For non-simple requests, browser sends an `OPTIONS` first to ask the server if the request is OK.

**Q6: Authentication vs Authorization?**
- **Authn** — who are you? (login, JWT, OAuth).
- **Authz** — what can you do? (roles, scopes).

**Q7: Tokens in cookies vs Authorization header?**
- Cookies — auto-sent; same-site, httpOnly, secure, CSRF-protected.
- Authorization header — explicit, common in SPAs.

**Q8: JWT?**
JSON Web Token — header.payload.signature. Stateless, but cannot be revoked without extra infrastructure.

**Q9: Fetch with retry?**
```js
async function fetchWithRetry(url, n = 3) {
 for (let i = 0; i < n; i++) {
 try { return await fetch(url); } catch (e) { if (i === n-1) throw e; }
 await new Promise(r => setTimeout(r, 2 ** i * 100));
 }
}
```

**Q10: WebSockets?**
Full-duplex persistent TCP connection. Use `WebSocket` API; or libraries like Socket.IO, native.

**Q11: SSE (Server-Sent Events)?**
One-way push from server. `new EventSource("/stream")`.

**Q12: Idempotency?**
Same request returns same result. Use `Idempotency-Key` header on POSTs that charge cards.

**Q13: Pagination — offset vs cursor?**
- Offset — `?page=2&size=20`. Simple, slow deep.
- Cursor — `?cursor=...&size=20`. Stable under inserts, fast.

---

## 22. Browser Storage

### Q&A

**Q1: Storage options.**
- **localStorage** — ~5 MB, synchronous, persists.
- **sessionStorage** — like local but per tab.
- **Cookies** — ~4 KB, sent to server, httpOnly for security.
- **IndexedDB** — large, async, structured.
- **Cache API** — for request/response pairs (PWA offline).

**Q2: localStorage vs cookies?**
- localStorage — JS-only, larger.
- Cookies — sent automatically, can be httpOnly (no XSS exposure).

**Q3: IndexedDB?**
NoSQL store in the browser. Async. Use via libraries like `idb`, `Dexie`, `RxDB`.

**Q4: When to use what?**
- Theme/language — localStorage.
- Auth — httpOnly cookie.
- Large user data — IndexedDB.
- Offline assets — Cache API.

**Q5: Quota exceeded?**
localStorage throws QuotaExceededError. IndexedDB prompts the user (in some browsers). Always handle failures.

---

## 23. Security Basics

### Q&A

**Q1: XSS (Cross-Site Scripting)?**
Attacker injects script into your page. Prevent by **never using `dangerouslySetInnerHTML`** unless you sanitize, and never trusting user input.

```jsx
// BAD
<div dangerouslySetInnerHTML={{ __html: user.bio }} />

// GOOD
<div>{user.bio}</div>
```

**Q2: CSP (Content Security Policy)?**
HTTP header telling the browser what sources are allowed.

```
Content-Security-Policy: default-src 'self'; img-src https://cdn.example.com
```

**Q3: CSRF (Cross-Site Request Forgery)?**
Tricks a logged-in user's browser into submitting a request.
- Use `SameSite=Lax`/`Strict` cookies.
- Use CSRF tokens for state-changing requests.

**Q4: HTTPS / TLS?**
Encrypts HTTP. Required in production. Free via Let's Encrypt.

**Q5: Sensitive data in URL?**
Never put tokens or passwords in URLs (logged in proxies / browser history).

**Q6: Subresource Integrity?**
```html
<script src="https://cdn/x.js"
 integrity="sha384-..."
 crossorigin="anonymous"></script>
```
Browser verifies hash.

**Q7: `rel="noopener noreferrer"`?**
For `target="_blank"` links — prevents the new tab from accessing `window.opener`.

**Q8: Dependency vulnerabilities?**
`npm audit`, Snyk, Dependabot. Patch regularly.

**Q9: Clickjacking?**
Use `X-Frame-Options: DENY` or CSP `frame-ancestors 'none'`.

**Q10: Token storage?**
- httpOnly cookies — best for auth.
- localStorage — vulnerable to XSS.

---

## 24. Web Performance Metrics (Web Vitals)

### Q&A

**Q1: Core Web Vitals.**
- **LCP (Largest Contentful Paint)** — should be < 2.5s.
- **INP (Interaction to Next Paint)** — < 200 ms.
- **CLS (Cumulative Layout Shift)** — < 0.1.

**Q2: Other useful metrics.**
- **FCP** (First Contentful Paint) — first text/image.
- **TTFB** (Time to First Byte) — server responsiveness.
- **FID** (replaced by INP).
- **TBT** (Total Blocking Time) — long tasks blocking main thread.

**Q3: How to measure?**
- **Lab:** Lighthouse, WebPageTest, Chrome DevTools.
- **Field:** Real User Monitoring (RUM) — web-vitals JS library, CrUX.

**Q4: Optimize LCP.**
- SSR/SSG the largest element.
- Preload critical resources (`<link rel="preload">`).
- CDNs, smaller images.

**Q5: Optimize INP.**
- Break up long tasks (`scheduler.yield()`, `setTimeout`).
- Defer non-urgent work (`useTransition`).
- Avoid heavy work on input handlers.

**Q6: Optimize CLS.**
- Always set `width` and `height` on images/videos.
- Reserve space for ads/embeds.
- Avoid inserting content above existing content.

**Q7: Reduce bundle size?**
Tree-shake, code-split, lazy-load routes.

**Q8: Reduce main-thread work?**
- Web Workers for heavy compute.
- Avoid layout thrashing.
- Use `transform` / `opacity` for animations (composited).

**Q9: Image formats.**
WebP, AVIF are smaller than JPEG/PNG. Use `<picture>` with fallbacks.

**Q10: Font loading?**
```css
@font-face { font-display: swap; }
```
Avoid invisible text (FOIT) and layout shift.

---

## 25. Browser Rendering Internals

### Q&A

**Q1: How does the browser render a page?**
1. HTML parsing → DOM tree.
2. CSS parsing → CSSOM.
3. Combine → Render Tree.
4. **Layout** — compute geometry.
5. **Paint** — draw pixels.
6. **Composite** — combine layers and send to GPU.

**Q2: Reflow vs Repaint?**
- **Reflow** — layout recalculation. Triggers: size, position, content change.
- **Repaint** — repaint pixels. Cheaper.

**Q3: Layout thrashing?**
Read/write layout in a loop — each read forces a sync layout. Batch reads then writes.

```js
// BAD
els.forEach(el => { const h = el.offsetHeight; el.style.height = h + 10 + "px"; });

// GOOD
const hs = els.map(el => el.offsetHeight);
els.forEach((el, i) => el.style.height = hs[i] + 10 + "px");
```

**Q4: Critical rendering path?**
The minimum work to render the first pixels: HTML → CSS → render-blocking JS → first paint.

**Q5: `<script>` placement?**
Default is render-blocking. Put `<script defer>` in `<head>` or `<script type="module">` (deferred by default).

**Q6: What is GPU acceleration?**
`transform` and `opacity` are composited on the GPU → very smooth. Top/left/width force layout.

**Q7: Long tasks?**
JS tasks > 50 ms block the main thread. Break them up.

**Q8: requestAnimationFrame?**
Schedules a callback before the next paint. Best for animations.

```js
function tick() { /* ... */ requestAnimationFrame(tick); }
requestAnimationFrame(tick);
```

---

## 26. PWA, Service Workers, WebSockets

### Q&A

**Q1: What is a PWA?**
A web app that behaves like a native app: installable, offline-capable, push notifications.

**Q2: Service Worker?**
A JS file that runs in a separate thread, intercepting network requests. Enables offline + caching.

```js
self.addEventListener("install", (e) => {
 e.waitUntil(caches.open("v1").then(c => c.addAll(["/"])));
});
self.addEventListener("fetch", (e) => {
 e.respondWith(caches.match(e.request).then(r => r || fetch(e.request)));
});
```

**Q3: Web App Manifest?**
JSON file declaring app name, icons, theme color, display mode.

**Q4: Web Push?**
Subscribe to push messages from server via browser (Notification API + Push API).

**Q5: WebSocket lifecycle.**
- Client `new WebSocket(url)`.
- Events: `open`, `message`, `close`, `error`.
- `socket.send(JSON.stringify(payload))`.
- Reconnect logic on close.

**Q6: SSE vs WebSocket?**
- **SSE** — one-way, simpler, over HTTP, auto-reconnect.
- **WebSocket** — bidirectional, lower overhead, separate protocol.

**Q7: Common WebSocket libraries?**
Socket.IO (with fallbacks), native `WebSocket`, ws-server (Node).

**Q8: Message ordering?**
Yes, WebSocket preserves order. At-most-once delivery; reconnection may miss messages — implement sequencing.

---

## 27. Coding Challenges (Frontend)

Each challenge with solution in JS.

### 1. Debounce
```js
function debounce(fn, ms) {
 let t;
 return (...args) => {
 clearTimeout(t);
 t = setTimeout(() => fn(...args), ms);
 };
}
```

### 2. Throttle
```js
function throttle(fn, ms) {
 let last = 0;
 return (...args) => {
 const now = Date.now();
 if (now - last >= ms) {
 last = now;
 fn(...args);
 }
 };
}
```

### 3. `myMap`, `myFilter`, `myReduce`
```js
Array.prototype.myMap = function (fn) {
 const out = [];
 for (let i = 0; i < this.length; i++) out.push(fn(this[i], i, this));
 return out;
};
```

### 4. Flatten nested array
```js
function flatten(arr) {
 return arr.reduce((a, x) => a.concat(Array.isArray(x) ? flatten(x) : x), []);
}
// Or modern: arr.flat(Infinity)
```

### 5. Deep clone
```js
function deepClone(v, seen = new WeakMap()) {
 if (v === null || typeof v !== "object") return v;
 if (seen.has(v)) return seen.get(v);
 if (v instanceof Date) return new Date(v);
 if (Array.isArray(v)) return v.map(x => deepClone(x, seen));
 const out = {};
 seen.set(v, out);
 for (const k of Object.keys(v)) out[k] = deepClone(v[k], seen);
 return out;
}
// Or: structuredClone(v)
```

### 6. `Promise.all` polyfill
```js
function promiseAll(promises) {
 return new Promise((resolve, reject) => {
 const out = new Array(promises.length);
 let done = 0;
 if (promises.length === 0) return resolve([]);
 promises.forEach((p, i) => {
 Promise.resolve(p).then(v => {
 out[i] = v;
 if (++done === promises.length) resolve(out);
 }, reject);
 });
 });
}
```

### 7. Event emitter
```js
class Emitter {
 constructor() { this.handlers = new Map(); }
 on(event, fn) {
 if (!this.handlers.has(event)) this.handlers.set(event, new Set());
 this.handlers.get(event).add(fn);
 return () => this.handlers.get(event).delete(fn);
 }
 emit(event, ...args) {
 for (const fn of this.handlers.get(event) ?? []) fn(...args);
 }
}
```

### 8. JSON.stringify implementation (basic)
```js
function stringify(v) {
 if (v === null) return "null";
 if (typeof v === "string") return JSON.stringify(v);
 if (typeof v === "number" || typeof v === "boolean") return String(v);
 if (Array.isArray(v)) return "[" + v.map(stringify).join(",") + "]";
 if (typeof v === "object") {
 return "{" + Object.entries(v).map(([k, val]) => stringify(k) + ":" + stringify(val)).join(",") + "}";
 }
 return undefined;
}
```

### 9. Currying
```js
function curry(fn) {
 return function curried(...args) {
 if (args.length >= fn.length) return fn.apply(this, args);
 return (...more) => curried.apply(this, [...args, ...more]);
 };
}
```

### 10. React: build a Star Rating component
```jsx
function StarRating({ value, onChange }) {
 return (
 <div role="radiogroup">
 {[1, 2, 3, 4, 5].map(n => (
 <button key={n} role="radio" aria-checked={value === n} onClick={() => onChange(n)}>★</button>
 ))}
 </div>
 );
}
```

### 11. React: build a Modal with portal
```jsx
function Modal({ open, onClose, children }) {
 if (!open) return null;
 return createPortal(
 <div className="overlay" onClick={onClose}>
 <div className="modal" onClick={e => e.stopPropagation()}>
 <button onClick={onClose} aria-label="Close">×</button>
 {children}
 </div>
 </div>,
 document.body
 );
}
```

### 12. React: useDebounce hook
```jsx
function useDebounce<T>(value: T, ms: number): T {
 const [v, setV] = useState(value);
 useEffect(() => {
 const t = setTimeout(() => setV(value), ms);
 return () => clearTimeout(t);
 }, [value, ms]);
 return v;
}
```

### 13. React: usePrevious hook
```jsx
function usePrevious<T>(value: T): T | undefined {
 const ref = useRef<T | undefined>(undefined);
 useEffect(() => { ref.current = value; }, [value]);
 return ref.current;
```

### 14. React: useIntersectionObserver hook
```jsx
function useInView(ref) {
 const [inView, setInView] = useState(false);
 useEffect(() => {
 const io = new IntersectionObserver(
 ([entry]) => setInView(entry.isIntersecting),
 { threshold: 0.1 }
 );
 if (ref.current) io.observe(ref.current);
 return () => io.disconnect();
 }, [ref]);
 return inView;
}
```

### 15. Implement a simple virtual list
```jsx
function VirtualList({ items, rowHeight, height }) {
 const [scrollTop, setScrollTop] = useState(0);
 const start = Math.floor(scrollTop / rowHeight);
 const visibleCount = Math.ceil(height / rowHeight);
 const end = start + visibleCount;
 const offsetY = start * rowHeight;
 return (
 <div style={{ height, overflow: "auto" }} onScroll={e => setScrollTop(e.currentTarget.scrollTop)}>
 <div style={{ height: items.length * rowHeight, position: "relative" }}>
 <div style={{ transform: `translateY(${offsetY}px)` }}>
 {items.slice(start, end).map((it, i) => (
 <div key={start + i} style={{ height: rowHeight }}>{it}</div>
 ))}
 </div>
 </div>
 </div>
 );
}
```

---

## 28. Behavioral & System Design for Frontend

### Theory

Frontend system design interviews ask you to design the **client-side** architecture for an app: components, data fetching, state, performance, real-time updates, and offline behavior.

### Q&A

**Q1: Design an autocomplete search bar (like Google).**
- Debounce input (~200 ms).
- Show recent searches.
- Call API `GET /suggest?q=...`.
- Show top 10 in dropdown.
- Keyboard nav (arrows, enter).
- Throttle analytics.
- Prefetch next page on scroll.
- Cache results per prefix for a few seconds.

**Q2: Design an infinite scroll feed.**
- Fetch first page on mount.
- Use `IntersectionObserver` on a sentinel at the bottom.
- Append results, advance cursor.
- Keep a window of items (avoid DOM explosion).
- Optimistic update on user actions.

**Q3: Design an image gallery like Instagram.**
- Grid of thumbnails.
- Tap → modal with full image.
- Lazy load images below the fold.
- Pre-fetch next page.
- Cache images in IndexedDB for offline.

**Q4: Design a real-time chat widget.**
- WebSocket connection to chat server.
- Optimistic local append.
- Mark messages delivered on server ack.
- Reconnect with backoff.
- Re-fetch missed messages on resume.
- Show typing indicator via presence events.

**Q5: Design a notifications dropdown.**
- WebSocket or SSE for live updates.
- Mark as read on click.
- Batch unread count.
- Persist seen state in localStorage.

**Q6: How do you decide between CSR and SSR?**
- Public, SEO-critical → SSR/SSG.
- Auth-only dashboards → CSR.
- Mix → hybrid (Next.js with selective SSR).

**Q7: How to handle slow networks?**
- Skeleton loaders.
- Optimistic updates.
- Retry with backoff.
- Local cache.
- Graceful degradation.

**Q8: How do you debug a slow app?**
- React DevTools Profiler — find re-render hot spots.
- Chrome Performance tab — find long tasks.
- Bundle analyzer — find big deps.
- Lighthouse — find LCP/CLS/INP issues.

**Q9: Common behavioral questions.**
- "Tell me about a tricky UI bug you fixed."
- "How do you balance speed and quality?"
- "How do you keep up with frontend changes?"

Use **STAR** (Situation, Task, Action, Result) and quantify outcomes.

---

## 29. Quick Reference Card

| Concept | One-liner |
|---------|-----------|
| JSX | HTML-like syntax in JS |
| Virtual DOM | In-memory copy of DOM |
| Props | Read-only inputs from parent |
| State | Component-owned mutable data |
| Hook | Function that uses React features |
| useEffect | Run side effects after render |
| useMemo | Cache a computed value |
| useCallback | Cache a function |
| useRef | Persistent mutable reference |
| useTransition | Mark updates as non-urgent |
| useDeferredValue | Defer a value update |
| Context | Share data without prop drilling |
| Suspense | Show fallback while loading |
| Server Component | Renders only on server |
| Server Action | Server-side mutation from client |
| StrictMode | Dev-only invariant checks |
| CSR / SSR / SSG / ISR | Render strategies |
| LCP / INP / CLS | Core Web Vitals |
| CORS | Cross-origin policy |
| XSS / CSRF | Injection / request forgery attacks |
| CSP | Content Security Policy header |
| JWT | Signed stateless token |
| localStorage / IndexedDB / Cookies | Browser storage |
| Service Worker | Offline + push |
| HMR | Hot module replacement |
| Tree shaking | Remove unused code |
| ESLint / Prettier | Lint / format |

---

Good luck with your frontend interviews!
