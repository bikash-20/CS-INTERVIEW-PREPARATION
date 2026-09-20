# Design Dropbox / Google Drive (File Storage & Sync)

Tests file storage, sync, conflict resolution, and chunking for efficient large-file handling.

---

## 1. Requirements

### Functional
- Upload / download files from any device.
- Sync across devices when network returns.
- Share files with other users (link / by email).
- Folders, version history.
- Offline edits are merged later.

### Non-Functional
- Sync should be near real-time on good networks.
- Durable — files must never be lost.
- Bandwidth-efficient — don't re-upload unchanged files.
- Support large files (multi-GB).
- Strong consistency per file (no conflicting concurrent edits).

---

## 2. Capacity Estimation

Assume 500M users, 100 GB / user average, 5 GB active per user.

| Metric | Value |
|---|---|
| Total storage | 500M × 100 GB = **50 EB** |
| Active storage (hot) | 500M × 5 GB = **2.5 EB** |
| Daily uploads | ~10% of users edit a file = 50M edits/day ≈ **600 edits/sec** |
| Avg file size | 1 MB |
| Chunk size | 4 MB |

---

## 3. High-Level Design

```
 Client app
 │
 │ sync protocol (long-poll / WebSocket / gRPC stream)
 ▼
 Sync Service (per region)
 │
 ├── Metadata DB (file/folder tree, ownership)
 ├── Chunk Store (S3-like, content-addressed)
 └── Notification Service (changes to push to other devices)
```

Key choices:
- Files split into **chunks** (4 MB).
- Chunks deduplicated by **content hash** (SHA-256).
- Metadata in a transactional DB.
- Chunk blobs in object storage.

---

## 4. Chunking

### Why chunk?
- Reuse identical chunks across files (deduplication).
- Faster uploads: only changed chunks re-upload.
- Smaller delta syncs.

### How to chunk?
- **Fixed-size:** every 4 MB. Simple, but small edits shift chunks.
- **Content-defined chunking (CDC):** find chunk boundaries by rolling hash. Tiny edits only affect one chunk. More complex but great for binaries.

Most products use a hybrid:
- Variable-size chunks (e.g., 4-8 MB) using CDC for large files.
- Whole file as one chunk for small files.

### Chunk address = SHA-256 of bytes
- Two clients uploading the same chunk → only one stored.
- Storage cost savings of 30-50% in real systems.

---

## 5. Metadata

```
Table: files
 file_id BIGINT PK
 user_id BIGINT
 parent_folder_id BIGINT
 name VARCHAR
 mime_type VARCHAR
 size_bytes BIGINT
 current_version_id BIGINT
 created_at, updated_at, deleted_at NULL

Table: chunks
 chunk_hash CHAR(64) PK -- SHA-256 hex
 size_bytes INT
 storage_url TEXT
 ref_count BIGINT -- dedup count

Table: file_chunks
 file_id BIGINT
 version_id BIGINT
 chunk_index INT
 chunk_hash CHAR(64)
 PRIMARY KEY (file_id, version_id, chunk_index)

Table: versions
 version_id BIGINT PK
 file_id BIGINT
 created_at TIMESTAMP
 created_by BIGINT
 size_bytes BIGINT
```

Versioning: each save creates a new version row. Old chunks kept until GC after retention.

---

## 6. Upload Flow

1. Client computes chunk hashes locally.
2. Client sends metadata + list of chunk hashes to server (`manifest`).
3. Server replies with which chunks it doesn't have.
4. Client uploads only missing chunks.
5. Server assembles chunks into a new version, updates metadata.

If the file hasn't changed at all (same hashes), server is a no-op.

---

## 7. Download Flow

1. Client asks for file's current version.
2. Server returns chunk hashes in order.
3. Client downloads only chunks it doesn't have locally.
4. Client reassembles.

---

## 8. Sync Across Devices

Two main approaches:
- **Long-poll / WebSocket push:** server pushes change notifications to connected devices.
- **Periodic poll:** client asks every N seconds.

Both. The push wakes clients quickly; the poll catches missed events.

