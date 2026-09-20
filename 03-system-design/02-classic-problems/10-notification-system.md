# Design Notification System (Multi-Channel)

A system that sends transactional notifications (push, email, SMS, in-app, webhook) reliably and at scale.

---

## 1. Requirements

### Functional
- Send notifications to a user via one or more channels.
- Channels: **push** (APNs / FCM), **email** (SES / SendGrid), **SMS** (Twilio), **in-app**, **webhook**.
- Support templates with variables.
- Respect user preferences (don't email me about promos).
- Retry failed deliveries.
- Track delivery status.

### Non-Functional
- **At-least-once** delivery (never lose a notification you tried to send).
- High throughput — millions of notifications/day.
- Low latency where needed (real-time OTP) but acceptable delay for marketing.
- Rate limiting and throttling (don't blast a user).
- Bounce / complaint handling.

---

## 2. Capacity Estimation

Assume 100M users, 200M notifications/day (including marketing).

| Metric | Value |
|---|---|
| Notifications/day | 200M |
| Notifications/sec avg | 200M / 86400 ≈ **2,300** |
| Peak (5×) | ~12,000 |
| Burst (promo) | up to 100,000/sec |

---

## 3. High-Level Design

```
 Producer (any service)
 │
 ▼
 Notification Service
 │
 ├── Validation / dedup
 ├── Template rendering
 ├── User preference check
 ├── Channel selection
 │
 ▼
 Notification Queue (Kafka / SQS)
 │
 ▼
 Channel Workers (per channel)
 │ ├─ Push Worker (APNs / FCM)
 │ ├─ Email Worker (SES)
 │ ├─ SMS Worker (Twilio)
 │ ├─ In-App Worker (WebSocket)
 │ └─ Webhook Worker
 │
 ▼
 Providers
 │
 ▼
 Delivery status callback → Status Store
 │
 ▼
 Analytics / dashboards
```

---

## 4. Producers

Any service emits a notification:
```
POST /notifications
 Body: {
 user_id: 123,
 template: "order_shipped",
 channels: ["push", "email"],
 data: { order_id: "..." },
 idempotency_key: "..."
 }
```

The `idempotency_key` makes retries safe — same key = same notification.

---

## 5. Pipeline Steps

1. **Receive** request, validate `idempotency_key` (Redis SETNX).
2. **Prefilter:** do we have consent / preference on file for this user? Check preferences table.
3. **Render** template with variables. (Keep templates in DB, cache compiled.)
4. **Personalize** (locale, time zone, name).
5. **Channel selection:**
   - User has push enabled + recent activity → push.
   - Quiet hours → buffer for later.
   - Critical (OTP, security) → SMS + push.
6. **Enqueue** per-channel messages in Kafka.
7. **Workers** consume, call providers.
8. **Status callbacks** update delivery state.
9. **Retry** failures with backoff.

---

## 6. Templates

Two engines:
- **String templates:** `Hello, {{user.name}}!`
- **Block-based:** structured subject/body blocks for in-app, with i18n.

Store rendered output when possible to avoid re-render cost.

---

## 7. User Preferences

```
Table: notification_prefs
 user_id BIGINT PK
 email_enabled BOOL
 push_enabled BOOL
 sms_enabled BOOL
 marketing BOOL
 digest_frequency ENUM(realtime, hourly, daily, off)
 quiet_hours_start TIME
 quiet_hours_end TIME
 locale VARCHAR
 updated_at TIMESTAMP
```

Look up asynchronously and cache in Redis (`prefs:{user_id}`, TTL 1h).

---

## 8. Provider Integration

### Push (APNs / FCM)
- HTTP/2 long-lived connection per provider region.
- Batch up to 500 tokens per request.
- Honor `apns-collapse-id` for collapsibles.

### Email
- Use a managed provider (SES, SendGrid, Postmark).
- Handle bounces / complaints via webhooks → update user prefs.

### SMS
- Twilio / Vonage.
- Watch for delivery receipts (DLR).
- Rate-limit per destination to avoid carrier spam triggers.

### Webhook
- Outbound HTTP POST to user's URL.
- HMAC signature for authenticity.
- Retry with exponential backoff (8 attempts over 24 h).
- Track response codes; alert on failure.

---

## 9. Reliability

### Idempotency
`idempotency_key` stored in Redis with TTL; second request is a no-op.

### Retry / DLQ
- Per-channel DLQ for messages failing after N retries.
- On-call reviews DLQ.

### Backpressure
- If a worker is slow → Kafka lag grows, consumer group rebalances.
- Don't shed — buffer.

### Ordering
- Per-user ordering for related events (e.g., password change then MFA prompt).
- Use a partition key = `user_id`.

---

## 10. Rate Limiting & Deduplication

- **Throttle per user:** e.g., max 5 marketing push per day.
- **Throttle per channel/provider:** match provider limits.
- **Coalesce:** if 10 alerts fire in 1 minute, send one digest.

---

## 11. Database Schema

```
Table: notifications
 notif_id UUID PK
 user_id BIGINT
 template VARCHAR
 status ENUM(queued, sent, delivered, failed, suppressed)
 idempotency_key VARCHAR UNIQUE
 created_at TIMESTAMP
 delivered_at TIMESTAMP NULL

Table: notification_attempts
 notif_id UUID FK
 channel VARCHAR
 attempt INT
 provider_msg_id VARCHAR
 response_code INT
 error TEXT
 attempted_at TIMESTAMP

Table: notification_templates
 template_id VARCHAR PK
 subject TEXT
 body_text TEXT
 body_html TEXT
 body_push TEXT
 channels TEXT[] -- which channels this template supports
 locale VARCHAR DEFAULT 'en'
```

---

## 12. Optimizations

- **Batching:** coalesce notifications for one user into a digest.
- **Templates cached** in-memory on workers.
- **Connection pools** to each provider.
- **Provider-specific retries:** some channels retry more than others.
- **Bounce/complaint webhooks** auto-suppress bad recipients.
- **Local regional providers** for lower latency (Twilio regions, SES regions).

---

## 13. Failure Modes

| Failure | Mitigation |
|---|---|
| Provider down | Retry queue → backup provider (SNS/SES fallback). |
| User device offline | Push notifications queued by APNs / FCM. |
| Email bounces | Webhook → suppress. |
| SMS carrier fails | Retry with backoff; report eventually. |
| Template error | Validate templates at deploy; test render before send. |

---

## 14. API Design

```
POST /api/v1/notifications
 Body: { user_id, template, data, channels, idempotency_key }
 → 202 { notif_id, status: "queued" }

GET /api/v1/notifications/{id}
 → 200 { status, attempts: [...], delivered_at }

POST /api/v1/notifications/{id}/retry
 → 202

POST /webhooks/{provider} (provider → us)
 → 204 (ack)

GET /api/v1/prefs/{user_id}
PUT /api/v1/prefs/{user_id}
```

---

## 15. Follow-up Questions

**Q: How to ensure a notification is delivered exactly once?**
You can't — networks fail. Use `idempotency_key` so retries don't multiply sends. Use message hashes to dedupe on the receiver side.

**Q: How to respect user quiet hours across time zones?**
Store user's TZ. Compute local quiet hours at send time. Re-queue if outside allowed window.

**Q: How to handle digest mode?**
Buffer notifications for N hours; periodic job aggregates per user; render digest template.

**Q: How to localize?**
Render with locale-specific template; pick at send time using user's preference and content negotiation.

**Q: How to track delivery accurately?**
Combine sent events + provider callbacks. Reconcile with status pipeline.

**Q: How to test new templates without affecting users?**
Render-only mode: render the template against fixtures; nothing sent.

**Q: How to scale to billions / day?**
Partition by user; multiple consumer groups; horizontal scale per channel.

**Q: How to handle ordered events (e.g., "your payment succeeded" → "your receipt")?**
Single partition key (`user_id`) ensures order. Use sequence numbers client-side if needed.

**Q: How to secure webhook delivery?**
HMAC signature, replay protection (timestamp + nonce), short-lived tokens per endpoint.

---

## 16. End-to-End Diagram

```
 Producer ─► Notification Service
 │
 ├─ dedup, prefs, render
 │
 ▼
 Kafka topic: notifications.all
 │
 ├── push.workers ─► APNs / FCM
 ├── email.workers ─► SES
 ├── sms.workers ─► Twilio
 ├── inapp.workers ─► WebSocket fan-out
 └── webhook.workers ─► Customer URL

 Status callbacks (webhooks)
 │
 ▼
 Status Store (Postgres + Redis) ─► Analytics
```
