# Design YouTube / Netflix (Video Streaming)

Tests how you handle huge files, encoding, CDN, and adaptive streaming.

---

## 1. Requirements

### Functional
- Upload videos.
- Watch / stream videos.
- Search videos.
- Like, comment, subscribe.
- Recommendations ("Up next").
- Multiple resolutions (360p / 720p / 1080p / 4K).

### Non-Functional
- Massive scale — billions of views/day.
- Low startup latency — video starts in < 2 s.
- Adaptive bitrate — switch quality based on bandwidth.
- Durability — videos must never be lost.

---

## 2. Capacity Estimation

Assume 1B DAU, avg 5 videos/day per user, avg 10 min watch, ~5 Mbps at 1080p.

| Metric | Value |
|---|---|
| Daily views | 5 × 1B = **5 billion/day** |
| Avg view size | 10 min × 5 Mbps × 60 = **3.75 GB** (peak) |
| Daily egress | 5B × 3.75 GB = **~19 EB/day** — clearly CDN-served |
| Upload size | 500M hours / year × 5 GB/h ≈ **2.5 EB/year** new video |
| Storage growth | ~2.5 EB/year raw → + encoded versions = ~5-10× |

---

## 3. High-Level Design

```
 ┌────────┐ upload ┌─────────┐
 │ Client │ ──────► │ API GW │
 └────┬───┘ └────┬────┘
 │ │ upload trigger
 │ ▼
 │ ┌─────────────┐
 │ │ Object Store │ (S3 — raw upload)
 │ └─────┬───────┘
 │ transcode event
 ▼ ▼
 ┌────────────┐ ┌────────────┐ ┌────────────┐
 │ Transcoder │ ───► │ Encoded │ (HLS/DASH segments)
 │ Workers │ │ Storage │
 └────────────┘ └─────┬──────┘
 │
 ▼
 ┌────────────┐
 │ CDN │ (CloudFront / Akamai)
 └─────┬──────┘
 │ manifest + segments
 ▼
 Client (player)
 │
 metadata reads
 ▼
 ┌──────────┐ ┌────────────┐
 │ Video │ │ User │
 │ Service │──│ Service │
 │ (Postgres)│ │ (Postgres)│
 └──────────┘ └────────────┘
 │
 ▼
 Elasticsearch (search)
```

---

## 4. Video Upload Flow

1. Client requests a **presigned upload URL** from API.
2. Client uploads the file directly to **S3 / object storage**.
3. Upload triggers an event ("video.uploaded") to a queue.
4. **Transcoder worker** picks up the file:
   - Probes codec, duration.
   - Re-encodes to multiple resolutions: 360p, 480p, 720p, 1080p, 4K.
   - Each rendition is **chunked** into segments (e.g., 4-10 s).
   - Outputs an **HLS manifest (`.m3u8`)** or **DASH MPD** that lists the segments.
5. Worker uploads encoded segments to CDN-backed storage.
6. Worker updates metadata in DB: title, description, status = `ready`.

### Why HLS / DASH?
- Adaptive bitrate: client picks segments based on bandwidth.
- Better than progressive MP4 because users can switch quality mid-video.

---

## 5. Video Streaming Flow

1. Client opens a video URL.
2. Player calls API: `GET /videos/{id}` → metadata + manifest URL.
3. Player fetches the **HLS manifest** from CDN.
4. Player downloads the first few segments (lowest bitrate), starts playing.
5. Player monitors bandwidth; requests higher-bitrate segments if it can.
6. Player may prefetch the next segment while current plays.

### Why CDN
- Egress is enormous; serving from origin would melt the DB and burn money.
- CDN caches segments at the edge; cold videos still warm when watched.

---

## 6. Storage Architecture

### Raw uploads
- Object store (S3) — durable, cheap.
- Lifecycle policy → move to cold storage after N days.

### Encoded renditions
- Each video: one folder per resolution, each with `segment_000.ts`, `segment_001.ts`, ...
- Plus a manifest file per resolution (and a master manifest).

### Metadata
- Postgres / MySQL: video metadata, owner, status.
- Redis: hot videos, view counts.

