# Design Uber / Lyft (Ride Sharing)

A location-heavy, real-time dispatch system. Tests geospatial indexing, real-time updates, and matching at scale.

---

## 1. Requirements

### Functional
- Riders request a ride.
- Drivers see nearby requests, accept one.
- Match rider ↔ driver.
- Real-time location updates on map.
- Trip lifecycle (start, end, payment).
- Driver onboarding, ratings, fares.

### Non-Functional
- Real-time updates at low latency (< 2 s).
- Massive concurrency — millions of active drivers.
- Geospatial accuracy — match with drivers within ~1 km.
- High availability; location updates must keep flowing even during a deploy.

---

## 2. Capacity Estimation

Assume 30M daily rides, avg 5 drivers actively looking per km² in busy cities.

| Metric | Value |
|---|---|
| Active drivers globally (peak) | ~5M |
| Location updates / driver / min | 1 |
| Location writes/sec | 5M / 60 ≈ **80,000** |
| Ride request writes/sec | 30M / 86400 ≈ **350** avg, ~10× peak |
| Concurrent open trip sessions | ~500K |

---

## 3. High-Level Design

```
 Rider app Driver app
 │ │
 API Gateway API Gateway
 │ │
 ┌──────────────┐ ┌──────────────┐
 │ Rider │ │ Driver │
 │ Service │ │ Service │
 └────┬─────────┘ └─────┬────────┘
 │ │
 ┌────┴────┐ ┌──────┴───────┐
 │ Postgres│ │ Driver Loc │
 │ (users, │ │ Service │
 │ trips) │ │ (geospatial │
 └─────────┘ │ index) │
 └────┬───────┘
 │
 ┌────────────────┐
 │ Dispatcher │ ◄── Kafka (ride.requested)
 │ (matching) │
 └────────┬───────┘
 │
 WebSocket / push to driver apps
```

---

## 4. Geospatial Indexing

The core problem: "find drivers near point (lat, lng)".

### Options

**QuadTree**
- Recursively divide the world into 4 quadrants.
- Each leaf has at most N items; subdivide when full.
- Fast range queries and point updates.
- **Used by Uber.**

**Google S2 / H3**
- Project sphere onto cells with stable IDs.
- Excellent for proximity + clustering.
- H3 (hexagonal grid) popular for analytics and joins.

**R-Tree (PostGIS)**
- Standard geospatial index in relational DBs.
- Simpler but heavier at very high update rates.

**Redis GEORADIUS (GEOADD)**
- Use for low-cardinality, high-velocity cases.
- Limited to bounding-box queries.

### Recommendation
- Hot path: in-memory QuadTree, rebuilt every few seconds from Kafka stream.
- Cold path / analytics: PostGIS or BigQuery GIS.

---

## 5. Driver Location Pipeline

```
 Driver app (every 3-5 s)
 │
 │ HTTPS / gRPC over WebSocket
 ▼
 API Gateway
 │
 ▼
 Driver Location Service
 │
 ├─ Kafka topic: driver.location (partition by driver_id)
 │
 ├─ Stream consumer updates QuadTree
 │
 └─ Stream consumer updates Postgres driver.last_location
```

Why Kafka?
- Decouples write path from query path.
- Lets multiple consumers rebuild different indexes (QuadTree, analytics).

---

## 6. Ride Request Flow

1. Rider taps "Request".
2. API → Dispatcher with pickup lat/lng.
3. Dispatcher queries QuadTree: drivers within 1 km.
4. Filters by availability, vehicle type, rating.
5. Sends ride offer to top-K drivers (e.g., 5).
6. First driver to accept wins.
7. Dispatcher notifies other drivers the offer is gone.
8. Trip record created in Postgres.
9. Both apps move to "in trip" mode.

### Why not broadcast to all nearby drivers?
- Too many notifications.
- Better UX: small set of likely matches.

### Why not a single global dispatcher?
- One per city/region for latency + fault isolation.

---

## 7. Trip Lifecycle

