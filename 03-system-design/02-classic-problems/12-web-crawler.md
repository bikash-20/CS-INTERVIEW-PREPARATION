# Design Web Crawler (Googlebot / Bingbot Style)

Tests distributed systems thinking: politeness, dedup, scale, fault tolerance.

---

## 1. Requirements

### Functional
- Given a set of seed URLs, crawl pages and extract links.
- Add new URLs to the frontier.
- Respect `robots.txt`.
- Avoid revisiting.
- Store page contents for indexing.

### Non-Functional
- **Politeness:** don't hammer a single domain.
- **Scale:** billions of pages.
- **Fault-tolerant:** survive worker crashes; URLs persist.
- **Observability:** QPS, errors, queue depth.

---

## 2. Capacity Estimation

| Metric | Value |
|---|---|
| Pages crawled (initial) | 1B |
| URLs extracted per page | ~50 |
| Total URLs in frontier | up to 50B |
| Crawl rate (steady state) | 10K pages/sec |
| Avg HTML size | 50 KB |
| Storage for raw pages / month | 10K × 86400 × 50 KB ≈ **40 TB** |

---

## 3. High-Level Design

```
 Seed URLs
 │
 ▼
 URL Frontier (priority queue)
 │
 ▼
 DNS Resolver
 │
 ▼
 Fetch Workers (HTTP GET)
 │
 ▼
 Parsers / Extractors
 │
 ├─ Content Storage → Indexer (offline)
 │
 └─ Outgoing URLs → Dedup + Filter → Frontier
```

---

## 4. URL Frontier

A **priority queue** with:
- Politeness per host: max N concurrent requests per host; min delay between requests.
- Prioritization: important URLs first (homepages, fresh news).
- Persistent: must survive crashes (Kafka or DB-backed).

### Implementation
- Per-host queue.
- Active workers pick the next host with a slot.
- Within host, FIFO or by priority.

---

## 5. Distributed Workers

- Thousands of workers.
- Coordinate via distributed queue (Kafka) or DB-backed leases.
- Sticky assignment per host (one worker fetches from one host at a time → easier politeness).

---

## 6. Fetch & Parse

1. Fetch HTML.
2. Parse with library (`BeautifulSoup`, `jsdom`, `lxml`).
3. Extract `<a href>` URLs.
4. Normalize URL (resolve, lowercase host, strip fragments).
5. Filter (robots.txt, disallow paths, MIME types).
6. Push new URLs to dedup check.

---

## 7. De-duplication

Many URLs return same content (and many collisions).
- **URL dedup:** hash normalized URL; check Bloom filter or Redis set.
- **Content dedup:** hash page body (SimHash / MinHash); skip near-duplicates.

Bloom filter for "have we seen this URL?" — fast, space-efficient.

---

## 8. Politeness

- **robots.txt** per host, cached and re-fetched weekly.
- Per-host rate limit: e.g., 1 req / 2 s.
- Honor `Crawl-delay` directive.
- Identify ourselves in `User-Agent`.

---

## 9. Robustness

- Retries with backoff for transient errors (5xx, network).
- Crawl traps (calendars with infinite next pages): detect by depth / uniqueness.
- Per-host failure budget; skip host for backoff window after repeated errors.

---

## 10. Freshness

- Re-crawl important pages more frequently.
- "Last-Modified" / ETag support — conditional GET.
- Sitemap.xml ingestion.

---

## 11. Storage

### Frontier DB
```
Table: urls
 url_hash CHAR(64) PK
 url TEXT
 host VARCHAR
 last_crawled_at TIMESTAMP
 next_crawl_at TIMESTAMP
 priority INT
 state ENUM(queued, fetching, fetched, error, blocked)
```

### Page Store
Object storage (S3) of raw HTML / extracted text. Indexed by `url_hash`.

### Index (out of scope for crawler)
Built by a separate indexer reading from page store.

---

## 12. Optimizations

- **Per-host sharding** of the frontier so each worker pulls from few hosts.
- **Bloom filter** for URL dedup; periodic flush to persistent set.
- **Fetcher pipelining** — open N concurrent TCP connections per host (HTTP/1.1 limit 6-8, HTTP/2 many more).
- **HTTP/2 + HTTP/3** for fewer connections.
- **Adaptive politeness** — learn the slowest acceptable rate per host.

---

## 13. API / Admin Tools

- List URLs in frontier.
- Force a re-crawl.
- Block a host.
- Adjust priority.

---

## 14. Failure Modes

| Failure | Mitigation |
|---|---|
| Worker crash | URL re-queued after lease expiry. |
| Host blocks us | Slower rate; backoff. |
| DNS slow | DNS cache; resolver pool. |
| Infinite trap detected | Mark host as problematic. |
| Indexer slow | Page store buffered; OK. |

---

## 15. Follow-up Questions

**Q: How to avoid crawler traps?**
Limit depth, max URLs per path, detect cycles, monitor URL count growth per host.

**Q: How to ensure freshness?**
Priority boost for sitemaps / news sources; conditional GETs.

**Q: How to dedup at scale?**
Bloom filter in front of URL store; periodic compaction.

**Q: How to test at scale?**
Shadow traffic from logs; replay into a non-production frontier.

**Q: How to measure page importance for crawl priority?**
Use signals: PageRank, inbound links, domain authority, update frequency.

**Q: How to handle JavaScript-rendered pages?**
Headless browser (Puppeteer, Playwright) for sub-set of pages; expensive but correct.

---

## 16. End-to-End Diagram

```
 Frontier (DB / Kafka)
 │
 ▼
 Scheduler (per-host fairness)
 │
 ▼
 Workers (fetches + parses)
 │
 ├─► Page Store (S3)
 │ └─► Indexer
 │
 └─► New URLs → Dedup → Frontier
```