Server maintains a per-user **notification queue** of recent changes:
- When device A edits → server pushes notification to all other devices of user A.
- When a shared file changes → push to collaborators.

---

## 9. Conflict Resolution

When the same file is edited offline on two devices:

1. Server detects a write conflict (same `file_id`, new version from device B while A is uploading).
2. Options:
   - **Last-write-wins:** simpler but loses data.
   - **Create copy:** rename one to "file (conflict).ext", keep both.
   - **Operational transform / CRDT (Google Docs style):** merge intelligently. Complex.

For binary files, "create copy" is standard.

---

## 10. Sharing

```
Table: shares
 file_id BIGINT
 shared_with_user_id BIGINT OR share_token VARCHAR
 permission ENUM(view, comment, edit)
 PRIMARY KEY (file_id, shared_with_user_id)
```

When a shared file changes, server pushes notifications to all users in the share list.

Public links: generate a `share_token` (URL-safe random); anyone with it gets view access (optionally with password / expiry).

---

## 11. Optimizations

- **Deduplication** at chunk level — huge savings.
- **Compression** on the wire and at rest.
- **Delta sync** — only changed chunks travel.
- **Local cache** on each device — most reads never hit the network.
- **Lazy uploads** — small edits batched for N seconds.
- **CDN** for popular shared files.
- **Pre-fetch** likely-next chunks during downloads.

---

## 12. Security

- TLS everywhere.
- At-rest encryption: chunks encrypted with per-user key.
- Sharing: token-based access with expiry.
- Audit log for compliance (HIPAA / SOC2).

---

## 13. Failure Modes

| Failure | Mitigation |
|---|---|
| Chunk store down | Cached chunks; queue writes; retry. |
| Metadata DB down | Read-only mode. |
| Network drop during upload | Resumable uploads (chunk-level resume). |
| Two devices edit offline | Create conflict copy. |
| Disk full on client | Pause sync; show user. |

---

## 14. API Design

```
POST /api/v1/files (init upload)
 Body: { "parent_id": "...", "name": "...", "size": ..., "chunk_hashes": [...] }
 → 200 { "file_id": "...", "version_id": "...", "missing_chunks": [...] }

PUT /api/v1/chunks/{hash} (upload chunk bytes)
 → 204

GET /api/v1/files/{id}
 → 200 { "metadata": ..., "chunks": [...], "version_id": ... }

GET /api/v1/chunks/{hash}
 → 200 (binary bytes)

POST /api/v1/files/{id}/share
 Body: { "with_user": "...", "permission": "edit" }
 → 201

GET /api/v1/changes?since=cursor
 → 200 { "changes": [...], "next_cursor": "..." } (long-poll supported)
```

---

## 15. Follow-up Questions

**Q: How to handle very large files (10 GB+)?**
Chunked uploads with resumability. Parallel chunk uploads. Manifest check before uploading chunks.

**Q: How to limit storage per user?**
Track total bytes per user in metadata; enforce quota before accepting new chunks.

**Q: How to delete and free storage?**
Mark file deleted (soft). Decrease `ref_count` on chunks. Async GC removes unreferenced chunks.

**Q: How to recover a deleted file?**
Version history retained for N days; user can restore.

**Q: How to implement "available offline" on mobile?**
Client pins chunks to local storage; LRU evicts old.

**Q: How to compress / de-duplicate across users?**
Already dedup at chunk level; per-user encryption breaks this. Optional: client-side encryption with server-stored keys reduces savings.

**Q: How to handle bandwidth throttling on mobile?**
Adaptive chunk size; pause-and-resume; smaller chunked uploads; prioritize metadata over data.

**Q: How to detect / prevent ransomware?**
Version history lets you roll back. Detection: spike in file changes → notify user.

---

## 16. End-to-End Diagram

```
 Device A Device B
 │ (chunked) │ (delta sync)
 ▼ ▼
 Sync Service ─► Metadata DB
 │
 ├──► Chunk Store (S3 / GCS)
 │
 └──► Notification fan-out
 (to other devices of same user + shared users)
```