```
States: REQUESTED → MATCHED → EN_ROUTE → ARRIVED → IN_TRIP → COMPLETED → PAID
```

Transitions publish to Kafka; trip service updates Postgres; events go to billing, ratings, etc.

---

## 8. Pricing & ETA

### ETA
- Distance + traffic + historical patterns.
- Use a routing service (OSRM, Google Maps) per pair.
- Cache hot routes for popular corridors.

### Fare
- Base + per-km + per-minute + surge.
- Surge computed from supply / demand in the area.

---

## 9. Real-time Map Updates

- Each driver app opens a WebSocket to API Gateway.
- Server pushes relevant events (new trip offer, rider cancellation, position of matched rider).
- WebSocket is kept open with heartbeat.
- On reconnect, client re-syncs state with REST.

---

## 10. Database Schema

```
Table: users
 user_id BIGINT PK
 role ENUM(rider, driver, both)
 phone VARCHAR
 email VARCHAR
 rating_avg FLOAT

Table: drivers
 driver_id BIGINT PK (FK users)
 vehicle_type ENUM(motorbike, car, xl)
 license_no VARCHAR
 status ENUM(offline, online, on_trip)
 current_lat DOUBLE
 current_lng DOUBLE
 updated_at TIMESTAMP

Table: trips
 trip_id BIGINT PK
 rider_id BIGINT
 driver_id BIGINT
 pickup_lat, pickup_lng
 drop_lat, drop_lng
 state ENUM(...)
 requested_at, accepted_at, started_at, ended_at
 fare DECIMAL
 rating INT
```

---

## 11. Optimizations

- **Edge WebSocket servers** close to drivers; sticky connections.
- **Coalesce location updates:** don't process every tick; interpolate between updates on the client.
- **Pre-compute ETAs** between hot zones every minute.
- **Sharded dispatchers** per city.
- **Backpressure:** if a dispatcher is slow, drop to less-frequent location writes.
- **Cached nearby-driver lists** per hot pickup zone (e.g., airport queue).

---

## 12. Failure Modes

| Failure | Mitigation |
|---|---|
| Driver offline mid-trip | Notify rider; offer reassignment or cancel. |
| Dispatcher crash | Failover to replica; in-flight offers expire. |
| Map service slow | Cached ETAs; rough fallback (haversine distance). |
| Payment service down | Defer payment; trip still ends; retry async. |
| Surge calc wrong | Manual override / clamp in app. |

---

## 13. Follow-up Questions

**Q: How to handle very dense areas (concerts)?**
- Geo-fence the area; queue riders; assign in batches.
- Pre-position drivers (driver incentives).

**Q: How to ensure no double-match?**
- Optimistic: insert trip with state = MATCHED into a unique row by driver_id. DB unique constraint prevents two open trips for same driver.

**Q: How to test matching logic at scale?**
- Replay historical location streams; simulate demand; measure acceptance rate.

**Q: How to handle driver cancellations?**
- Trip moves back to REQUESTED; offer to next best driver; cap on retries.

**Q: How to detect fraud (fake GPS)?**
- Compare reported speed vs allowed; cross-check with map constraints; anomaly ML.

**Q: How to support ride-pooling (shared rides)?**
- Match multiple riders going in the same direction; route becomes a multi-stop sequence (TSP-ish).

**Q: How to support offline ETA prediction when traffic data is sparse?**
- Use historical averages for that route at that time-of-day.

**Q: How to scale to 100M concurrent drivers?**
- Shard by geohash / city; per-shard dispatcher; load balancer in front.

---

## 14. End-to-End Diagram

```
 Driver app ─► Driver Loc Service ─► Kafka
 │ │
 │ │
 │ ▼
 │ QuadTree in-memory
 │ ▲
 │ │ query
 Rider app ─► Dispatcher ──────────┘
 │ │
 ▼ ▼
 Kafka (trip events)
 │
 ▼
 Trip Service ─► Postgres (trips, billing)
 │
 ▼
 Notifications (push, SMS)
```
