# Design Dropbox / Google Drive (File Storage & Sync)

## Requirements

### Functional
- Upload / download files.
- Sync across devices.
- Share files with others.
- Versioning (restore old versions).

### Non-Functional
- Reliability — never lose a file.
- Availability — accessible anywhere.
- Efficient sync — minimal bandwidth usage.

---

## High-Level Design

```
┌──────────────┐ ┌──────────┐ ┌────────────┐
│ Client (App) │────►│ API │────►│ Metadata │
│ + Local Sync │ │ Gateway │ │ Service │
└──────┬───────┘ └────┬─────┘ └─────┬──────┘
 │ │ │
 │ ▼ ▼
 │ ┌──────────┐ ┌──────────┐
 │ │ Chunk │ │ Block │
 │ │ Service │ │ Storage │
 │ └────┬─────┘ │ (S3) │
 │ │ └──────────┘
 │ ▼
 │ ┌──────────┐
 │ │ Dedup DB │
 │ └──────────┘
 │
 └── Watch local folder, sync deltas
```

---

## Key Insight: Chunking + Deduplication

Split files into **chunks** (e.g., 4MB each). Store chunks indexed by **content hash**. Many users' files share chunks → massive storage savings.

### Why?
- A 100MB PowerPoint with mostly identical base content → most chunks already exist → only new chunks uploaded.
- Block-level deduplication → Dropbox reportedly saved 50%+ storage via this.

---

## Components

### 1. Client App
- Watches local folder (using file system events).
- Computes hashes of changed chunks.
- Uploads new chunks; downloads missing ones.

### 2. Chunk Service
- Receives chunk + hash.
- Checks if hash already exists (dedup DB).
- If exists, no storage write — just metadata.
- If not, stores in block storage (S3).

### 3. Metadata Service
- Stores per-user file tree: filename, path, chunk hashes, version, timestamps.
- Database: PostgreSQL or sharded MySQL.
- Each user has a metadata file (like a journal).

### 4. Block Storage
- S3 / GCS / Azure Blob.
- Stores chunks by hash.

### 5. Notification Service
- Tells other devices of a user that files changed (long-poll / WebSocket).

### 6. Sharing Service
- Manages shared links, permissions.

---

## Sync Algorithm

### On File Change (Upload):
1. Client watches folder; detects change.
2. Splits changed file into chunks.
3. Hashes each chunk (SHA-256).
4. Sends list of chunk hashes to server.
5. Server replies with which hashes it already has.
6. Client uploads only missing chunks.
7. Server stores new chunks + updates metadata.

### On Sync (Download):
1. Client queries server for metadata of folder.
2. Compares with local chunks.
3. Downloads missing chunks.
4. Reassembles files.

---

## Conflict Resolution

If same file edited on two devices offline:
- Server creates both versions; last writer wins, OR
- Save both copies: `file (conflicted copy).docx`.

---

## Schema

```sql
CREATE TABLE file_metadata (
 user_id BIGINT,
 file_path TEXT,
 version INT,
 chunk_hashes TEXT[], -- array of chunk hashes
 updated_at TIMESTAMP,
 PRIMARY KEY (user_id, file_path, version)
);

CREATE TABLE chunks (
 chunk_hash CHAR(64) PRIMARY KEY,
 size INT,
 storage_url TEXT
);

CREATE TABLE shares (
 share_id UUID PRIMARY KEY,
 owner_id BIGINT,
 file_path TEXT,
 permission ENUM('READ','WRITE'),
 created_at TIMESTAMP
);
```

---

## Optimizations

1. **Compression** — chunks gzip'd before upload.
2. **Differential sync** — only changed chunks.
3. **LAN sync** — peer devices on same network sync directly.
4. **Lazy loading** — only download chunks when accessed.
5. **CDN** — popular files served from edge.
6. **Encryption** — at-rest (S3 SSE) + in-transit (TLS).

---

## Capacity Estimation

- 500M users
- Avg 50GB / user = 25 EB total raw
- With dedup (5x compression) → ~5 EB
- 10M files added/day; avg 1MB → 10 TB/day new chunks

---

## Follow-up Questions

**Q: How to handle large file uploads (resume on failure)?**
Multipart upload with chunk-level tracking; client resumes from last successful chunk.

**Q: How to ensure durability?**
Replicate chunks across multiple regions; erasure coding for cold storage.

**Q: How to handle deletions?**
Soft delete + tombstones; periodic garbage collection.

**Q: How to share a folder with thousands of users?**
Permission inheritance; check at access time; cached ACL.
