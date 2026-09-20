# Computer Networks — Comprehensive Interview Guide

> **Every line of HTTP, every TCP handshake, every DNS lookup** — interviewers expect you to reason about what happens between the browser and the server. This guide gives you the **theory, comparison tables, diagrams, and 50+ Q&A** to handle any networking round.

---

## Table of Contents

1. [OSI 7-Layer & TCP/IP 4-Layer Models](#1-osi-7-layer--tcpip-4-layer-models)
2. [HTTP & HTTPS — Methods, Headers, Status Codes](#2-http--https--methods-headers-status-codes)
3. [TCP vs UDP — Which to Pick When](#3-tcp-vs-udp--which-to-pick-when)
4. [TCP Handshake, Termination, Flow & Congestion Control](#4-tcp-handshake-termination-flow--congestion-control)
5. [DNS — How Name Resolution Works](#5-dns--how-name-resolution-works)
6. [Load Balancing — L4 vs L7, Algorithms, Health Checks](#6-load-balancing--l4-vs-l7-algorithms-health-checks)
7. [CDNs, Proxies, Reverse Proxies](#7-cdns-proxies-reverse-proxies)
8. [Caching Strategies & Cache Invalidation](#8-caching-strategies--cache-invalidation)
9. [REST vs GraphQL vs gRPC vs WebSockets](#9-rest-vs-graphql-vs-grpc-vs-websockets)
10. [Cookies, Sessions, JWT, OAuth, OIDC](#10-cookies-sessions-jwt-oauth-oidc)
11. [TLS / SSL — How HTTPS Secures the Wire](#11-tls--ssl--how-https-secures-the-wire)
12. [Security Essentials — CORS, CSRF, XSS, SQLi, MITM](#12-security-essentials--cors-csrf-xss-sqli-mitm)
13. [Network Performance & Troubleshooting](#13-network-performance--troubleshooting)
14. [Quick Reference Card](#14-quick-reference-card)
15. [Top Interview Q&A (45 entries)](#15-top-interview-qa-45-entries)

---

## 1. OSI 7-Layer & TCP/IP 4-Layer Models

### OSI Reference Model

```
Layer 7 — Application   → HTTP, FTP, SMTP, DNS, SSH, gRPC
Layer 6 — Presentation  → TLS/SSL, MIME, encryption, data format
Layer 5 — Session       → NetBIOS, RPC, SIP (manages sessions/dialogs)
Layer 4 — Transport     → TCP, UDP, QUIC (segments / datagrams)
Layer 3 — Network       → IP, ICMP, ARP, routers (packets)
Layer 2 — Data Link     → Ethernet, Wi-Fi (802.11), MAC, switches (frames)
Layer 1 — Physical      → Cables, fiber, radio, hubs (bits)
```

**Mnemonic (top-down):** "**A**ll **P**eople **S**eem **T**o **N**eed **D**ata **P**rocessing"

### TCP/IP 4-Layer Model (Real-World)

```
Application    → HTTP, DNS, SMTP, TLS (maps to OSI 5–7)
Transport      → TCP, UDP, QUIC (OSI 4)
Internet       → IPv4/IPv6, ICMP (OSI 3)
Link / Network → Ethernet, Wi-Fi (OSI 1–2)
```

### Encapsulation

Data flows down the stack on the sender, up on the receiver. Each layer adds its own header (and sometimes trailer):

```
+--------------------+
| HTTP body          |  ← Application data
+--------------------+
| HTTP header        |
+--------------------+
| TLS record         |  ← (if HTTPS)
+--------------------+
| TCP header         |  ← source/dest port, seq, ack, flags
+--------------------+
| IP header          |  ← source/dest IP, TTL
+--------------------+
| Ethernet header    |  ← MAC addresses, EtherType
+--------------------+
| Bits on the wire   |
+--------------------+
```

### PDU Names

| Layer | PDU |
|-------|-----|
| Application | Message / Data |
| Transport | Segment (TCP) / Datagram (UDP) |
| Network | Packet |
| Data Link | Frame |
| Physical | Bits |

---

## 2. HTTP & HTTPS — Methods, Headers, Status Codes

### HTTP Methods (Verbs)

| Method | Purpose | Idempotent? | Safe? |
|--------|---------|-------------|-------|
| **GET** | Read a resource | Yes | Yes |
| **HEAD** | Same as GET without body | Yes | Yes |
| **POST** | Create / submit | No | No |
| **PUT** | Replace whole resource | Yes | No |
| **PATCH** | Partial update | No | No |
| **DELETE** | Remove a resource | Yes | No |
| **OPTIONS** | Discover allowed methods | Yes | Yes |

> **Idempotent** = same result no matter how many times called. **Safe** = doesn't modify server state.

### Status Codes (Cheat Sheet)

| Range | Meaning | Common Codes |
|-------|---------|--------------|
| **1xx** Informational | Continue, processing | 100 Continue |
| **2xx** Success | It worked | 200 OK, 201 Created, 202 Accepted, 204 No Content, 206 Partial Content |
| **3xx** Redirect | Go elsewhere | 301 Moved Permanently, 302 Found, 303 See Other, 304 Not Modified, 307 Temporary, 308 Permanent |
| **4xx** Client Error | You messed up | 400 Bad Request, 401 Unauthorized, 403 Forbidden, 404 Not Found, 405 Method Not Allowed, 409 Conflict, 410 Gone, 415 Unsupported Media Type, 422 Unprocessable, 429 Too Many Requests |
| **5xx** Server Error | We messed up | 500 Internal Server Error, 501 Not Implemented, 502 Bad Gateway, 503 Service Unavailable, 504 Gateway Timeout |

### Essential Headers

| Header | Purpose |
|--------|---------|
| `Host` | Required in HTTP/1.1; tells server which virtual host |
| `User-Agent` | Client identification |
| `Accept` / `Content-Type` | Negotiated media type (e.g., `application/json`) |
| `Authorization` | `Bearer ...` or `Basic ...` |
| `Cookie` | Send cookies back |
| `Cache-Control` | Caching directives (see §8) |
| `ETag` / `If-None-Match` | Conditional GET |
| `Content-Length` | Body size |
| `Location` | Used with 3xx |
| `X-Forwarded-For` / `X-Real-IP` | Original client IP behind proxies |
| `Strict-Transport-Security` | Force HTTPS |
| `X-Content-Type-Options: nosniff` | Prevent MIME sniffing |

### HTTPS = HTTP + TLS

- All HTTP semantics, but the connection is encrypted and authenticated.
- Default port **443** (vs HTTP's **80**).
- TLS provides **confidentiality**, **integrity**, and **server authentication**.

---

## 3. TCP vs UDP — Which to Pick When

| Aspect | TCP | UDP |
|--------|-----|-----|
| Connection | Connection-oriented (handshake) | Connectionless |
| Reliability | ACKs, retransmission, sequencing | Best-effort, may drop / duplicate |
| Ordering | In-order delivery | No ordering |
| Flow control | Sliding window | None |
| Congestion control | Yes (Cubic, BBR) | None |
| Header size | 20–60 bytes | 8 bytes |
| Speed | Slower (more overhead) | Faster, lower latency |
| Examples | HTTP, HTTPS, SSH, SMTP, FTP | DNS, VoIP, video streaming, gaming, QUIC |

**Quick decision rule:** Use TCP when correctness matters (transfers, transactions). Use UDP when **latency** matters and you can tolerate loss (live video, multiplayer game state). Modern stacks use **QUIC** (UDP + reliability + encryption) to get the best of both.

---

## 4. TCP Handshake, Termination, Flow & Congestion Control

### 3-Way Handshake (Open)

```
       Client                 Server
        |-------- SYN (seq=x) -------->|
        |<----- SYN+ACK (seq=y, ack=x+1) ---|
        |-------- ACK (ack=y+1) ------->|
        |                                 |
        |     Connection established      |
```

**Why 3 steps?** Synchronize sequence numbers on both sides and prevent old duplicate SYNs from creating spurious connections.

### 4-Way Termination (Close)

```
       Client                 Server
        |-------- FIN -------->|
        |<------- ACK ---------|
        |<------- FIN ---------|
        |-------- ACK -------->|
        |                                 |
        |  wait ~2*MSL (TIME_WAIT)        |
```

- **TIME_WAIT** (2 * MSL ≈ 60s by default): ensure last ACK reaches the other side and clean up old packets on the wire.

### Flow Control (Sliding Window)

Receiver advertises its available buffer (`rwnd`) in every ACK. Sender won't have more than `rwnd` unacknowledged bytes outstanding — prevents the receiver from being overwhelmed.

### Congestion Control

| Phase | Behavior |
|-------|----------|
| **Slow Start** | Exponential growth of `cwnd` until packet loss or threshold |
| **Congestion Avoidance** | Linear growth of `cwnd` |
| **Fast Retransmit** | After 3 duplicate ACKs, retransmit immediately |
| **Fast Recovery** | Cut `cwnd` in half instead of going to 1 |
| **Cubic / BBR** | Modern algorithms — better performance on high-bandwidth paths |

### MSS, MTU, PMTUD

- **MSS:** Maximum Segment Size (~1460 bytes for Ethernet).
- **MTU:** Maximum Transmission Unit — physical layer limit (Ethernet 1500).
- **PMTUD:** Path MTU Discovery — sender discovers the smallest MTU along the path.

### TCP_NODELAY & Nagle's Algorithm

Nagle coalesces small writes for efficiency. Disable (`TCP_NODELAY = 1`) for low-latency protocols like game networking or RPC.

---

## 5. DNS — How Name Resolution Works

### Hierarchy

```
       . (root)
        |
     .com  .org  .io  .in  ...
        |
   google.com  github.com
        |
  mail.google.com  www.google.com
```

### Resolution Flow

1. Browser checks its own cache.
2. OS resolver checks (`/etc/hosts`, OS cache).
3. Query **recursive resolver** (ISP, 8.8.8.8, 1.1.1.1).
4. Resolver queries **root servers** → returns TLD server (`.com`).
5. Resolver queries **TLD server** → returns authoritative NS for `google.com`.
6. Resolver queries **authoritative** → returns the A/AAAA record.
7. Resolver caches (TTL).

### Record Types

| Type | Purpose | Example |
|------|---------|---------|
| **A** | IPv4 address | `google.com → 142.250.x.x` |
| **AAAA** | IPv6 address | `google.com → 2607:f8b0:...` |
| **CNAME** | Canonical name (alias) | `www.example.com → example.com` |
| **MX** | Mail exchange | `gmail.com → gmail-smtp-in.l.google.com` |
| **NS** | Authoritative name server | `google.com → ns1.google.com` |
| **TXT** | Free-form text (SPF, DKIM, verification) | `"v=spf1 include:_spf.google.com ~all"` |
| **SOA** | Start of authority (zone metadata) | — |
| **SRV** | Service locator | `_sip._tcp.example.com` |
| **PTR** | Reverse (IP → name) | used in reverse DNS |

### Why DNS Uses UDP Primarily

Small query/response, low overhead, fast. Falls back to TCP for large responses (DNSSEC, AXFR zone transfers). Modern DNS also runs over **HTTPS** (DoH) and **TLS** (DoT) for privacy.

---

## 6. Load Balancing — L4 vs L7, Algorithms, Health Checks

### L4 (Transport Layer)

- Routes by IP + port.
- Doesn't look at payload.
- **Faster** (less CPU), used for raw TCP/UDP traffic.
- Examples: AWS NLB, IPVS, HAProxy (in TCP mode).

### L7 (Application Layer)

- Routes by URL, headers, cookies, body.
- **Smarter**: `/api` → backend, `/img` → CDN, A/B test by `Cookie`.
- Adds CPU cost and slight latency.
- Examples: AWS ALB, NGINX, Envoy, HAProxy (in HTTP mode).

### Load Balancing Algorithms

| Algorithm | Idea | Best For |
|-----------|------|----------|
| **Round Robin** | Take turns | Equal servers, similar requests |
| **Weighted RR** | Round robin with weights | Heterogeneous capacity |
| **Least Connections** | Send to least-busy | Long-lived connections |
| **Least Response Time** | Combine latency + load | User-facing latency-sensitive |
| **IP Hash** | Hash source IP | Session stickiness (basic) |
| **Consistent Hash** | Hash key → ring | Caches, sharding |
| **Random / Power of 2** | Random pick 2, pick less loaded | Cheap, surprisingly good |
| **EWMA** | Exponentially weighted moving average | Modern LBs (Envoy) |

### Health Checks

- **Active:** LB periodically probes (HTTP `/healthz`, TCP `SYN`).
- **Passive:** LB watches real traffic; back off if errors spike.

### Common LB Features

- TLS termination (offload decryption to LB).
- Connection draining during deploys.
- Rate limiting per route.
- Sticky sessions (cookie-based affinity).
- Auto-scaling groups behind LB for elasticity.

---

## 7. CDNs, Proxies, Reverse Proxies

### CDN (Content Delivery Network)

- Cache static content at **edge servers** close to users.
- Reduce latency, offload origin, absorb DDoS.
- Examples: Cloudflare, Akamai, Fastly, CloudFront, Cloudflare R2, Bunny CDN.

### Forward Proxy

- Sits **in front of clients**.
- Used for anonymity, content filtering, corporate egress.
- Examples: Squid, corporate proxies.

### Reverse Proxy

- Sits **in front of servers**.
- Used for load balancing, TLS termination, caching, WAF, routing.
- Examples: NGINX, HAProxy, Envoy, Traefik, Caddy.

### Side-by-Side

| | Forward Proxy | Reverse Proxy |
|---|---|---|
| **Sits in front of** | Clients | Servers |
| **Visibility** | Clients know about it | Clients see proxy as the origin |
| **Typical use** | Anonymity, filtering | LB, caching, TLS, WAF |

---

## 8. Caching Strategies & Cache Invalidation

### Where Caching Happens

- **Browser cache**
- **CDN edge**
- **Reverse proxy cache** (NGINX)
- **Application cache** (in-memory, e.g., Redis, Caffeine)
- **Database query cache**
- **OS page cache** (kernel-managed, disk I/O)

### HTTP Caching (Cache-Control)

```
Cache-Control: public, max-age=31536000, immutable   # static asset, 1 year
Cache-Control: private, no-cache                     # user's personal page
Cache-Control: no-store                              # sensitive, never cache
Cache-Control: public, s-maxage=600                  # shared caches can hold 10 min
```

- `ETag` + `If-None-Match` for **conditional GET** (304 Not Modified).
- `Last-Modified` + `If-Modified-Since` for time-based.

### Application Cache Patterns

| Pattern | Idea | Trade-off |
|---------|------|-----------|
| **Cache-aside (lazy)** | App reads cache; on miss, reads DB and populates | Simple; risk of stale data |
| **Read-through** | Cache itself reads DB on miss | Cache library does work |
| **Write-through** | Write goes to cache **and** DB synchronously | Slow writes, fresh data |
| **Write-back (write-behind)** | Write to cache, async flush to DB | Fast writes, risk of loss |
| **Write-around** | Write goes to DB only; cache updated on next miss | Good for write-heavy cold data |

### Cache Invalidation

> *"There are only two hard things in Computer Science: cache invalidation and naming things."* — Phil Karlton

Strategies:

- **TTL-based** — expire after N seconds.
- **Event-driven** — invalidate when source-of-truth changes (pub/sub).
- **Versioned keys** — `user:42:v3` — change `v` on update.
- **Cache stampede protection** — single-flight, request coalescing, lock.

---

## 9. REST vs GraphQL vs gRPC vs WebSockets

### REST

- Resource-oriented, stateless, uses HTTP methods.
- Mature, cacheable, simple.
- Pain points: over-fetching, under-fetching, multiple round-trips.

```http
GET /users/42
GET /users/42/orders
```

### GraphQL

- Single endpoint (`POST /graphql`).
- Client asks for **exactly** the fields it needs.
- Schema-first, strongly typed.
- Pain points: caching is harder, N+1, complex authorization.

```graphql
query {
  user(id: 42) {
    name
    orders(last: 5) { id, total }
  }
}
```

### gRPC

- **HTTP/2 + Protobuf** binary protocol.
- Strongly typed `.proto` contracts.
- Bidirectional streaming, low overhead.
- Pain points: not human-readable, browser support requires `grpc-web` proxy.

```proto
service UserService {
  rpc GetUser(UserId) returns (User);
  rpc StreamUsers(StreamRequest) returns (stream User);
}
message User { int64 id = 1; string name = 2; }
```

### WebSockets

- Full-duplex persistent connection over a single TCP socket.
- Starts as an HTTP upgrade handshake.
- Use for chat, multiplayer games, live trading, collaborative editing.

```http
GET /chat HTTP/1.1
Upgrade: websocket
Connection: Upgrade
Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==
Sec-WebSocket-Version: 13
```

### Comparison

| Aspect | REST | GraphQL | gRPC | WebSockets |
|--------|------|---------|------|------------|
| Style | Resource | Query | RPC | Persistent stream |
| Schema | Implicit | Strong | Strong (proto) | N/A |
| Transport | HTTP/JSON | HTTP | HTTP/2 + protobuf | TCP (HTTP upgrade) |
| Caching | Easy | Hard | Per-call | Manual |
| Use case | Public APIs | Flexible UIs | Microservices | Real-time |

### SSE (Server-Sent Events)

- Server → client only streaming over HTTP.
- Simpler than WebSockets when bidirectional isn't needed.
- Auto-reconnect built-in.

---

## 10. Cookies, Sessions, JWT, OAuth, OIDC

### Cookies

- Small data stored in the browser, sent with every HTTP request.
- **Attributes:**
  - `HttpOnly` — JS can't read it (mitigates XSS).
  - `Secure` — only over HTTPS.
  - `SameSite=Strict|Lax|None` — mitigates CSRF.
  - `Domain`, `Path`, `Max-Age`, `Expires`.

### Sessions

- Server keeps session state (e.g., in Redis).
- Client carries only an **opaque session ID** in a cookie.
- Revocable immediately.

### JWT (JSON Web Token)

```
header.payload.signature  →  eyJhbGciOiJIUzI1NiJ9.eyJzdWIiOiI0MiJ9.abc...
```

- **Stateless**: server doesn't need to look anything up to verify.
- **Self-contained**: claims are in the token.
- Signed with HS256 (shared secret) or RS256 (asymmetric).
- Hard to revoke; typically use short-lived access tokens + refresh tokens.

```json
// Payload
{
  "sub": "user-42",
  "role": "admin",
  "exp": 1735689600,
  "iat": 1735603200
}
```

### OAuth 2.0

- **Authorization framework** — delegate access without sharing credentials.
- Roles: Resource Owner, Client, Authorization Server, Resource Server.
- Grants: Authorization Code (with PKCE for SPAs), Client Credentials, Password (deprecated), Refresh Token.
- **OIDC** (OpenID Connect) layers identity (ID token) on top of OAuth.

### When to Use What

| Need | Use |
|------|-----|
| Internal microservice auth | mTLS or service JWT |
| SPA login | OAuth 2.0 + PKCE + OIDC |
| Server-to-server | Client Credentials |
| Simple monolith sessions | Server-side sessions + cookies |
| API tokens for developers | API keys or PAT |

---

## 11. TLS / SSL — How HTTPS Secures the Wire

### TLS 1.3 Handshake (Modern)

```
Client                                  Server
  |------- ClientHello (supported ciphers, key share) ------>|
  |<------ ServerHello + EncryptedExtensions + Certificate + CertificateVerify + Finished ------|
  |------- Finished + (early data, optional) --------------->|
  |                                                         |
  |     Encrypted application data flows in both ways      |
```

- **0-RTT** option: client can send data on the first flight (with replay caveats).
- Forward-secret by default (uses ephemeral Diffie-Hellman).
- Removed legacy ciphers (RC4, MD5, SHA-1).

### Certificates

- **X.509** certificate binds a public key to a domain name.
- Signed by a **Certificate Authority (CA)** the client trusts.
- **Let's Encrypt** issues free DV certs via ACME protocol.

### Cipher Suites

A cipher suite is a bundle: `key exchange + bulk encryption + MAC`.

```
TLS_AES_128_GCM_SHA256
 │     │       │      └─ MAC / PRF hash
 │     │       └─ Encryption: AES 128 GCM (AEAD)
 │     └─ Mode
 └─ Protocol
```

AEAD = Authenticated Encryption with Associated Data (encrypts + authenticates in one step).

---

## 12. Security Essentials — CORS, CSRF, XSS, SQLi, MITM

### CORS (Cross-Origin Resource Sharing)

- Browser blocks cross-origin reads by default.
- Server opts in via headers:
  - `Access-Control-Allow-Origin: https://app.example.com` (or `*` for public)
  - `Access-Control-Allow-Methods: GET, POST, OPTIONS`
  - `Access-Control-Allow-Headers: Content-Type, Authorization`
  - `Access-Control-Allow-Credentials: true` (only with explicit origin, not `*`)
- Preflight `OPTIONS` checks complex requests.

### CSRF (Cross-Site Request Forgery)

- Attacker tricks a logged-in user's browser into making a state-changing request.
- Mitigations: `SameSite=Lax|Strict` cookies, CSRF tokens, double-submit cookies, `Origin` header checks.

### XSS (Cross-Site Scripting)

- Attacker injects JS into a page that runs in the victim's browser.
- **Stored XSS** — payload in DB.
- **Reflected XSS** — payload in URL/query.
- **DOM-based XSS** — JS reads from untrusted DOM and writes to sink.
- Mitigations: escape output, `Content-Security-Policy`, sanitize HTML (DOMPurify), `HttpOnly` cookies.

### SQL Injection

- User input concatenated into SQL string.
- Mitigations: **parameterized queries / prepared statements**, ORMs, input validation, principle of least privilege for DB user.

```sql
-- Bad
"SELECT * FROM users WHERE name = '" + name + "'"
-- Good (parameterized)
"SELECT * FROM users WHERE name = ?"
```

### MITM (Man-in-the-Middle)

- Attacker intercepts traffic between client and server.
- Mitigations: HTTPS / TLS, HSTS, certificate pinning (mobile apps), DNSSEC.

### Common Headers for Security

| Header | Purpose |
|--------|---------|
| `Strict-Transport-Security` | Force HTTPS |
| `Content-Security-Policy` | Limit what JS/HTML can run |
| `X-Frame-Options: DENY` | Prevent clickjacking |
| `X-Content-Type-Options: nosniff` | Stop MIME sniffing |
| `Referrer-Policy` | Control referrer leakage |
| `Permissions-Policy` | Restrict browser APIs |

---

## 13. Network Performance & Troubleshooting

### Latency, Throughput, Bandwidth

- **Bandwidth:** maximum data rate (Mbps).
- **Throughput:** actual achieved rate.
- **Latency:** time for one packet to travel.
- **RTT (Round Trip Time):** 2 × one-way latency.
- **Bandwidth-delay product:** how much data fits "in flight" — drives TCP window sizing.

### Common Bottlenecks

- **DNS lookup** — cache, use shorter TTLs carefully.
- **TLS handshake** — session resumption, TLS 1.3 0-RTT.
- **TCP slow start** — keep-alive, larger initial cwnd.
- **Server processing** — caching, async I/O, profiling.
- **Payload size** — gzip/brotli compression, code splitting, lazy loading.

### Tools

| Tool | Purpose |
|------|---------|
| `ping` | Reachability + RTT |
| `traceroute` / `mtr` | Path discovery, hop timing |
| `curl -v` | Headers, redirects, TLS details |
| `dig` / `nslookup` | DNS queries |
| `tcpdump` / `Wireshark` | Packet capture |
| `ab` / `wrk` / `k6` | HTTP load testing |
| `iperf3` | Bandwidth testing |
| `netstat` / `ss` / `lsof` | Open ports, sockets, FDs |
| `httping` | HTTP latency |

### HTTP/2 & HTTP/3 Wins

- **HTTP/2:** multiplexed streams (no head-of-line), HPACK header compression, server push (now deprecated).
- **HTTP/3:** QUIC over UDP, 0-RTT, no TCP/TLS handshake, better on lossy networks (mobile).

### TCP Optimizations (Linux)

- `tcp_tw_reuse` — reuse TIME_WAIT sockets for outgoing connections.
- `tcp_fastopen` — send data in SYN.
- `net.ipv4.tcp_window_scaling = 1` — larger windows for high BDP.
- `tcp_congestion_control = bbr` — modern algorithm.

---

## 14. Quick Reference Card

| Concept | One-Liner |
|---------|----------|
| OSI | 7 layers: App, Pres, Sess, Trans, Net, Data, Phys |
| TCP/IP | 4 layers: App, Trans, Internet, Link |
| HTTP | Stateless request/response over TCP |
| HTTPS | HTTP + TLS (port 443) |
| TCP | Reliable, ordered, byte-stream |
| UDP | Best-effort, datagram, low overhead |
| 3-way handshake | SYN → SYN+ACK → ACK |
| TIME_WAIT | 2×MSL after last ACK to clean up old packets |
| DNS | Name → IP; UDP 53 (mostly) |
| L4 LB | Routes by IP/port; faster |
| L7 LB | Routes by URL/headers/cookies; smarter |
| CDN | Edge cache closer to users |
| REST | Resource-oriented, HTTP methods |
| GraphQL | Single endpoint, client-defined shape |
| gRPC | HTTP/2 + protobuf; strong contracts |
| WebSocket | Full-duplex persistent |
| CORS | Browser cross-origin policy + server opt-in headers |
| CSRF | Use SameSite cookies + tokens |
| XSS | Escape output + CSP |
| SQLi | Use parameterized queries |
| TLS 1.3 | 1-RTT handshake, forward secrecy by default |

---

## 15. Top Interview Q&A (45 entries)

**Q1: HTTPS vs HTTP — what does TLS add?**
Encryption (confidentiality), integrity (no tampering), and server authentication via certificates.

**Q2: Why is TCP "reliable"?**
ACKs, retransmission on timeout, sequence numbers for ordering, flow control (sliding window), congestion control (slow start, AIMD).

**Q3: Why does DNS mostly use UDP?**
Small query/response, low overhead, fast. Falls back to TCP for large responses (DNSSEC, AXFR). Also over HTTPS (DoH) / TLS (DoT) for privacy.

**Q4: Switch vs router?**
**Switch:** Layer 2, forwards by MAC address within a LAN.
**Router:** Layer 3, forwards by IP across networks.

**Q5: What is a subnet mask?**
Specifies which bits of an IP are network vs host. `/24` means first 24 bits are network, 8 bits are host (256 addresses).

**Q6: What is CORS?**
Cross-Origin Resource Sharing — a browser mechanism using HTTP headers (`Access-Control-Allow-Origin`, etc.) to allow cross-domain requests. Servers opt in; the browser enforces.

**Q7: Forward vs reverse proxy?**
**Forward:** client → proxy → internet (anonymity, filtering).
**Reverse:** client → proxy → servers (LB, caching, TLS termination, WAF).

**Q8: What is the TIME_WAIT state?**
After sending final ACK in 4-way close, the side stays in TIME_WAIT for ~2×MSL (~60s) so any retransmitted FIN/ACK on the wire can be properly handled, and to avoid old segments being mistaken for new ones.

**Q9: Difference between 401 and 403?**
**401 Unauthorized** = no/invalid credentials; you should authenticate.
**403 Forbidden** = authenticated but not allowed; you cannot.

**Q10: PUT vs PATCH?**
PUT replaces the entire resource. PATCH applies a partial change.

**Q11: Idempotency — why does it matter?**
If a network glitch causes a retry, an idempotent operation yields the same result — safe to retry. POST is NOT idempotent; PUT/DELETE/GET are.

**Q12: What is a CDN and how does it work?**
Network of edge servers distributed globally that cache static content close to users. First request may hit origin; subsequent requests hit nearest edge.

**Q13: When would you use UDP over TCP?**
Real-time audio/video, gaming, DNS — when low latency matters more than reliability. With QUIC you can get both.

**Q14: What is flow control?**
Prevents sender from overwhelming receiver. Receiver advertises its available buffer (`rwnd`) in every ACK.

**Q15: What is congestion control?**
Prevents sender from overwhelming the network. TCP uses slow start + AIMD (additive increase, multiplicative decrease). Modern algorithms: Cubic, BBR.

**Q16: HTTP/2 vs HTTP/1.1?**
Multiplexed streams (no head-of-line blocking per request), header compression (HPACK), server push (deprecated), binary framing. Major perf win for many small requests.

**Q17: HTTP/3 vs HTTP/2?**
HTTP/3 uses QUIC (UDP-based), removes TCP head-of-line blocking, 0-RTT handshake, better loss recovery.

**Q18: What is QUIC?**
UDP-based transport with built-in TLS 1.3, multiplexing, and stream-level reliability. Powers HTTP/3.

**Q19: Why is GET cacheable but POST typically not?**
GET is safe and idempotent by spec, so caches can serve without changing server state. POST is a write — caching it could double-submit or hide failures.

**Q20: Explain CSRF and how to prevent it.**
Attacker tricks browser into making authenticated request to your site. Mitigations: `SameSite=Lax/Strict` cookies, CSRF tokens, double-submit cookie pattern, `Origin`/`Referer` checks.

**Q21: What is SameSite cookie attribute?**
`Strict` — never sent on cross-site requests.
`Lax` — sent on top-level navigation (safe for most links).
`None` — always sent (requires `Secure`).

**Q22: What is XSS and how do you prevent it?**
Injection of untrusted JS into a trusted page. Mitigations: escape output (HTML/attribute/JS/URL context), `Content-Security-Policy`, `HttpOnly` cookies, sanitize HTML on input.

**Q23: What is SQL injection and the fix?**
Concatenating user input into SQL strings. Fix: **parameterized queries / prepared statements** + ORM + least privilege.

**Q24: How does TLS 1.3 differ from 1.2?**
1-RTT handshake (often 0-RTT), mandatory forward secrecy, AEAD-only, removed legacy ciphers (RC4, MD5, SHA-1, CBC modes).

**Q25: What is HSTS?**
`Strict-Transport-Security` response header — tells browsers to always use HTTPS for that domain for `max-age` seconds. Mitigates SSL-strip attacks.

**Q26: What is mTLS?**
Mutual TLS — both client and server present certificates. Common in service-to-service auth in zero-trust networks.

**Q27: What's the difference between authentication and authorization?**
**Authentication (AuthN):** who are you?
**Authorization (AuthZ):** what are you allowed to do?

**Q28: JWT vs session cookie — when to use which?**
JWT: stateless, works for microservices and cross-domain SSO. Hard to revoke; usually short-lived + refresh tokens.
Session cookie: server-side state, instantly revocable, simpler for monoliths.

**Q29: What is OAuth 2.0 — and what it isn't?**
OAuth 2.0 is an **authorization** framework for delegated access. **Not an authentication protocol.** OpenID Connect (OIDC) layers authentication (ID token) on top.

**Q30: What is a sticky session?**
LB forwards all requests of a user to the same backend (cookie-based affinity). Use when state is stored locally; otherwise prefer stateless servers.

**Q31: How does HTTP polling differ from WebSockets?**
**Polling:** client repeatedly asks "any updates?" — wasteful when nothing changes.
**WebSockets:** persistent duplex connection; server pushes when there's data.

**Q32: Long polling vs SSE vs WebSocket?**
**Long polling:** request stays open until server has data; client reconnects.
**SSE:** server → client stream over HTTP, auto-reconnect.
**WebSocket:** full-duplex bidirectional.

**Q33: What is a SYN flood attack?**
Attacker sends many SYNs without completing handshake, exhausting server's half-open connections. Mitigations: SYN cookies, rate limiting, firewalls.

**Q34: What's a reverse proxy used for?**
TLS termination, caching, load balancing, gzip, security (WAF), single ingress point for many services.

**Q35: How does a CDN handle cache invalidation?**
TTL-based expiry, purge URLs/tags, origin shield (CDN acts as proxy cache), versioned URLs (`v=hash`), surrogate keys for tag-based purges.

**Q36: When would you use gRPC?**
Internal microservices needing low latency, strong contracts, streaming. Public-facing APIs usually stick with REST/GraphQL for tooling/browser support.

**Q37: What is HTTP keep-alive?**
Single TCP connection reused for multiple requests. HTTP/1.1 default. Eliminates handshake overhead.

**Q38: What is a CDN POP?**
Point of Presence — a physical edge location where the CDN serves cached content. Lower POP distance = lower RTT.

**Q39: What is the bandwidth-delay product?**
BDP = bandwidth × RTT. The maximum amount of data "in flight" at once. Drives TCP window size for full link utilization.

**Q40: What is a connection pool?**
A pre-warmed set of TCP connections (often to a DB) that are reused — avoids handshake cost. Tune max active, idle timeout, max lifetime.

**Q41: What is a packet?**
A unit of data at Layer 3 (network), consisting of IP header + payload (segment). Routers forward packets.

**Q42: Difference between public and private IP?**
**Public:** globally routable, assigned by ISP, unique on internet.
**Private:** used inside a LAN (RFC 1918: `10/8`, `172.16/12`, `192.168/16`), not routed globally.

**Q43: NAT — what is it and why?**
Network Address Translation — maps many private IPs to one (or few) public IPs. Conserves IPv4 space, hides internal topology.

**Q44: What is BGP?**
Border Gateway Protocol — the protocol that exchanges routing info between autonomous systems on the internet. Path-vector, policy-based.

**Q45: How would you debug "site is slow"?**
1. `curl -w` to see total time, DNS, connect, TLS, TTFB.
2. `dig` for DNS, check latency from multiple regions.
3. `traceroute` for packet loss / path issues.
4. Server metrics (CPU, GC pauses, DB slow log).
5. CDN cache hit ratio.
6. Reproduce with realistic load test.

---

> **Next:** continue with `04-oop-concepts/README.md` to understand the OOP building blocks that ride on top of all this networking and data plumbing.
