# Computer Networks — Interview Notes

## 1. OSI 7-Layer Model

```
Layer 7: Application → HTTP, FTP, SMTP, DNS, SSH
Layer 6: Presentation → TLS/SSL, JPEG, MPEG (data format, encryption)
Layer 5: Session → NetBIOS, RPC (manages sessions)
Layer 4: Transport → TCP, UDP (segments)
Layer 3: Network → IP, ICMP, routers (packets)
Layer 2: Data Link → Ethernet, MAC, switches (frames)
Layer 1: Physical → Cables, hubs, bits
```

### TCP/IP 4-Layer Model
```
Application (HTTP, DNS, SMTP)
Transport (TCP, UDP)
Internet (IP, ICMP)
Link (Ethernet, Wi-Fi)
```

---

## 2. HTTP & HTTPS

### HTTP Methods
- `GET` — read (idempotent)
- `POST` — create
- `PUT` — replace (idempotent)
- `PATCH` — partial update
- `DELETE` — remove

### HTTP Status Codes
- `2xx` Success: 200 OK, 201 Created, 204 No Content
- `3xx` Redirect: 301 Moved Permanently, 302 Found, 304 Not Modified
- `4xx` Client Error: 400 Bad Request, 401 Unauthorized, 403 Forbidden, 404 Not Found, 429 Too Many Requests
- `5xx` Server Error: 500 Internal Server Error, 502 Bad Gateway, 503 Service Unavailable, 504 Gateway Timeout

### HTTPS = HTTP + TLS
- TLS handshake: client & server agree on cipher, exchange keys via asymmetric crypto.
- Provides: confidentiality, integrity, authentication.

---

## 3. TCP vs UDP

| Feature | TCP | UDP |
|---------|-----|-----|
| Connection | Connection-oriented | Connectionless |
| Reliability | ACK, retransmission | Best-effort |
| Order | In-order delivery | No ordering |
| Flow control | Yes (sliding window) | No |
| Congestion control | Yes | No |
| Speed | Slower | Faster |
| Use cases | HTTP, SSH, file transfer | DNS, video, gaming, VoIP |

---

## 4. TCP 3-Way Handshake

```
Client Server
 |---- SYN --------->| (client picks seq=x)
 |<--- SYN+ACK ------| (server: seq=y, ack=x+1)
 |---- ACK --------->| (client: ack=y+1)
 | |
 | Connection open |
```

### Why 3-way?
- To synchronize sequence numbers on both sides.
- To prevent old duplicate SYN segments from creating spurious connections.

### TCP Termination (4-way FIN)
```
Client Server
 |---- FIN --------->|
 |<--- ACK ----------|
 |<--- FIN ----------|
 |---- ACK --------->|
```

---

## 5. DNS

- Translates domain name → IP address.
- **Hierarchy:** Root → TLD (.com) → Authoritative (google.com).
- **Lookup types:**
 - Recursive: server resolves fully.
 - Iterative: server returns referrals.
- **Record types:** A, AAAA, CNAME, MX, NS, TXT, SOA.

---

## 6. Load Balancers

### L4 (Transport Layer)
- Routes based on IP + port.
- Faster, no payload inspection.

### L7 (Application Layer)
- Routes based on URL, headers, cookies.
- Smarter (e.g., /api → backend, /img → CDN).

### Algorithms
- Round Robin
- Least Connections
- Weighted Round Robin
- IP Hash (sticky sessions)
- Random

---

## 7. CDNs

Content Delivery Networks:
- Cache static content at edge servers close to users.
- Reduce latency, offload origin.
- Examples: Cloudflare, Akamai, CloudFront.

---

## 8. Caching Strategies

- **Cache-aside** (lazy loading): app reads cache, on miss reads DB and populates cache.
- **Write-through:** writes go to cache and DB synchronously.
- **Write-back:** writes go to cache, asynchronously to DB.
- **Read-through:** cache acts as the main reader; populates from DB on miss.

---

## 9. Cookies, Sessions, JWT

### Cookies
- Small data stored in browser.
- Sent with every HTTP request.
- `HttpOnly` (not accessible via JS), `Secure` (HTTPS only), `SameSite`.

### Sessions
- Server-side state (e.g., session ID in cookie).
- Need sticky sessions in distributed systems.

### JWT (JSON Web Token)
- Self-contained token: header.payload.signature.
- Stateless — server doesn't need to store session.
- Used for auth in microservices.

---

## 10. REST vs GraphQL vs gRPC

| Aspect | REST | GraphQL | gRPC |
|--------|------|---------|------|
| Style | Resource-based | Query-based | RPC |
| Schema | Implicit | Strong | Strong (protobuf) |
| Payload | Fixed | Client-defined | Fixed |
| Transport | HTTP/JSON | HTTP | HTTP/2 + protobuf |
| Use case | Public APIs | Flexible UIs | Internal microservices |

---

## 11. WebSockets

- Full-duplex, persistent connection over a single TCP socket.
- Starts as HTTP upgrade handshake.
- Use cases: chat, real-time dashboards, multiplayer games.

---

## 12. Top Interview Q&A

**Q: What's the difference between HTTPS and HTTP?**
HTTPS adds TLS for encryption, server authentication, and data integrity.

**Q: Why is TCP called reliable?**
ACKs, retransmission, sequencing, flow control, congestion control.

**Q: What is the time-wait state?**
After sending final ACK in connection termination, sender waits ~2*MSL to handle retransmitted FIN/ACK.

**Q: Why does DNS use UDP primarily?**
Fast, low overhead. Small query/response. Larger responses fall back to TCP.

**Q: What's the difference between a switch and a router?**
Switch: Layer 2, MAC addresses, local network. Router: Layer 3, IP addresses, between networks.

**Q: What is a subnet mask?**
Specifies which bits of an IP are network vs host. `/24` = first 24 bits network.

**Q: What is CORS?**
Cross-Origin Resource Sharing. Browser mechanism using HTTP headers to allow cross-domain requests.

**Q: Difference between forward and reverse proxy?**
Forward: client → proxy → internet (anonymity, filtering).
Reverse: client → proxy → backend servers (load balancing, caching, SSL termination).
