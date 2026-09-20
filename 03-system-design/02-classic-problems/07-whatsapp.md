# Design WhatsApp / Messenger (Chat System)

A real-time messaging system. Tests your understanding of WebSockets, message ordering, presence, and end-to-end encryption.

---

## 1. Requirements

### Functional
- 1:1 chat.
- Group chat.
- Online / last-seen status.
- Push notifications when offline.
- Message delivery + read receipts.
- Media messages (images, video, voice).

### Non-Functional
- Real-time delivery (< 200 ms in same region).
- Message ordering within a conversation.
- Durability — messages must not be lost.
- Encryption — only sender and receiver can read content.

---

## 2. Capacity Estimation

Assume 2B users, 50B messages/day, avg 100 friends, 50% online at peak.

| Metric | Value |
|---|---|
| Messages/day | 50B |
| Messages/sec avg | 50B / 86400 ≈ **580,000** |
| Peak (3×) | ~1.8M writes/sec |
| Concurrent online users | ~1B |
| WebSocket connections | ~1B (one per online user) |
| Avg message size | 100 B (text), 50 KB (image), 5 MB (video) |
| Storage / day (text only) | 50B × 100 B ≈ **5 TB** |

---

## 3. High-Level Design

```
 Sender ─► Chat Server (WebSocket) ─► Receiver (if online)
 │ │
 │ persistent connection
 │ │
 ▼ store
 Message DB (per conversation)
 │
 │ if receiver offline
 ▼
 Push Notification Service (APNs / FCM)
```

Key idea: **persistent WebSocket** per online user.

---

## 4. Connection Layer

### WebSocket per online client
- Persistent, full-duplex TCP connection.
- One per device; multiple devices per user.
- Server keeps a map: `user_id → list of WebSocket connections`.

### Chat server
- Holds many open sockets.
- Stateless w.r.t. message content (DB is the source of truth).
- Routes incoming message to recipient's server if needed.

### Scaling chat servers
- One server holds ~50K-100K sockets.
- 1B online users → 10K-20K chat servers.
- Need a **routing layer** (consistent hashing on `user_id`) to find the right server.

```
 Client A ─► Chat Server S1
 (user A)
 Client B ─► Chat Server S42
 (user B)

 S1 → "send to user B" → router → S42 → Client B
```

---

## 5. Message Flow (1:1 chat, both online)

1. Sender's app sends message over its WebSocket.
2. Chat server receives, persists to message DB.
3. Chat server looks up recipient's server in routing map.
4. Forwards the message to recipient's chat server.
5. Recipient's chat server pushes to recipient's WebSocket.
6. Recipient's app acks; ack flows back; sender sees "delivered" tick.

### Sequence number
Each message in a conversation gets an increasing sequence number. Clients use it to detect missing messages and reorder.

---

## 6. Offline Delivery

If recipient is offline:
- Message is stored in DB.
- Push notification is sent (APNs / FCM / Web Push) to wake up the device.
- On reconnect, client pulls missed messages via REST: `GET /conversations/{id}/messages?since_seq=N`.

---

## 7. Group Chat

- Group is identified by a `group_id`.
- Members stored in a group membership table.
- Sender publishes message once; fan-out server pushes to each online member.

### Fan-out
- **Write fan-out:** push to each member's inbox on send.
- **Read fan-out:** members pull from a shared log on demand.

For small groups (≤ 100 members): write fan-out is fine.
For huge channels: use a single log per channel; clients subscribe.

### Storage per conversation
```
Table: messages
 message_id BIGINT PK
 conversation_id BIGINT
 sender_id BIGINT
 seq BIGINT -- monotonic per conversation
 content TEXT (or media_url)
 created_at TIMESTAMP
 PRIMARY KEY (conversation_id, seq)
```

`seq` per conversation = simple ordering, easy resync.

---

## 8. Presence & Last Seen

- **Online:** server tracks which users are connected (TTL heartbeats).
- **Last seen:** persisted; updated when user disconnects.
- Broadcast presence changes via a pub/sub (Redis / Kafka) to friends who care.

Note: WhatsApp hides last seen in some cases for privacy.

---

## 9. End-to-End Encryption (E2EE)

- Signal Protocol (used by WhatsApp, Signal, Messenger).
- Each client has long-term identity keys + ephemeral pre-keys.
- Sender derives a shared secret with recipient's public bundle; messages encrypted with symmetric ratchet.
- **Server never sees plaintext.** It only sees ciphertext + metadata (who, when).

