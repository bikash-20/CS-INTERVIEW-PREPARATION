# Design YouTube / Netflix (Video Streaming)

## Requirements

### Functional
- Upload videos.
- Watch / stream videos.
- Like, comment, subscribe.
- Search, recommendations.

### Non-Functional
- Massive scale (billions of videos watched/day).
- Low-latency playback (start in <1s).
- Adaptive bitrate (different qualities).

---

## Capacity Estimation

- 1B DAU
- Avg watch time: 30 min/day = 1.8B hours/day
- 1.8B hours * 60min = 108B minutes/day
- Storage: 1B videos * avg 1GB = 1 EB (exabyte) — store across many object stores
- Bandwidth: massive — served from CDN

---

## High-Level Design

```
┌────────┐ ┌────────────┐ ┌──────────────┐
│ Client │───────►│ CDN Edge │───────►│ Origin Server│
└────────┘ └──────┬─────┘ └──────────────┘
 ▲ │
 │ ▼
 │ ┌────────────┐
 │ │ Transcoder │ (FFmpeg → multiple bitrates)
 │ └────┬───────┘
 │ │
 │ ▼
 │ ┌─────────────┐
 │ │ Object Store│ (S3)
 │ └─────────────┘
 │
 │ ┌─────────────┐
 └────────────│ Metadata DB │
 │ (Cassandra) │
 └─────────────┘
```

---

## Core Components

### 1. Video Upload Pipeline
1. Client requests **upload URL** (signed, points to S3 / GCS).
2. Client uploads video chunks directly to object store.
3. On complete, a worker triggers **transcoding** (FFmpeg) → multiple bitrates (240p, 360p, 720p, 1080p, 4K).
4. Metadata stored in DB (video_id, title, owner, tags, URLs of all renditions).

### 2. Video Streaming
- Client requests metadata of video_id.
- Server returns URLs for manifest + rendition files.
- Client uses **HLS / DASH** adaptive streaming — downloads manifest + chunks based on bandwidth.
- CDN serves the chunks from nearest edge.

### 3. CDN
- Major cost saver — videos cached at edges worldwide.
- For long-tail (rare videos), use **origin shield** or pull-through cache.

### 4. Adaptive Bitrate Streaming
- Player measures bandwidth, picks appropriate rendition.
- Each rendition split into 4-10s chunks.
- Resilient to network fluctuations.

---

## Schema

```sql
-- Video metadata
video_id BIGINT PRIMARY KEY
user_id BIGINT
title TEXT
description TEXT
duration INT
visibility ENUM('public','private','unlisted')
uploaded_at TIMESTAMP
renditions JSON -- map of {resolution: cdn_url}

-- User
user_id BIGINT PRIMARY KEY
username VARCHAR
subscribers BIGINT

-- View count (denormalized for fast read)
views BIGINT
```

---

## Encoding & Storage

- **Storage tier:**
 - Hot videos (recent, popular) → high-perf SSD
 - Cold videos (old, unpopular) → cheaper object storage (S3 Glacier)

- **Transcoding:** done asynchronously via workers (FFmpeg or hardware-accelerated).

---

## Key Optimizations

1. **Pre-fetch next chunk** based on user behavior.
2. **CDN caching** of popular videos.
3. **Video compression** (H.264/H.265/AV1).
4. **Chunk-based protocol** (HLS/DASH) — no full download needed.
5. **Edge compute** for personalization.

---

## How to Make Money (Side Note)
- Subscriptions
- Ads (server-side ad insertion in stream)
- Pay-per-view

---

## Follow-up Questions

**Q: How to handle live streaming?**
- Use RTMP ingest → transcoder → HLS/DASH output → CDN.
- Lower latency requires WebRTC or LL-HLS.

**Q: How to recommend videos?**
- ML pipeline: collaborative filtering + content-based + ranking.
- Train offline; serve top-N per user.

**Q: How to ensure copyright? (Content ID)**
- Match uploaded video against database of copyrighted fingerprints.
- Allow claims, monetization, or removal.

**Q: How to support offline downloads?**
- DRM-protected local storage on client.

---

## Diagram: Playback Flow

```
1. Client → API: GET /video/{id}
2. API → DB: lookup metadata
3. API → Client: return metadata + CDN URLs for manifest & renditions
4. Client → CDN: GET /manifest.m3u8
5. CDN → Client: manifest (lists all chunks)
6. Client → CDN: GET chunk_001.ts, chunk_002.ts, ...
 (CDN serves from edge cache or fetches from origin)
```
