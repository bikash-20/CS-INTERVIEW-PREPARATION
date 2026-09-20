# Design WhatsApp / Messenger (Chat System)

## Requirements

### Functional
- 1-to-1 and group chat.
- Online presence (online/offline/last seen).
- Message delivery and read receipts.
- Push notifications when offline.
- Media messages (image, video, voice).

### Non-Functional
- Real-time delivery.
- Message ordering.
- High availability.
- End-to-end encryption (E2EE).

---

## High-Level Design

```
┌────────┐ ┌────────┐
│ Sender │── WebSocket ──► Chat Gateway ──►│Receiver│
└────────┘ │ └────────┘
 ▼
 ┌──────────┐
 │ Message │
 │ Service │
 └────┬─────┘
 │
 ▼
 ┌────────────────┐ ┌──────────┐
 │ Message Store │ │ Push │
 │ (Cassandra/ │ │ Notif │
 │ HBase) │ │ Service │
 └────────────────┘ └──────────┘
 │
 ▼
 ┌────────────────┐
 │ Presence Svc │ (Redis)
 └────────────────┘
```

---

## Core Components

### 1. Chat Gateway (WebSocket Server)
- Maintains persistent WebSocket connection per user.
- Receives messages; routes to recipient(s).
- Stateless — multiple instances behind load balancer.

### 2. Message Service
- Validates, persists, and routes messages.
- Generates message_id (Snowflake).
- Maintains conversation list per user.

### 3. Message Storage
- **Cassandra / HBase** — wide column; excellent for time-series chat history.
- Partition by `(user_id, conversation_id)` for fast retrieval.
- Messages sorted by timestamp.

```sql
-- Conceptual schema
PRIMARY KEY ((user_id, conversation_id), timestamp)
```

### 4. Presence Service
- Tracks online status.
- Heartbeat every 30s.
- **Redis:** `SET presence:user_123 online EX 60`; expire on disconnect.
- On expiration → offline.

### 5. Push Notification Service
- For offline users, send push via APNs (iOS) / FCM (Android).
- User's device token registered with service.

---

## Message Flow

### Sending a Message
```
1. Sender → Chat Gateway: WebSocket frame with message
2. Gateway → Message Service: validate, assign msg_id
3. Message Service → Message Store: persist (write-ahead)
4. Message Service → Recipient's Gateway (if online): forward
5. Message Service → Push Notif (if offline): notify device
6. Recipient: ack → Service marks as delivered/read
```

---

## Message Ordering

- Each message has `Snowflake ID` → sortable by time.
- Per-conversation ordering guaranteed by single partition.
- For group chats, server is source of truth (clients don't re-order).

---

## End-to-End Encryption (Signal Protocol)
- Each user has a key pair.
- Sender encrypts with recipient's public key.
- Server never sees plaintext.
- Forward secrecy via ratcheting.

---

## Group Chat

- Group is a logical entity with member list.
- Sender sends to group; service fans out to all members.
- For large groups (>500), use **read receipts aggregation** lazily.

---

## Schema

```sql
CREATE TABLE messages (
 message_id BIGINT PRIMARY KEY,
 conversation_id BIGINT,
 sender_id BIGINT,
 recipient_id BIGINT, -- or group_id
 content_encrypted TEXT,
 type ENUM('TEXT','IMAGE','VIDEO','AUDIO'),
 created_at TIMESTAMP,
 delivered_at TIMESTAMP NULL,
 read_at TIMESTAMP NULL
);
```

---

## Capacity

- 2B users.
- 100B messages/day.
- ~1M messages/sec globally.
- Average message size: 1KB.

---

## Optimizations

1. **Connection pooling** — many users per gateway.
2. **Message batching** — small messages coalesced.
3. **Compression** of message bodies.
4. **Tiered storage** — recent messages on hot SSD, older on cold storage.
5. **CDN for media** — image/video files.
6. **End-to-end retry** with exponential backoff.

---

## Follow-up Questions

**Q: How to support read receipts in groups?**
Aggregate receipts lazily; send batched updates.

**Q: How to migrate users without losing messages?**
Dual-write during migration; verify counts; cut over; backfill if needed.

**Q: How to handle media uploads?**
Direct upload to S3 via signed URL; send media reference in chat.

**Q: How to support voice/video calls?**
Use WebRTC; signaling server to negotiate; STUN/TURN for NAT traversal.

**Q: How to ensure message delivery on flaky networks?**
Ack-based; client buffers unsent messages; exponential backoff retry.