### Search index
- Elasticsearch: title, description, tags, transcript.

---

## 7. Database Schema

```
Table: videos
 video_id UUID PK
 uploader_id BIGINT
 title TEXT
 description TEXT
 tags TEXT[]
 duration_sec INT
 status ENUM(uploading, processing, ready, failed)
 visibility ENUM(public, unlisted, private)
 created_at TIMESTAMP
 view_count BIGINT
 like_count BIGINT

Table: video_renditions
 video_id UUID
 resolution VARCHAR (e.g., "1080p")
 bitrate_kbps INT
 codec VARCHAR
 manifest_url TEXT
 segment_count INT
 PRIMARY KEY (video_id, resolution)
```

---

## 8. Adaptive Bitrate Streaming

The player measures:
- Buffer health (how many seconds are buffered ahead).
- Estimated bandwidth from recent downloads.
- Device capabilities.

Then picks the highest rendition where buffer won't empty in < N seconds.

### Why it matters
- Slow connection → 360p segments → no buffering.
- Fast connection → 1080p / 4K segments.

---

## 9. Optimizations

- **CDN everywhere.** Videos are huge; origin only for cache misses.
- **Pre-fetch next segment** in the player.
- **Pre-roll ads** cached at edge.
- **Thumbnails** generated at multiple timestamps; pre-rendered.
- **Lazy view count** — increment in Redis, flush to DB async.
- **Hot video caching** — top 1% of videos in memory at CDN edge.
- **Cold storage** — old videos served from lower-tier CDN or origin-shielded.

---

## 10. API Design

```
POST /api/v1/videos (init upload)
 Body: { "title": "...", "size_bytes": ... }
 → 200 { "video_id": "...", "upload_url": "..." }

PUT <upload_url> (multipart upload to S3)
 → 204

GET /api/v1/videos/{id}
 → 200 { "title": "...", "manifest_url": "...", "renditions": [...] }

GET /api/v1/videos/{id}/comments?cursor=...
POST /api/v1/videos/{id}/like
POST /api/v1/subscriptions/{channel_id}

GET /api/v1/search?q=...
 → 200 { "results": [...] }
```

---

## 11. Failure Modes

| Failure | Mitigation |
|---|---|
| Upload fails mid-way | Resumable uploads (multipart). |
| Transcode fails | Retry with backoff; mark `failed`; user can re-upload. |
| CDN miss for segment | Origin fetches; caches at edge for next viewer. |
| Player stalls | Player falls back to lower resolution; eventually goes to audio-only. |
| View count skew | Sample / aggregate; eventual is fine. |

---

## 12. Follow-up Questions

**Q: How to handle live streaming?**
- Use RTMP ingest → transcode to HLS → push segments to CDN.
- Latency: HLS is ~10-30 s. For low latency, use LL-HLS or WebRTC.

**Q: How to handle copyright (Content ID)?**
- Generate fingerprints (audio/video hashes) of uploads.
- Compare against a reference DB; flag matches.

**Q: How to support "watch later"?**
- Simple list per user; Redis sorted set.

**Q: How to recommend videos?**
- Offline ML on user watch history, similarity.
- Online candidate generation + re-ranking.
- Pre-computed "Up next" list cached per video.

**Q: How to handle 4K / HDR?**
- Encode additional high-bitrate renditions; player picks if device + bandwidth support.

**Q: How to test the transcoder at scale?**
- Re-process existing videos on a schedule to test new pipelines.

**Q: How to secure premium content (DRM)?**
- Use Widevine, FairPlay, or PlayReady.
- Segments are encrypted; player retrieves decryption keys from a license server.

**Q: How to scale comments to billions?**
- Sharded by `video_id`.
- Paginate by cursor.
- Cache hot videos' comment pages in Redis.

---

## 13. End-to-End Diagram

```
 Upload ─► S3 (raw) ─► Queue ─► Transcoder
 │
 │
 Manifests + segments
 │
 ▼
 CDN edges (CloudFront)
 │
 manifest request + segment requests
 │
 ▼
 Player (mobile / web)
 │
 metadata
 │
 ▼
 Postgres (video metadata) ─► Elasticsearch (search)
```
