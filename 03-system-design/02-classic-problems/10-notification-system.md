# Design Notification System

## Requirements

### Functional
- Send notifications via multiple channels: Push (mobile), Email, SMS, In-app.
- Trigger on events (e.g., order shipped, video uploaded).
- User notification preferences.

### Non-Functional
- Reliable (at-least-once delivery).
- Low latency.
- Scalable.
- Decoupled from main app.

---

## High-Level Design

```
 ┌─────────────┐
[App Service / Event] ────►│ Notification│──┐
 │ Service │ │
[CRM / Marketing] ────►│ │ │
 └─────┬───────┘ │
 │ │
 ┌─────▼──────┐ │
 │ Kafka / │ │
 │ Queue │ │
 └─────┬──────┘ │
 │ │
 ┌────────────────────────┼────────────────────────┐
 ▼ ▼ ▼
[Push Service] [Email Service] [SMS Service]
 (APNs/FCM) (SES/SendGrid) (Twilio)
 │ │ │
 └────────────────────────┴────────────────────────┘
 │
 ▼
 [Delivery Tracking]
 [Retry/Logging DB]
```

---

## Components

### 1. Notification Service (API)
- Receives notification request (user_id, type, payload, channel).
- Validates user prefs (channel enabled, do-not-disturb).
- Enriches with user data.
- Publishes event to Kafka.

### 2. Notification Processor (Workers)
- Subscribe to Kafka topics per channel.
- Apply templates (i18n, personalization).
- Call channel-specific delivery service.
- Handle retries with exponential backoff.
- Dead-letter queue after N failures.

### 3. Channel-Specific Services
- **Push:** APNs (iOS) / FCM (Android).
- **Email:** SES, SendGrid, Mailgun.
- **SMS:** Twilio, AWS SNS.
- **In-app:** WebSocket / persistent connection.

### 4. User Preferences
- Per-user settings: which events trigger which channels.
- Quiet hours, frequency cap.
- Stored in `user_preferences` table (Redis cache).

### 5. Templates
- Per event type, per channel.
- Localization (i18n) support.
- A/B testing variants.

### 6. Tracking
- Log every attempt: success/fail/retry.
- Idempotency key to prevent duplicates.

---

## Schema

```sql
CREATE TABLE notifications (
 notif_id UUID PRIMARY KEY,
 user_id BIGINT,
 type VARCHAR(50),
 channel ENUM('PUSH','EMAIL','SMS','INAPP'),
 payload JSON,
 status ENUM('PENDING','SENT','FAILED','READ'),
 attempts INT,
 scheduled_at TIMESTAMP,
 sent_at TIMESTAMP NULL,
 created_at TIMESTAMP
);

CREATE TABLE user_preferences (
 user_id BIGINT PRIMARY KEY,
 prefs JSON -- { "marketing": ["email"], "transactional": ["push", "email"] }
);
```

---

## Reliability

- **At-least-once:** Kafka + retry + idempotency key.
- **Idempotency:** Use `notif_id` to deduplicate.
- **Retry:** Exponential backoff; max attempts; DLQ for poison messages.
- **Tracking:** All attempts logged; reconcile with delivery receipts.

---

## Optimizations

1. **Batch sends** — group users per template.
2. **Connection pooling** to APNs/FCM.
3. **Provider failover** — if Twilio fails, fall back to AWS SNS.
4. **Rate limiting** — respect provider limits; user frequency caps.
5. **Priority queues** — transactional > marketing.

---

## Follow-up Questions

**Q: How to prevent spamming users?**
Frequency caps (e.g., max 5 marketing emails/week). Per-user counters in Redis.

**Q: How to support real-time in-app notifications?**
WebSocket / SSE; persistent connection per logged-in user.

**Q: How to handle provider outages?**
Retry queues; secondary providers; circuit breakers.

**Q: How to ensure exactly-once?**
Idempotency keys + dedup window in DB.

**Q: How to design A/B test notifications?**
Template variants; hash on user_id for consistent assignment.
