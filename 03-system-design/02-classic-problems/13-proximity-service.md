# Design Proximity Service (Yelp / Google Places)

Tests geospatial indexing and search at scale.

---

## 1. Requirements

### Functional
- Search for businesses / places near a location.
- Filter by category, rating, price, open-now.
- Sort by distance or relevance.
- Show details (hours, photos, reviews).

### Non-Functional
- Low-latency searches (< 200 ms).
- Global coverage.
- Up-to-date info (hours, availability).

---

## 2. Capacity Estimation

Assume 200M businesses globally, 50M in dense cities, 50K searches/sec.

| Metric | Value |
|---|---|
| Total businesses | 200M |
| Search QPS | 50K avg, 200K peak |
| Geospatial data size | 200M × 1 KB ≈ **200 GB** |
| Reviews | 100B; size in PB |

---

## 3. High-Level Design

```
 Client (app)
 │
 ▼
 API Gateway
 │
 ▼
 Search Service
 │
 ├── Geo Index (QuadTree / H3 / Elasticsearch geo)
 ├── Text Search (Elasticsearch)
 ├── Filters (Postgres + cache)
 │
 ▼
 Ranker
 │
 ▼
 Response (places + photos + reviews)
```

---

## 4. Geospatial Index

### Option 1 — Elasticsearch geo_point / geo_shape
- Built-in geo queries (`geo_distance`, `geo_bounding_box`).
- Combine with text search.

### Option 2 — H3 (Uber's hex grid)
- Encode (lat, lng) into a hex ID.
- Neighbors are easy to enumerate.
- Great for "all places in this hex + neighbors".

### Option 3 — PostGIS
- GIST index on geography column.
- Simple, SQL-friendly.

### Option 4 — QuadTree
- In-memory tree rebuilt periodically.
- Very fast for large static datasets.

### Recommendation
- Elasticsearch for combined text + geo search.
- H3 for fast pre-filtering when the query is "near (lat, lng)".

---

## 5. Schema (Elasticsearch index)

```
business:
 id
 name (text + keyword)
 description
 categories [keyword]
 location (geo_point)
 address (text)
 city (keyword)
 rating (float)
 review_count (int)
 price_level (int 1-4)
 hours (nested)
 photos [url]
 open_now (bool, computed)
 updated_at (date)
```

---

## 6. Search Query

```
GET /api/v1/search?lat=37.78&lng=-122.41&radius=2km&q=pizza&min_rating=4&open_now=true
```

Pipeline:
1. Parse query.
2. **Geo pre-filter** with H3 (hex contains candidates).
3. **Elasticsearch query:** bool query = `geo_distance` + text + filters.
4. **Ranker:** blend distance, rating, popularity, sponsored.
5. Return top-N.

---

## 7. Updates

- New / edited business → publish to Kafka.
- Indexer consumer updates Elasticsearch.
- Cache invalidation: in-place update + delete from `business:{id}` in Redis.

---

## 8. Photos

- Object storage (S3).
- Thumbnail variants generated on upload (CDN cache).
- Signed URLs for originals.

---

## 9. Reviews

- Postgres, sharded by `business_id`.
- Pagination by cursor.
- Aggregate rating + count stored on `business` row; updated async.

---

## 10. Optimizations

- **H3 pre-filter** reduces the candidate set 10-100×.
- **Local cache** of popular queries (e.g., "pizza near me" in SF).
- **Personalization layer** if user is logged in.
- **Geo replica per region** for proximity.
- **Photograph CDN** with size variants.

---

## 11. Failure Modes

| Failure | Mitigation |
|---|---|
| ES cluster down | Fallback to PostGIS geo + SQL filters (slower). |
| Index lag | New edits become visible in seconds; accept. |
| Hot region (NYC at noon) | More shards in dense regions; over-provision. |

---

## 12. API Design

```
GET /api/v1/search
 ?lat&lng&radius&category&q&min_rating&price&open_now&cursor&limit

GET /api/v1/businesses/{id}
 → 200 { name, location, hours, photos, reviews_count, ... }

GET /api/v1/businesses/{id}/reviews?cursor&limit
POST /api/v1/businesses/{id}/reviews
```

---

## 13. Follow-up Questions

**Q: How to handle "near" when user moves?**
Use current lat/lng; re-query every position update (debounced).

**Q: How to surface relevant results vs distance?**
Custom ranker: `score = w1 * relevance + w2 * (1/distance) + w3 * rating + w4 * sponsored`.

**Q: How to support "open now"?**
Precompute `open_now` per business per minute; index as boolean.

**Q: How to support autocomplete?**
Tries + ES completion suggester. Pre-warm popular prefixes.

**Q: How to handle places with multiple branches?**
Index each branch as a separate doc, with parent brand; dedup at query time if needed.

**Q: How to prevent duplicates?**
Source-of-truth DB with unique constraint on (name, address); human review for ambiguous cases.

---

## 14. End-to-End Diagram

```
 App ─► API ─► Search Service
 │
 ├─► Elasticsearch (geo + text + filters)
 ├─► Ranker (relevance, distance, popularity)
 ├─► Cache (Redis)
 │
 ├─► Postgres (reviews, photos metadata)
 └─► S3 (photos)
```
