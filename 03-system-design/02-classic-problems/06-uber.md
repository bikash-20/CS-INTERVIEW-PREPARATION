# Design Uber / Lyft (Ride Sharing)

## Requirements

### Functional
- Riders request rides.
- Drivers see nearby ride requests.
- Matching: nearest available driver.
- Real-time location tracking.
- Trip start/end, fare calculation, payment.

### Non-Functional
- Low-latency matching.
- High availability.
- Handle millions of concurrent users.

---

## High-Level Design

```
┌────────┐ ┌──────────┐ ┌──────────────┐
│ Rider │────────►│ Trip │──────►│ Dispatch │
│ App │ │ Service │ │ Service │
└────────┘ └──────────┘ └──────┬───────┘
 │
 ▼
┌────────┐ ┌──────────┐ ┌──────────────┐
│ Driver │────────►│ Location │ │ Matching │
│ App │ │ Service │──────►│ Engine │
└────────┘ └──────────┘ └──────────────┘
 │ │
 ▼ ▼
[ WebSocket/RTC [ Redis: geo-index
 for live GPS ] of drivers ]
```

---

## Core Components

### 1. Location Service
- Drivers send GPS updates every 3-5 seconds.
- Update Redis with current location.
- Use **Redis GEO commands** (`GEOADD`, `GEORADIUS`) for geospatial indexing.

```redis
GEOADD drivers:beijing <lng> <lat> driver_123
GEORADIUS drivers:beijing <rider_lng> <rider_lat> 5 km WITHDIST
```

### 2. Trip Service
- Trip state machine: `REQUESTED → MATCHED → EN_ROUTE → IN_PROGRESS → COMPLETED → PAID`.
- Persists trip state in DB.

### 3. Dispatch / Matching
- On ride request, find nearby drivers via geospatial index.
- Try drivers in order of distance + rating.
- Send offer to driver (push notification / WebSocket).
- If no acceptance in N seconds, try next.
- On acceptance → MATCHED.

### 4. Real-Time Communication
- WebSocket for location updates (server pushes, driver receives).
- Use socket.io / MQTT / gRPC streams.

### 5. Pricing
- Base fare + (per km * distance) + (per min * time) + surge multiplier.
- Surge: based on demand vs supply in area.
- Maps to dynamic pricing service.

---

## Schema

```sql
CREATE TABLE trips (
 trip_id BIGINT PRIMARY KEY,
 rider_id BIGINT,
 driver_id BIGINT NULL,
 status VARCHAR(20),
 pickup_lat DOUBLE, pickup_lng DOUBLE,
 drop_lat DOUBLE, drop_lng DOUBLE,
 requested_at TIMESTAMP,
 matched_at TIMESTAMP NULL,
 started_at TIMESTAMP NULL,
 ended_at TIMESTAMP NULL,
 fare DECIMAL(10,2) NULL,
 surge_multiplier DECIMAL(3,2)
);

CREATE TABLE drivers (
 driver_id BIGINT PRIMARY KEY,
 status ENUM('OFFLINE','ONLINE','ON_TRIP'),
 current_lat DOUBLE, current_lng DOUBLE,
 last_heartbeat TIMESTAMP,
 rating DECIMAL(3,2)
);
```

---

## Key Design Challenges

### 1. Efficient Nearest Driver Query
- **Naive:** Scan all drivers. Doesn't scale.
- **Geohash:** Divide world into cells; drivers in nearby cells.
- **Redis GEO:** O(log N) per query. Excellent for this use case.
- **QuadTree / Google S2:** Spatial indexing library.

### 2. Surge Pricing
- Real-time demand vs supply per geo-cell.
- Computed periodically (e.g., every 30s).
- Adjust multiplier.

### 3. ETA Calculation
- Use map service (OSRM, Google Maps, GraphHopper) — precomputed graph + live traffic.
- Cache ETAs.

### 4. Distributed Trip State
- Single source of truth in DB; updates via trip service.
- Use Kafka events to broadcast state changes.

### 5. Driver Acceptance Rate
- Track acceptance; lower priority for chronic decliners.

---

## Capacity Estimation

- 1M drivers online.
- 100K trips in progress simultaneously.
- 30K location updates/sec (1M drivers / 30s each).
- 10K new trip requests/sec.

---

## Optimizations

1. **Pre-compute surge** every minute per cell.
2. **Cache driver list per geo-cell** in Redis.
3. **Batch driver location updates** in memory then flush.
4. **Use WebSockets** for bidirectional comm (vs HTTP polling).
5. **Predict demand** using ML (for driver positioning).

---

## Follow-up Questions

**Q: How to handle payment failures?**
Retry; idempotent transactions; ledger entry; offline mode for cash.

**Q: How to detect fraud (fake GPS)?**
Sanity-check locations; speed limits; device fingerprinting; ML on movement patterns.

**Q: How to scale globally?**
Region-based deployment; data residency; map data local to region.

**Q: How to design for offline drivers?**
Driver app caches pending requests; reconnects and syncs state.