In the design interview, you only need to know:
- Each conversation has a symmetric key.
- Server is "honest-but-curious" — it can see metadata, not content.
- Public key bundles are exchanged out-of-band.

---

## 10. Media Messages

For images / videos:
1. Sender uploads media to object storage via presigned URL.
2. Server encrypts / transcodes if needed.
3. Sender's app sends a text message containing the media URL + thumbnail.
4. Recipient downloads media via CDN.

Bandwidth optimization: thumbnail first, full image on tap.

---

## 11. Database Schema

```
Table: users
 user_id BIGINT PK
 phone VARCHAR UNIQUE
 display_name VARCHAR
 last_seen TIMESTAMP

Table: conversations
 conversation_id BIGINT PK
 type ENUM(1:1, group)
 created_at TIMESTAMP

Table: conversation_members
 conversation_id BIGINT
 user_id BIGINT
 joined_at TIMESTAMP
 PRIMARY KEY (conversation_id, user_id)

Table: messages
 conversation_id BIGINT
 seq BIGINT
 sender_id BIGINT
 content TEXT
 media_url TEXT NULL
 created_at TIMESTAMP
 PRIMARY KEY (conversation_id, seq)

Table: read_receipts
 conversation_id BIGINT
 user_id BIGINT
 last_read_seq BIGINT
 PRIMARY KEY (conversation_id, user_id)
```

For 1:1 conversations, the `(conversation_id, seq)` index gives O(log N) writes.

---

## 12. Scaling Storage

- Shard `messages` by `conversation_id`.
- Hot conversations (e.g., big groups) get their own shard.
- Old messages move to cold storage after 30 days.

---

## 13. API Design

```
POST /api/v1/messages
 Body: { "to": "user_b", "content": "hi", "client_msg_id": "uuid" }
 → 201 { "message_id": "...", "seq": 42 }

GET /api/v1/conversations/{id}/messages?since_seq=42&limit=50
 → 200 { "messages": [...], "has_more": true }

POST /api/v1/conversations/{id}/read
 Body: { "last_read_seq": 100 }

WS /ws (WebSocket)
 Bidirectional: incoming messages, acks, typing indicators, presence.
```

`client_msg_id` lets the client dedupe retries (idempotency).

---

## 14. Failure Modes

| Failure | Mitigation |
|---|---|
| Chat server dies | Clients reconnect; resync missed messages via REST. |
| Recipient offline | Store message; push notification later. |
| Push notification lost | Recipient pulls on next connect. |
| Network jitter | Reconnect logic with backoff; resume seq. |
| Message DB down | Buffer messages on chat server (bounded); reject if buffer full. |

---

## 15. Optimizations

- **Idle connections** use small keep-alive heartbeats (every 30 s).
- **Compression** on WebSocket frames.
- **Server-side ack** decoupled from client delivery to amortize load.
- **Local message DB** on each chat server for hot conversations (write-through).
- **Geo-distributed chat servers** for lower latency.
- **Selective routing** for huge broadcast channels.

---

## 16. Follow-up Questions

**Q: How to deliver a message to all my devices?**
Each device opens its own WebSocket. Server maintains `user_id → [device_id, conn_id]`. Fan-out message to each.

**Q: How to support message reactions / edits / deletes?**
Store as new events in the same conversation (with `type` field). Clients re-render.

**Q: How to scale to 1B concurrent WebSockets?**
- 10K+ chat servers.
- Efficient event loop (epoll / io_uring).
- No heavy per-connection state.

**Q: How to handle disappearing messages (24 h)?**
TTL on the message row + scheduled delete job.

**Q: How to support voice/video calls?**
Separate signaling server using WebRTC; SFU for group calls.

**Q: How to prevent spam?**
Rate limits per user; ML on content patterns; report / block.

**Q: How to migrate from server to E2EE?**
Provide key bundle sync; old messages remain unencrypted (or you don't store them).

**Q: How to ensure ordering when a chat server is multi-region?**
Use a monotonic sequence per conversation, generated by the message DB.

---

## 17. End-to-End Diagram

```
 Sender Sender Chat Router Recipient Chat Recipient
 App ◄──────► Server A ◄──────────► Server B ◄──────► App
 │ │
 ▼ ▼
 ┌────────────────────────────┐
 │ Message DB (sharded by │
 │ conversation_id) │
 └────────────────────────────┘
 │
 │ offline recipients
 ▼
 ┌─────────────────┐
 │ Push (APNs/FCM) │
 └─────────────────┘
```
