# Frontend Interview Preparation (React + TypeScript)

This file covers the most-asked questions in frontend developer interviews at product companies (FAANG, startups, and service-based). The focus is **React + TypeScript**, with extra sections on **HTML, CSS, JavaScript fundamentals, performance, testing, and accessibility**.

---

## Table of Contents

1. [HTML & CSS Fundamentals](#1-html--css-fundamentals)
2. [JavaScript Core Concepts](#2-javascript-core-concepts)
3. [TypeScript Essentials](#3-typescript-essentials)
4. [React Fundamentals](#4-react-fundamentals)
5. [React Hooks (Deep Dive)](#5-react-hooks-deep-dive)
6. [React Advanced Concepts](#6-react-advanced-concepts)
7. [State Management](#7-state-management)
8. [Routing in React](#8-routing-in-react)
9. [Forms and Validation](#9-forms-and-validation)
10. [Performance Optimization](#10-performance-optimization)
11. [Testing](#11-testing)
12. [Accessibility (a11y)](#12-accessibility-a11y)
13. [Build Tools & Bundlers](#13-build-tools--bundlers)
14. [Networking & APIs](#14-networking--apis)
15. [Browser Storage](#15-browser-storage)
16. [Security Basics](#16-security-basics)
17. [Web Performance Metrics](#17-web-performance-metrics)
18. [Coding Challenges (Frontend)](#18-coding-challenges-frontend)
19. [Behavioral & System Design for Frontend](#19-behavioral--system-design-for-frontend)

---

## 1. HTML & CSS Fundamentals

### Q1: What is the difference between `<div>` and `<span>`?
**Answer:**
- `<div>` is a **block-level** element. It starts on a new line and takes the full width available.
- `<span>` is an **inline** element. It only takes as much width as its content and does not start on a new line.
- Use `<div>` for layout blocks. Use `<span>` for small inline text styling.

```html
<div>This is a block.</div>
<span>This is inline.</span>
```

### Q2: What is semantic HTML? Give examples.
**Answer:** Semantic HTML uses tags that describe the meaning of the content, not just how it looks. It helps with SEO and accessibility.
**Examples:** `<header>`, `<nav>`, `<main>`, `<article>`, `<section>`, `<aside>`, `<footer>`, `<figure>`.

```html
<article>
  <header><h1>Title</h1></header>
  <section>Body content</section>
  <footer>Author info</footer>
</article>
```

### Q3: Explain the CSS box model.
**Answer:** Every HTML element is a rectangular box made of 4 layers (from inside to outside):
1. **Content** — the actual text or image
2. **Padding** — space between content and border
3. **Border** — line around the padding
4. **Margin** — space outside the border, between this element and others

```css
.box {
  width: 200px;
  padding: 10px;
  border: 2px solid black;
  margin: 20px;
}
```

### Q4: What is `box-sizing: border-box`?
**Answer:** By default, `width` and `height` only include the content. With `border-box`, they include content + padding + border. This makes layout much easier.

```css
* { box-sizing: border-box; }
```

### Q5: Difference between `display: none`, `visibility: hidden`, and `opacity: 0`?
**Answer:**
| Property | Takes space? | Clickable? | In layout? |
|----------|--------------|------------|------------|
| `display: none` | No | No | Removed |
| `visibility: hidden` | Yes | No | Hidden but reserved |
| `opacity: 0` | Yes | No | Transparent |

### Q6: What is the difference between Flexbox and Grid?
**Answer:**
- **Flexbox** is **1-dimensional** (row OR column). Best for components and small layouts.
- **Grid** is **2-dimensional** (rows AND columns). Best for page layouts.

```css
/* Flexbox */
.flex { display: flex; justify-content: space-between; align-items: center; }

/* Grid */
.grid { display: grid; grid-template-columns: 1fr 2fr 1fr; gap: 10px; }
```

### Q7: What is the CSS specificity order?
**Answer:** From highest to lowest:
1. `!important`
2. Inline styles (`style="..."`)
3. IDs (`#myId`)
4. Classes, attributes, pseudo-classes (`.myClass`, `[type="text"]`, `:hover`)
5. Elements and pseudo-elements (`div`, `::before`)
6. Order in source code (last wins for same specificity)

### Q8: What are pseudo-classes and pseudo-elements?
**Answer:**
- **Pseudo-class** describes a special state. Example: `:hover`, `:focus`, `:first-child`.
- **Pseudo-element** lets you style a specific part of an element. Example: `::before`, `::after`, `::placeholder`.

```css
a:hover { color: red; }       /* pseudo-class */
p::first-letter { font-size: 2em; }  /* pseudo-element */
```

### Q9: What is the difference between `relative`, `absolute`, `fixed`, and `sticky` positioning?
**Answer:**
- `static` (default): normal flow.
- `relative`: stays in normal flow but can be offset with `top/left/right/bottom`.
- `absolute`: removed from flow, positioned relative to nearest positioned ancestor.
- `fixed`: positioned relative to the viewport. Stays in place when scrolling.
- `sticky`: acts like `relative` until a scroll point, then becomes `fixed`.

### Q10: What are CSS variables (custom properties)?
**Answer:** Reusable values you define once and use throughout your CSS.

```css
:root {
  --primary: #3498db;
  --font-size: 16px;
}
.button {
  background: var(--primary);
  font-size: var(--font-size);
}
```

---

## 2. JavaScript Core Concepts

### Q1: What are the different data types in JavaScript?
**Answer:**
- **Primitive types:** `string`, `number`, `boolean`, `null`, `undefined`, `symbol`, `bigint`.
- **Non-primitive (reference) types:** `object` (including arrays, functions, dates).

### Q2: Difference between `==` and `===`?
**Answer:**
- `==` compares values **with type coercion**.
- `===` compares values **without coercion** (strict equality).

```js
"5" == 5   // true  (coerces string to number)
"5" === 5  // false (different types)
```

**Always prefer `===`.**

### Q3: What is hoisting?
**Answer:** JavaScript moves **declarations** to the top of their scope before code runs.
- `var` declarations are hoisted and initialized with `undefined`.
- `let` and `const` are hoisted but not initialized (they are in the **temporal dead zone** until the line where they are declared).

```js
console.log(a); // undefined
var a = 5;

console.log(b); // ReferenceError
let b = 5;
```

### Q4: Difference between `var`, `let`, and `const`?
**Answer:**
| Feature | `var` | `let` | `const` |
|---------|-------|-------|---------|
| Scope | Function | Block | Block |
| Reassign | Yes | Yes | No |
| Redeclare | Yes | No | No |
| Hoisted | Yes (undefined) | Yes (TDZ) | Yes (TDZ) |

### Q5: What is closure? Give an example.
**Answer:** A closure is a function that remembers variables from its outer scope even after that outer function has returned.

```js
function outer() {
  let count = 0;
  return function inner() {
    count++;
    console.log(count);
  };
}
const fn = outer();
fn(); // 1
fn(); // 2
fn(); // 3
```

### Q6: What is the `this` keyword?
**Answer:** `this` refers to the object that is calling the function. Its value depends on how the function is called:
- In a method: the owner object.
- Alone: the global object (or `undefined` in strict mode).
- In an arrow function: the surrounding lexical scope.
- With `call`, `apply`, `bind`: the passed object.

```js
const obj = {
  name: "Alice",
  greet() { console.log(this.name); }
};
obj.greet(); // "Alice"
```

### Q7: Explain `call`, `apply`, and `bind`.
**Answer:** All three let you set the value of `this` explicitly.
- `fn.call(thisArg, arg1, arg2)` — calls immediately, args separated by commas.
- `fn.apply(thisArg, [args])` — calls immediately, args as array.
- `fn.bind(thisArg)` — returns a new function with `this` bound, does not call.

```js
function say(greeting) { console.log(greeting + ", " + this.name); }
const person = { name: "Bob" };
say.call(person, "Hi");        // "Hi, Bob"
say.apply(person, ["Hello"]);  // "Hello, Bob"
const bound = say.bind(person);
bound("Hey");                  // "Hey, Bob"
```

### Q8: What are arrow functions? How are they different?
**Answer:** Shorter syntax for writing functions. Differences:
- They do **not** have their own `this` (lexical `this`).
- They cannot be used as constructors (no `new`).
- They cannot be used as generator functions.

```js
const add = (a, b) => a + b;
```

### Q9: What is event bubbling and capturing?
**Answer:**
- **Bubbling:** Event goes from the target up to the root (default).
- **Capturing:** Event goes from the root down to the target.
- `addEventListener(type, fn, useCapture)` — pass `true` for capture phase.

```js
document.querySelector(".child").addEventListener("click", e => {
  e.stopPropagation(); // stop the event from bubbling further
});
```

### Q10: What is the event loop? How does async JavaScript work?
**Answer:** JavaScript is **single-threaded**. The event loop manages async operations using:
1. **Call stack** — runs synchronous code.
2. **Web APIs** — handle `setTimeout`, `fetch`, DOM events.
3. **Callback queue (macrotask queue)** — holds callbacks from Web APIs.
4. **Microtask queue** — holds Promise callbacks (higher priority).

The event loop runs callbacks from the microtask queue first, then takes the next macrotask.

```js
console.log("1");
setTimeout(() => console.log("2"), 0);
Promise.resolve().then(() => console.log("3"));
console.log("4");
// Output: 1, 4, 3, 2
```

### Q11: What are Promises? What are their states?
**Answer:** A Promise represents a value that may be available now, later, or never. It has 3 states:
- **Pending** — initial state.
- **Fulfilled** — operation completed successfully.
- **Rejected** — operation failed.

```js
const p = new Promise((resolve, reject) => {
  setTimeout(() => resolve("done"), 1000);
});
p.then(v => console.log(v)).catch(e => console.error(e));
```

### Q12: What is `async/await`?
**Answer:** A cleaner way to write Promise-based code. `await` pauses execution until the Promise resolves.

```js
async function getUser(id) {
  try {
    const res = await fetch(`/api/users/${id}`);
    return await res.json();
  } catch (err) {
    console.error(err);
  }
}
```

### Q13: What is prototypal inheritance?
**Answer:** Every JavaScript object has a hidden `[[Prototype]]` link to another object (or `null`). When accessing a property that does not exist on an object, JS looks up the prototype chain.

```js
const animal = { eats: true };
const dog = Object.create(animal);
console.log(dog.eats); // true (inherited)
```

### Q14: Difference between `null` and `undefined`?
**Answer:**
- `undefined` means a variable has been declared but not assigned a value.
- `null` is an assignment value that represents "no value" or "empty" intentionally.

```js
let a;          // undefined
let b = null;   // null
```

### Q15: What is the difference between `map`, `filter`, and `reduce`?
**Answer:**
- `map` — transforms each element, returns a new array of same length.
- `filter` — keeps elements that pass a test, returns a smaller array.
- `reduce` — combines all elements into a single value.

```js
const nums = [1, 2, 3, 4];
nums.map(n => n * 2);        // [2, 4, 6, 8]
nums.filter(n => n % 2);     // [1, 3]
nums.reduce((sum, n) => sum + n, 0); // 10
```

### Q16: What is destructuring?
**Answer:** A syntax to unpack values from arrays or objects into separate variables.

```js
const [a, b] = [1, 2];
const { name, age } = { name: "Alice", age: 30 };
```

### Q17: What is the spread operator?
**Answer:** `...` expands an iterable into individual elements. Useful for copying or merging arrays/objects.

```js
const a = [1, 2];
const b = [...a, 3, 4];           // [1, 2, 3, 4]
const obj = { ...{ x: 1 }, y: 2 }; // { x: 1, y: 2 }
```

### Q18: What is debouncing vs throttling?
**Answer:** Both limit how often a function runs. Useful for scroll, resize, search input.
- **Debounce:** Run only after the user stops calling for N ms.
- **Throttle:** Run at most once every N ms.

```js
function debounce(fn, ms) {
  let t;
  return (...args) => {
    clearTimeout(t);
    t = setTimeout(() => fn(...args), ms);
  };
}
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

### Q19: What are JavaScript modules? ES Modules vs CommonJS?
**Answer:**
- **CommonJS** (Node.js default): `require()` / `module.exports`.
- **ES Modules** (browsers, modern Node): `import` / `export`.

```js
// ES Module
import { useState } from "react";
export const PI = 3.14;

// CommonJS
const { useState } = require("react");
module.exports = { PI: 3.14 };
```

### Q20: What is `JSON`?
**Answer:** JavaScript Object Notation. A text format for data exchange. Two main methods:
- `JSON.stringify(obj)` — object to string.
- `JSON.parse(str)` — string to object.

---

## 3. TypeScript Essentials

### Q1: What is TypeScript? Why use it?
**Answer:** TypeScript is a superset of JavaScript that adds **static typing**. It catches errors at compile time instead of runtime, makes code easier to refactor, and improves autocomplete.

### Q2: What are the basic types in TypeScript?
**Answer:**

```ts
let name: string = "Alice";
let age: number = 30;
let active: boolean = true;
let data: any = "anything";      // opt out of type checking
let unknown: unknown = 5;        // safer than any
let nums: number[] = [1, 2, 3];
let tuple: [string, number] = ["age", 30];
let nothing: void = undefined;
let never: never;                // function that never returns
```

### Q3: What are interfaces and type aliases?
**Answer:** Both define the shape of an object. Interfaces are best for objects and can be extended. Type aliases can represent unions, primitives, and tuples.

```ts
interface User { name: string; age: number; }
type ID = string | number;
type Point = [number, number];
```

### Q4: What is the difference between `interface` and `type`?
**Answer:**
- `interface` can be **merged** (declaration merging). Best for object shapes and class contracts.
- `type` is more flexible (unions, intersections, primitives). Cannot merge.

### Q5: What are generics?
**Answer:** Generics let you write code that works with any type while keeping type safety.

```ts
function identity<T>(arg: T): T { return arg; }
identity<string>("hi");
identity(42); // T inferred as number
```

### Q6: What are union and intersection types?
**Answer:**
- **Union** (`|`): value can be one of several types.
- **Intersection** (`&`): value must satisfy all types.

```ts
type Result = string | number;
type Both = { a: string } & { b: number };
```

### Q7: What is `enum`?
**Answer:** A way to define a set of named constants.

```ts
enum Color { Red, Green, Blue }
let c: Color = Color.Green; // 1
```

### Q8: What are utility types?
**Answer:** Built-in types that transform other types.
- `Partial<T>` — all properties optional.
- `Required<T>` — all properties required.
- `Readonly<T>` — no mutation.
- `Pick<T, K>` — pick specific keys.
- `Omit<T, K>` — remove specific keys.
- `Record<K, V>` — object type with given keys and values.

```ts
interface User { id: number; name: string; email: string; }
type UserPreview = Pick<User, "id" | "name">;
type UserUpdate = Partial<User>;
```

### Q9: What is `unknown` vs `any`?
**Answer:**
- `any` — disables type checking completely. Avoid.
- `unknown` — type-safe alternative. You must narrow before use.

```ts
let v: unknown = "hello";
if (typeof v === "string") console.log(v.toUpperCase()); // OK
```

### Q10: How do you type a function?
**Answer:**

```ts
function add(a: number, b: number): number { return a + b; }
const greet = (name: string): string => `Hello, ${name}`;
```

### Q11: What is type narrowing?
**Answer:** Narrowing down the type of a variable based on checks.

```ts
function print(value: string | number) {
  if (typeof value === "string") console.log(value.toUpperCase());
  else console.log(value.toFixed(2));
}
```

### Q12: What is `as const`?
**Answer:** Makes an object/array deeply readonly with literal types.

```ts
const dirs = ["up", "down"] as const;
type Dir = typeof dirs[number]; // "up" | "down"
```

---

## 4. React Fundamentals

### Q1: What is React? Why use it?
**Answer:** React is a JavaScript library for building user interfaces using **components**. Benefits:
- **Component-based** — reusable, easy to manage.
- **Virtual DOM** — fast updates.
- **Declarative** — you describe what UI should look like, not how to change it.
- Huge ecosystem.

### Q2: What is JSX?
**Answer:** JSX is a syntax extension that lets you write HTML-like code in JavaScript. It gets compiled to `React.createElement` calls.

```jsx
const el = <h1 className="title">Hello</h1>;
// Compiles to:
React.createElement("h1", { className: "title" }, "Hello");
```

### Q3: What is the Virtual DOM?
**Answer:** A lightweight JavaScript representation of the real DOM kept in memory. When state changes, React:
1. Creates a new virtual DOM tree.
2. Compares it with the previous one (**diffing**).
3. Updates only the changed parts in the real DOM (**reconciliation**).

### Q4: What is a component? Functional vs Class component?
**Answer:** A component is a reusable piece of UI.
- **Functional component:** A plain JavaScript function. (Modern standard.)
- **Class component:** ES6 class that extends `React.Component`. (Legacy.)

```jsx
function Welcome({ name }) {
  return <h1>Hello, {name}</h1>;
}
```

### Q5: What are props?
**Answer:** Props are inputs passed from a parent component to a child. They are **read-only**.

```jsx
function Greeting({ name, age }) {
  return <p>{name} is {age} years old</p>;
}
<Greeting name="Alice" age={30} />
```

### Q6: What is state?
**Answer:** State is data that a component manages internally. When state changes, the component re-renders.

```jsx
function Counter() {
  const [count, setCount] = useState(0);
  return <button onClick={() => setCount(count + 1)}>{count}</button>;
}
```

### Q7: Difference between state and props?
**Answer:**
- **Props:** Passed from parent, read-only, cannot be modified by child.
- **State:** Owned by the component, can be changed with setter, triggers re-render.

### Q8: What is the difference between `useState` and `useReducer`?
**Answer:**
- `useState` — best for simple state (one or two values).
- `useReducer` — best for complex state with multiple sub-values or transitions.

```jsx
const [state, dispatch] = useReducer(reducer, initialState);
```

### Q9: What is the `key` prop? Why is it important?
**Answer:** `key` is a special prop that helps React identify which items in a list have changed. It must be **unique among siblings**. Using indexes as keys is a bad practice if the list can reorder.

```jsx
items.map(item => <li key={item.id}>{item.name}</li>)
```

### Q10: What is conditional rendering?
**Answer:** Showing different UI based on a condition.

```jsx
{isLoggedIn ? <Dashboard /> : <Login />}
{loading && <Spinner />}
```

### Q11: How do you handle events in React?
**Answer:** Use camelCase event names and pass a function.

```jsx
<button onClick={handleClick}>Click</button>
<input onChange={e => setName(e.target.value)} />
```

### Q12: How do you pass data from child to parent?
**Answer:** Pass a callback function from parent to child as a prop.

```jsx
function Parent() {
  const [msg, setMsg] = useState("");
  return <Child onSend={setMsg} />;
}
function Child({ onSend }) {
  return <button onClick={() => onSend("hi")}>Send</button>;
}
```

### Q13: What is `React.Fragment`?
**Answer:** Lets you return multiple elements without adding an extra DOM node.

```jsx
return (
  <>
    <h1>Title</h1>
    <p>Body</p>
  </>
);
```

### Q14: What is `children` prop?
**Answer:** It refers to whatever JSX you put between the opening and closing tags of a component.

```jsx
<Card>
  <h1>Inside</h1>
</Card>
// In Card: props.children === <h1>Inside</h1>
```

---

## 5. React Hooks (Deep Dive)

### Q1: What are Hooks? Rules of Hooks?
**Answer:** Hooks let you use state and other React features in functional components.
**Rules:**
1. Only call Hooks at the **top level** of a component (not inside loops, conditions, or nested functions).
2. Only call Hooks from **React functions** (components or custom hooks).

### Q2: Explain `useState` in detail.
**Answer:** Returns a state value and a setter. State updates are **batched** and **async**.

```jsx
const [count, setCount] = useState(0);
// Functional update (safer when new state depends on old):
setCount(prev => prev + 1);
```

### Q3: Explain `useEffect`.
**Answer:** Runs side effects after render. Takes a function and an optional dependency array.

```jsx
useEffect(() => {
  // runs after every render (no deps array)
  console.log("rendered");
  return () => console.log("cleanup"); // optional cleanup
}, [count]); // runs when count changes
```

**Dependency array behavior:**
- No array → runs after every render.
- `[]` → runs only once (on mount).
- `[a, b]` → runs when `a` or `b` changes.

### Q4: What is the cleanup function in `useEffect`?
**Answer:** Returned function that runs before the next effect and on unmount. Used to cancel timers, remove listeners, abort fetches.

```jsx
useEffect(() => {
  const id = setInterval(tick, 1000);
  return () => clearInterval(id);
}, []);
```

### Q5: Explain `useContext`.
**Answer:** Lets you share values between components without passing props down manually.

```jsx
const ThemeContext = createContext("light");
function App() {
  return (
    <ThemeContext.Provider value="dark">
      <Child />
    </ThemeContext.Provider>
  );
}
function Child() {
  const theme = useContext(ThemeContext);
  return <div className={theme}>...</div>;
}
```

### Q6: Explain `useRef`.
**Answer:** Returns a mutable object `{ current: ... }` that persists across renders without causing re-renders. Used for DOM access or storing mutable values.

```jsx
const inputRef = useRef(null);
<input ref={inputRef} />;
inputRef.current.focus();
```

### Q7: Explain `useMemo`.
**Answer:** Memoizes a computed value. Only recomputes when dependencies change.

```jsx
const sorted = useMemo(() => items.sort((a, b) => a.price - b.price), [items]);
```

### Q8: Explain `useCallback`.
**Answer:** Memoizes a function reference. Useful when passing callbacks to memoized children.

```jsx
const handleClick = useCallback(() => doSomething(id), [id]);
```

### Q9: Difference between `useMemo` and `useCallback`?
**Answer:**
- `useMemo(fn, deps)` — returns the **result** of `fn`.
- `useCallback(fn, deps)` — returns the **function** itself.

`useCallback(fn, deps)` is equivalent to `useMemo(() => fn, deps)`.

### Q10: Explain `useReducer`.
**Answer:** Alternative to `useState` for complex state logic.

```jsx
function reducer(state, action) {
  switch (action.type) {
    case "inc": return { count: state.count + 1 };
    case "dec": return { count: state.count - 1 };
  }
}
const [state, dispatch] = useReducer(reducer, { count: 0 });
dispatch({ type: "inc" });
```

### Q11: What is a custom hook?
**Answer:** A reusable function that uses other hooks. Naming must start with `use`.

```jsx
function useFetch(url) {
  const [data, setData] = useState(null);
  useEffect(() => { fetch(url).then(r => r.json()).then(setData); }, [url]);
  return data;
}
```

### Q12: Explain `useLayoutEffect`.
**Answer:** Same as `useEffect` but runs **synchronously** after DOM updates, before browser paints. Use for measurements or DOM mutations that must happen before paint.

### Q13: Explain `useImperativeHandle`.
**Answer:** Customizes the instance value exposed when a parent uses `ref` on a child. Used with `forwardRef`.

### Q14: Explain `useId`.
**Answer:** Generates a unique, stable ID. Useful for accessibility (linking labels to inputs).

```jsx
const id = useId();
<label htmlFor={id}>Name</label>
<input id={id} />
```

---

## 6. React Advanced Concepts

### Q1: What is React reconciliation?
**Answer:** The algorithm React uses to figure out the difference between two virtual DOM trees and update the real DOM with minimum operations.

### Q2: What are Higher-Order Components (HOC)?
**Answer:** A function that takes a component and returns a new component with extra behavior. (Mostly replaced by hooks today.)

```jsx
const withLogger = (Comp) => (props) => {
  useEffect(() => console.log(props), [props]);
  return <Comp {...props} />;
};
```

### Q3: What is the render prop pattern?
**Answer:** A component takes a function as a prop that returns JSX. The function is called with state/data.

```jsx
<Mouse render={pos => <p>{pos.x}, {pos.y}</p>} />
```

### Q4: What is `React.memo`?
**Answer:** Wraps a functional component to skip re-render if props are unchanged (shallow comparison).

```jsx
export default React.memo(MyComponent);
```

### Q5: What is `React.lazy`?
**Answer:** Lets you render a dynamic import as a component. Used for **code splitting**.

```jsx
const Settings = React.lazy(() => import("./Settings"));
<Suspense fallback={<Spinner />}><Settings /></Suspense>
```

### Q6: What is `Suspense`?
**Answer:** Lets you show a fallback UI while children are loading (lazy components, async data).

### Q7: What is `forwardRef`?
**Answer:** Lets a parent component get a `ref` to a child component's DOM element.

```jsx
const MyInput = forwardRef((props, ref) => <input ref={ref} {...props} />);
```

### Q8: What is the Context API? When would you use it?
**Answer:** A way to pass data through the component tree without props drilling. Best for **global** data like theme, language, authenticated user. For complex state, prefer Redux/Zustand.

### Q9: What is `useTransition`?
**Answer:** Marks a state update as non-urgent. UI stays responsive.

```jsx
const [pending, startTransition] = useTransition();
startTransition(() => setFilter(input));
```

### Q10: What is `useDeferredValue`?
**Answer:** Returns a deferred version of a value that lags behind updates. Useful for heavy renders.

### Q11: What is the difference between controlled and uncontrolled components?
**Answer:**
- **Controlled:** React state is the source of truth. You handle every change.
- **Uncontrolled:** The DOM holds the state. You read it using `ref` when needed.

```jsx
// Controlled
<input value={name} onChange={e => setName(e.target.value)} />

// Uncontrolled
<input ref={inputRef} defaultValue="" />
```

### Q12: What are error boundaries?
**Answer:** Components that catch JavaScript errors in their child tree and show a fallback UI instead of crashing.

```jsx
class ErrorBoundary extends React.Component {
  state = { hasError: false };
  static getDerivedStateFromError() { return { hasError: true }; }
  render() {
    return this.state.hasError ? <h1>Error!</h1> : this.props.children;
  }
}
```

### Q13: What is the difference between React and ReactDOM?
**Answer:**
- `react` — core library (components, hooks).
- `react-dom` — renders React components to the browser DOM.

### Q14: What is `StrictMode`?
**Answer:** A wrapper that activates extra checks (double-invokes effects in dev, warns about deprecated APIs). No effect in production.

```jsx
<React.StrictMode><App /></React.StrictMode>
```

---

## 7. State Management

### Q1: What is prop drilling? How do you avoid it?
**Answer:** Passing props through many layers just to reach a deep child. Solutions: Context API, Redux, Zustand, Recoil, Jotai.

### Q2: When should you use Redux?
**Answer:** When:
- Many components need the same state.
- State updates are complex.
- You need time-travel debugging, middleware, or devtools.

### Q3: Explain the Redux flow.
**Answer:**
1. UI dispatches an **action** (`dispatch({ type: "ADD" })`).
2. The **reducer** receives the action and returns a new state.
3. The **store** holds the state.
4. Components subscribe via `useSelector` and re-render on change.

```jsx
const dispatch = useDispatch();
dispatch({ type: "ADD_TODO", payload: "Buy milk" });
const todos = useSelector(state => state.todos);
```

### Q4: What is Redux Toolkit?
**Answer:** The official, recommended way to write Redux. It uses `createSlice` and `createAsyncThunk` to reduce boilerplate.

```jsx
const counterSlice = createSlice({
  name: "counter",
  initialState: { value: 0 },
  reducers: {
    increment: state => { state.value += 1; }
  }
});
```

### Q5: What is Zustand?
**Answer:** A small, fast state management library. Much simpler than Redux.

```jsx
import { create } from "zustand";
const useStore = create(set => ({
  count: 0,
  inc: () => set(state => ({ count: state.count + 1 }))
}));
```

### Q6: What is the difference between local state, context, and global state?
**Answer:**
- **Local state** (`useState`) — only one component.
- **Context** — shared across tree, low update frequency (theme, locale).
- **Global state** (Redux, Zustand) — many components update and read, complex logic.

### Q7: What is Recoil?
**Answer:** An experimental state management library from Meta. Uses **atoms** (units of state) and **selectors** (derived state). Works well with React Suspense.

---

## 8. Routing in React

### Q1: What is React Router?
**Answer:** The standard routing library for React. Maps URL paths to components.

```jsx
<BrowserRouter>
  <Routes>
    <Route path="/" element={<Home />} />
    <Route path="/about" element={<About />} />
    <Route path="/users/:id" element={<User />} />
  </Routes>
</BrowserRouter>
```

### Q2: What is the difference between `BrowserRouter` and `HashRouter`?
**Answer:**
- `BrowserRouter` uses the **History API** (clean URLs like `/about`).
- `HashRouter` uses the **hash** (`/#/about`). Useful for static hosts without server config.

### Q3: How do you navigate programmatically?
**Answer:** Use the `useNavigate` hook.

```jsx
const navigate = useNavigate();
navigate("/dashboard");
navigate(-1); // back
```

### Q4: How do you read URL params?
**Answer:** Use `useParams`.

```jsx
const { id } = useParams();
```

### Q5: What is `Link` vs `a` tag?
**Answer:** `Link` uses client-side routing (no full page reload). `a` does a full reload.

```jsx
<Link to="/about">About</Link>
```

---

## 9. Forms and Validation

### Q1: How do you handle forms in React?
**Answer:** Track input values in state, update on change, submit with handler.

```jsx
function Form() {
  const [email, setEmail] = useState("");
  const submit = e => {
    e.preventDefault();
    console.log(email);
  };
  return (
    <form onSubmit={submit}>
      <input value={email} onChange={e => setEmail(e.target.value)} />
      <button>Submit</button>
    </form>
  );
}
```

### Q2: What is Formik? What is React Hook Form?
**Answer:** Libraries that simplify form handling:
- **Formik** — handles state, validation, submission.
- **React Hook Form** — uses uncontrolled inputs and refs for performance.

```jsx
const { register, handleSubmit } = useForm();
<input {...register("email")} />
```

### Q3: How do you validate forms?
**Answer:**
- Manually with state.
- With HTML5 attributes (`required`, `type="email"`).
- With a library (Yup, Zod, Joi) paired with Formik or React Hook Form.

```jsx
const schema = z.object({ email: z.string().email() });
```

---

## 10. Performance Optimization

### Q1: How do you prevent unnecessary re-renders?
**Answer:**
- Use `React.memo` for pure components.
- Use `useMemo` and `useCallback` for stable references.
- Split components so only changed parts re-render.
- Use proper `key` props in lists.

### Q2: What is code splitting?
**Answer:** Breaking your bundle into smaller chunks that load on demand.

```jsx
const Page = React.lazy(() => import("./Page"));
```

### Q3: What is tree shaking?
**Answer:** A bundler feature that removes unused code from the final bundle.

### Q4: How do you lazy-load images?
**Answer:**

```html
<img loading="lazy" src="big.jpg" />
```

### Q5: How do you measure performance?
**Answer:**
- React DevTools Profiler.
- Chrome DevTools Performance tab.
- Web Vitals (LCP, FID, CLS).

### Q6: What is virtualization (windowing)?
**Answer:** Render only the visible items in a long list. Libraries: `react-window`, `react-virtualized`.

### Q7: What is a Web Worker?
**Answer:** A background thread that runs JavaScript without blocking the UI. Good for heavy computations.

---

## 11. Testing

### Q1: What is the testing pyramid?
**Answer:**
- **Unit tests** (most) — test one function/component.
- **Integration tests** — test how pieces work together.
- **E2E tests** (least) — test full user flows.

### Q2: What is Jest?
**Answer:** A JavaScript testing framework by Meta. Used for unit tests, snapshots, mocking.

```js
test("adds 1 + 2", () => {
  expect(1 + 2).toBe(3);
});
```

### Q3: What is React Testing Library?
**Answer:** A library for testing React components by simulating user interactions. Encourages testing behavior, not implementation.

```js
import { render, screen } from "@testing-library/react";
test("shows greeting", () => {
  render(<Greeting name="Alice" />);
  expect(screen.getByText(/Alice/)).toBeInTheDocument();
});
```

### Q4: What is the difference between `getBy`, `findBy`, and `queryBy`?
**Answer:**
- `getBy` — returns element or throws. Use when element should be there.
- `queryBy` — returns element or null. Use when checking absence.
- `findBy` — async, waits for element to appear.

### Q5: What is snapshot testing?
**Answer:** Saves the rendered output and compares it on future runs. Catches unintended UI changes.

```js
expect(container).toMatchSnapshot();
```

### Q6: What is mocking?
**Answer:** Replacing real modules/functions with fake ones to isolate what you're testing.

```js
jest.mock("./api");
```

### Q7: What is Cypress / Playwright?
**Answer:** End-to-end testing tools that simulate real user actions in a browser.

---

## 12. Accessibility (a11y)

### Q1: Why is accessibility important?
**Answer:** It makes your app usable by people with disabilities (screen readers, keyboard-only, etc.). It is also required by law in many countries.

### Q2: What are ARIA attributes?
**Answer:** `aria-*` attributes provide extra information to assistive technologies.
- `aria-label` — name for an element without visible text.
- `aria-hidden` — hide from screen readers.
- `aria-live` — announce updates.

```html
<button aria-label="Close">×</button>
```

### Q3: How do you make a form accessible?
**Answer:**
- Use proper `<label>` linked with `htmlFor`.
- Use semantic input types (`type="email"`).
- Add error messages with `aria-describedby`.
- Use `aria-invalid="true"` for invalid fields.

### Q4: What is the tab order?
**Answer:** The order in which focusable elements receive focus when pressing Tab. Make sure it matches the visual order.

### Q5: How do you make images accessible?
**Answer:** Always provide `alt` text describing the image. Use `alt=""` for decorative images.

```html
<img src="chart.png" alt="Sales grew 20% in Q4" />
```

### Q6: What is the difference between `role` and `<tag>`?
**Answer:** A `role` describes what an element does to assistive tech. A tag describes how it looks. Use the right tag; add `role` only when needed.

---

## 13. Build Tools & Bundlers

### Q1: What is a bundler?
**Answer:** A tool that combines many files (JS, CSS, images) into a smaller set of files the browser can load. Examples: Webpack, Vite, esbuild, Parcel.

### Q2: What is Vite?
**Answer:** A modern frontend build tool that uses native ES modules in dev for instant startup and Rollup for production.

### Q3: What is Webpack?
**Answer:** A powerful bundler that supports loaders, plugins, code splitting, hot reload. Heavier than Vite.

### Q4: What is a transpiler?
**Answer:** Converts code from one version/language to another. Example: Babel converts modern JS/JSX to old JS for older browsers.

### Q5: What is tree shaking?
**Answer:** Removing unused code from the final bundle. Works because ES modules are statically analyzable.

### Q6: What is the difference between dev and prod builds?
**Answer:**
- **Dev** — fast build, source maps, unminified, includes warnings.
- **Prod** — minified, tree-shaken, optimized, no debug info.

---

## 14. Networking & APIs

### Q1: What is `fetch`?
**Answer:** A modern browser API to make HTTP requests. Returns a Promise.

```js
const res = await fetch("/api/users");
const data = await res.json();
```

### Q2: What is Axios?
**Answer:** A popular third-party HTTP client. Easier to use than `fetch`, supports interceptors, automatic JSON, error handling.

```js
axios.get("/api/users").then(res => console.log(res.data));
```

### Q3: What are HTTP methods?
**Answer:** `GET` (read), `POST` (create), `PUT` (replace), `PATCH` (partial update), `DELETE` (remove).

### Q4: What are HTTP status codes?
**Answer:**
- `2xx` success (200 OK, 201 Created, 204 No Content)
- `3xx` redirect (301, 302, 304)
- `4xx` client error (400, 401, 403, 404)
- `5xx` server error (500, 502, 503)

### Q5: What is CORS?
**Answer:** Cross-Origin Resource Sharing. A browser security feature that blocks requests to a different domain unless the server allows it via headers.

### Q6: What is GraphQL?
**Answer:** A query language for APIs. The client asks for exactly what it needs in one request. Reduces over-fetching and under-fetching.

### Q7: What is WebSocket?
**Answer:** A protocol for **full-duplex** (two-way) real-time communication over a single TCP connection. Used for chat, live data.

### Q8: What is Server-Sent Events (SSE)?
**Answer:** A one-way stream from server to client over HTTP. Simpler than WebSockets for read-only updates.

---

## 15. Browser Storage

### Q1: Difference between `localStorage`, `sessionStorage`, and `cookies`?
**Answer:**
| Feature | localStorage | sessionStorage | Cookies |
|---------|--------------|----------------|---------|
| Size | ~5-10 MB | ~5 MB | ~4 KB |
| Expires | Never | On tab close | Configurable |
| Sent with requests | No | No | Yes |
| Scope | Origin | Tab | Domain/path |

### Q2: What is IndexedDB?
**Answer:** A low-level API for storing large amounts of structured data in the browser. Async, supports indexes, transactions.

### Q3: When to use what storage?
**Answer:**
- **Cookies** — auth tokens (with `HttpOnly`).
- **localStorage** — user preferences, theme.
- **sessionStorage** — temporary form data.
- **IndexedDB** — large data, offline support.

---

## 16. Security Basics

### Q1: What is XSS (Cross-Site Scripting)?
**Answer:** Attacker injects malicious scripts into your page. Always **sanitize user input** and avoid `dangerouslySetInnerHTML` unless you trust the source.

### Q2: What is CSRF (Cross-Site Request Forgery)?
**Answer:** Attacker tricks a logged-in user into submitting a request to your site. Mitigation: CSRF tokens, SameSite cookies.

### Q3: What is `Content Security Policy` (CSP)?
**Answer:** An HTTP header that tells the browser which sources of scripts, styles, and images are allowed. Helps prevent XSS.

### Q4: How do you store JWT tokens safely?
**Answer:** Prefer **HttpOnly cookies** so JavaScript cannot access them. Avoid localStorage for sensitive tokens.

### Q5: What is HTTPS?
**Answer:** HTTP over TLS. Encrypts traffic between client and server.

---

## 17. Web Performance Metrics

### Q1: What are Core Web Vitals?
**Answer:**
- **LCP** (Largest Contentful Paint) — loading speed. Target < 2.5s.
- **FID / INP** (First Input Delay / Interaction to Next Paint) — interactivity. Target < 200ms.
- **CLS** (Cumulative Layout Shift) — visual stability. Target < 0.1.

### Q2: How do you improve LCP?
**Answer:** Optimize images, preload key resources, use a CDN, reduce server response time.

### Q3: How do you reduce CLS?
**Answer:** Always set width/height on images, avoid inserting content above existing content, use `font-display: swap`.

### Q4: What is lazy loading?
**Answer:** Loading resources only when needed (e.g., images when they scroll into view).

### Q5: What is code splitting?
**Answer:** Breaking your JS bundle into smaller chunks so the browser loads only what is needed.

---

## 18. Coding Challenges (Frontend)

These are common live-coding questions.

### Q1: Build a counter component.
```jsx
function Counter() {
  const [count, setCount] = useState(0);
  return (
    <div>
      <button onClick={() => setCount(c => c - 1)}>-</button>
      <span>{count}</span>
      <button onClick={() => setCount(c => c + 1)}>+</button>
    </div>
  );
}
```

### Q2: Build a search input with debounce.
```jsx
function Search() {
  const [query, setQuery] = useState("");
  const [results, setResults] = useState([]);
  useEffect(() => {
    const t = setTimeout(() => {
      fetch(`/api/search?q=${query}`).then(r => r.json()).then(setResults);
    }, 300);
    return () => clearTimeout(t);
  }, [query]);
  return (
    <>
      <input value={query} onChange={e => setQuery(e.target.value)} />
      <ul>{results.map(r => <li key={r.id}>{r.name}</li>)}</ul>
    </>
  );
}
```

### Q3: Build a Todo list with add and remove.
```jsx
function Todo() {
  const [items, setItems] = useState([]);
  const [text, setText] = useState("");
  const add = () => {
    if (!text) return;
    setItems([...items, { id: Date.now(), text }]);
    setText("");
  };
  const remove = id => setItems(items.filter(i => i.id !== id));
  return (
    <>
      <input value={text} onChange={e => setText(e.target.value)} />
      <button onClick={add}>Add</button>
      <ul>{items.map(i => (
        <li key={i.id}>{i.text} <button onClick={() => remove(i.id)}>x</button></li>
      ))}</ul>
    </>
  );
}
```

### Q4: Build an infinite scroll list.
```jsx
function InfiniteList() {
  const [items, setItems] = useState([]);
  const [page, setPage] = useState(1);
  const loader = useRef(null);
  useEffect(() => {
    const obs = new IntersectionObserver(entries => {
      if (entries[0].isIntersecting) setPage(p => p + 1);
    });
    if (loader.current) obs.observe(loader.current);
    return () => obs.disconnect();
  }, []);
  useEffect(() => {
    fetch(`/api/items?page=${page}`).then(r => r.json()).then(d =>
      setItems(prev => [...prev, ...d])
    );
  }, [page]);
  return (
    <>{items.map(i => <p key={i.id}>{i.name}</p>)}
      <div ref={loader}>Loading...</div></>
  );
}
```

### Q5: Build a modal.
```jsx
function Modal({ open, onClose, children }) {
  if (!open) return null;
  return (
    <div className="backdrop" onClick={onClose}>
      <div className="modal" onClick={e => e.stopPropagation()}>
        {children}
      </div>
    </div>
  );
}
```

### Q6: Build a star rating component.
```jsx
function StarRating({ value, onChange }) {
  return [1, 2, 3, 4, 5].map(n => (
    <span key={n}
      style={{ cursor: "pointer", color: n <= value ? "gold" : "gray" }}
      onClick={() => onChange(n)}>★</span>
  ));
}
```

### Q7: Build an autocomplete input.
```jsx
function Autocomplete({ options }) {
  const [q, setQ] = useState("");
  const filtered = options.filter(o => o.toLowerCase().includes(q.toLowerCase()));
  return (
    <>
      <input value={q} onChange={e => setQ(e.target.value)} />
      <ul>{filtered.map(o => <li key={o}>{o}</li>)}</ul>
    </>
  );
}
```

### Q8: Build a tabs component.
```jsx
function Tabs({ tabs }) {
  const [active, setActive] = useState(0);
  return (
    <>
      <div>{tabs.map((t, i) => (
        <button key={i} onClick={() => setActive(i)}
          style={{ fontWeight: active === i ? "bold" : "normal" }}>{t.label}</button>
      ))}</div>
      <div>{tabs[active].content}</div>
    </>
  );
}
```

### Q9: Implement usePrevious hook.
```jsx
function usePrevious(value) {
  const ref = useRef();
  useEffect(() => { ref.current = value; }, [value]);
  return ref.current;
}
```

### Q10: Implement useDebounce hook.
```jsx
function useDebounce(value, delay) {
  const [debounced, setDebounced] = useState(value);
  useEffect(() => {
    const t = setTimeout(() => setDebounced(value), delay);
    return () => clearTimeout(t);
  }, [value, delay]);
  return debounced;
}
```

---

## 19. Behavioral & System Design for Frontend

### Q1: How do you handle disagreements with a designer?
**Answer:** Discuss user goals, share data, propose A/B test. Always tie the decision back to user impact.

### Q2: How do you prioritize when everything is "urgent"?
**Answer:** Use impact vs effort matrix. Talk to the PM. Focus on what moves the metric.

### Q3: Design an image gallery like Instagram.
**Answer:** Components, virtualization, lazy loading, optimistic UI, infinite scroll, modal on click, like animation, accessibility, responsive grid.

### Q4: Design a chat UI.
**Answer:** WebSocket for real-time, scroll-to-bottom on new message, virtualization, unread badges, typing indicators, optimistic UI, reconnection.

### Q5: Design an autocomplete dropdown.
**Answer:** Debounced input, recent searches, keyboard navigation, ARIA combobox role, virtualization for long lists, caching.

### Q6: How would you improve page load time?
**Answer:** Code splitting, lazy loading images, CDN, minimize CSS/JS, defer non-critical JS, prefetch, SSR.

### Q7: How do you handle errors in the UI?
**Answer:** Error boundaries, retry logic, fallback UI, user-friendly messages, logging to monitoring service.

---

## Quick Reference Card

| Concept | One-line definition |
|---------|---------------------|
| JSX | HTML-like syntax in JavaScript |
| Virtual DOM | In-memory copy of DOM |
| Props | Read-only inputs from parent |
| State | Component-owned mutable data |
| Hook | Function that uses React features |
| useEffect | Run side effects after render |
| useMemo | Cache a computed value |
| useCallback | Cache a function |
| useRef | Persistent mutable reference |
| Context | Share data without prop drilling |
| Redux | Predictable state container |
| Reconciliation | React's diffing algorithm |
| Suspense | Show fallback while loading |
| StrictMode | Extra dev-time checks |
| CSR vs SSR | Render in browser vs server |

---

Good luck with your frontend interviews!
