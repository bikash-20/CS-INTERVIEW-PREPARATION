# DevOps Interview — Complete Preparation Guide

A simple-English, broad-coverage guide to **DevOps interview questions and answers**. Covers Linux, Networking, Git, Docker, Kubernetes, AWS, CI/CD, Terraform, Ansible, Monitoring, Logging, Security, and more. Each topic has theory first, then common interview questions with short answers and code/CLI snippets.

---

## Table of Contents

1. [DevOps Fundamentals](#1-devops-fundamentals)
2. [Linux & Shell Scripting](#2-linux--shell-scripting)
3. [Networking for DevOps](#3-networking-for-devops)
4. [Git & Version Control](#4-git--version-control)
5. [Docker](#5-docker)
6. [Kubernetes](#6-kubernetes)
7. [AWS (Amazon Web Services)](#7-aws-amazon-web-services)
8. [CI/CD (Jenkins, GitHub Actions, GitLab CI)](#8-cicd-jenkins-github-actions-gitlab-ci)
9. [Terraform (Infrastructure as Code)](#9-terraform-infrastructure-as-code)
10. [Ansible (Configuration Management)](#10-ansible-configuration-management)
11. [Monitoring & Logging](#11-monitoring--logging)
12. [Security & DevSecOps](#12-security--devsecops)
13. [Coding / Scenario Questions](#13-coding--scenario-questions)
14. [Quick Revision Cheatsheet](#14-quick-revision-cheatsheet)

---

## 1. DevOps Fundamentals

### Theory

**DevOps** is a culture + set of practices that brings **Development** and **Operations** teams together. The main goal is to ship software **faster, safer, and more reliably**.

Core ideas:
- **Continuous Integration (CI)** — developers merge code often; every merge triggers an automated build + test.
- **Continuous Delivery (CD)** — every green build is *ready* to deploy to production.
- **Continuous Deployment** — automatically pushing every green build to production (no manual gate).
- **Infrastructure as Code (IaC)** — manage servers, networks, and services through code (Terraform, CloudFormation).
- **Monitoring & Feedback** — collect logs and metrics from production to improve the next cycle.
- **Microservices & Containers** — small, independent services packed into containers so they deploy fast.

The **DevOps lifecycle** is sometimes drawn as an **infinity loop** (the "DevOps Loop"):
```
Plan → Code → Build → Test → Release → Deploy → Operate → Monitor → (back to Plan)
```

Other key terms:
- **SRE (Site Reliability Engineering)** — Google's take on DevOps; uses error budgets and SLOs.
- **Shift-left testing** — run tests earlier in the pipeline (security tests, load tests in CI).
- **Mean Time To Recovery (MTTR)** — how fast you fix things in production.
- **Change Lead Time** — time from code committed to code running in prod.

### Questions

**Q1. What is DevOps? Why do we need it?**
DevOps is the union of people, process, and tools to deliver software faster and more reliably. It removes the wall between dev and ops. Benefits: faster releases, fewer failures, faster MTTR, better collaboration.

**Q2. DevOps vs SRE?**
- DevOps is a broad culture (CI/CD, automation, monitoring).
- SRE is a specific implementation: focus on reliability, SLOs, error budgets, blameless post-mortems. SRE is what you get when you "do DevOps with engineering rigor".

**Q3. What is the difference between Continuous Delivery and Continuous Deployment?**
- Continuous Delivery: every commit passes tests and is *deployable*; the actual push to production is manual.
- Continuous Deployment: every green build is *automatically* pushed to production.

**Q4. What is a "shift-left" approach?**
Doing things earlier in the cycle. Example: security testing (SAST/DAST) in CI instead of after release.

**Q5. What are the key DevOps KPIs?**
- Deployment frequency
- Lead time for changes
- Change failure rate
- Mean Time To Recovery (MTTR)
These four are the **DORA metrics**.

**Q6. What is an "immutable infrastructure"?**
Servers are never patched in place. To update, you build a new image/VM and replace the old one. Reduces config drift.

**Q7. Name popular DevOps tools.**
Git, Jenkins, GitHub Actions, GitLab CI, Docker, Kubernetes, Terraform, Ansible, Prometheus, Grafana, ELK, ArgoCD, Helm.

---

## 2. Linux & Shell Scripting

### Theory

Every DevOps engineer must know Linux. Most servers run Linux. You should know:
- File system, permissions, users/groups
- Processes, signals, `systemd`
- Package managers (`apt`, `yum`, `dnf`)
- SSH, users, `sudo`
- Basic networking (`ip`, `ss`, `curl`, `netstat`)
- Text processing (`grep`, `awk`, `sed`, `cut`, `sort`, `uniq`)

A **shell script** (`*.sh`) is a text file with commands run by a shell like `bash`. Bash scripts are used heavily in automation.

### Useful Commands

```bash
# Files & navigation
ls -lah                # list with details
cd /var/log            # change dir
pwd                    # print current dir
cp src dst             # copy
mv src dst             # move/rename
rm -rf folder          # delete recursively (careful!)
mkdir -p a/b/c         # make nested dirs
find / -name "*.log"   # find files

# File content
cat file.txt           # print file
less file.txt          # paginate
head -n 5 file.txt     # first 5 lines
tail -n 20 file.txt    # last 20 lines
tail -f file.log       # follow live (great for logs)

# Permissions
chmod 755 script.sh    # rwx for owner, rx for group/others
chown user:group file  # change owner

# Processes
ps aux                 # all processes
top / htop             # live process viewer
kill -9 PID            # force kill
systemctl status nginx # service status

# Networking
ss -tulpn              # listening ports
curl -I https://x.com  # headers only
ping host              # ICMP test
traceroute host        # path
dig domain.com         # DNS lookup

# Text power tools
grep -ri "error" /var/log/
awk -F, '{print $1}' data.csv      # print first column of CSV
sed -i 's/foo/bar/g' file.txt      # replace inline
sort file | uniq -c | sort -nr    # most common lines
```

### Shell Script Example

```bash
#!/bin/bash
# backup.sh - take a tar backup with timestamp

set -euo pipefail          # fail on error, undefined var, pipe fail

SRC="/var/www"
DEST="/backups"
TS=$(date +"%Y%m%d_%H%M%S")
NAME="backup_${TS}.tar.gz"

mkdir -p "$DEST"
tar -czf "${DEST}/${NAME}" "$SRC"

echo "Backup done: ${DEST}/${NAME}"

# Send to S3 (if aws cli installed)
if command -v aws &>/dev/null; then
  aws s3 cp "${DEST}/${NAME}" "s3://my-bucket/backups/"
fi
```

### Questions

**Q1. `chmod 755` — what does each digit mean?**
- `7` = `rwx` (read+write+execute) — owner
- `5` = `r-x` — group
- `5` = `r-x` — others
Each digit is `4 (r) + 2 (w) + 1 (x)`.

**Q2. How do you find which process is using port 8080?**
```bash
ss -tulpn | grep 8080
# or
lsof -i :8080
```

**Q3. How to check disk usage?**
```bash
df -h              # per filesystem
du -sh /var/log/*  # per folder
```

**Q4. What is the difference between `>` and `>>` in shell?**
- `>` overwrites a file.
- `>>` appends to a file.

**Q5. What does `set -euo pipefail` do in bash?**
- `-e` exit on error.
- `-u` error on undefined variable.
- `-o pipefail` catch errors inside pipes (not just the last command).

**Q6. How to run a command in the background and not die when shell exits?**
Use `nohup` + `&`:
```bash
nohup ./long_script.sh > out.log 2>&1 &
```
Or use a process manager / `systemd` service.

**Q7. How to schedule a job?**
Use `cron`. Edit with `crontab -e`:
```
*/5 * * * *  /home/user/backup.sh >> /home/user/backup.log 2>&1
```
This runs every 5 minutes.

**Q8. `grep`, `egrep`, `fgrep`?**
- `grep` — basic regex.
- `egrep` (or `grep -E`) — extended regex.
- `fgrep` (or `grep -F`) — fixed string, no regex.

**Q9. What is an inode?**
A data structure that stores info about a file (permissions, owner, timestamps, disk block pointers). Each file has an inode. `ls -i` shows inode numbers.

**Q10. Soft link vs Hard link?**
- **Soft (symbolic) link** — pointer to a filename; breaks if the target is deleted. Like a shortcut.
- **Hard link** — another name for the same inode; still works even if the original is deleted (until all links removed).
```bash
ln -s /path/to/file mylink     # soft
ln /path/to/file hardlink      # hard
```

---

## 3. Networking for DevOps

### Theory

DevOps engineers debug a lot of networking issues. You need to know the **OSI model**, **TCP/IP**, **DNS**, **HTTP**, **firewalls**, **load balancers**, and **TLS**.

Key concepts:
- **TCP vs UDP** — TCP is reliable, ordered, slower. UDP is fast, no guarantee.
- **3-way handshake** — `SYN → SYN-ACK → ACK`.
- **DNS** — translates names to IPs. Records: `A`, `AAAA`, `CNAME`, `MX`, `TXT`, `NS`.
- **HTTP methods** — `GET`, `POST`, `PUT`, `PATCH`, `DELETE`, `OPTIONS`.
- **Status codes** — `200`, `201`, `204`, `301`, `302`, `400`, `401`, `403`, `404`, `500`, `502`, `503`.
- **TLS / HTTPS** — encrypts traffic using certificates (Let's Encrypt is free).
- **Reverse proxy** — server that sits in front of backend servers (e.g., NGINX).
- **Load balancer** — distributes traffic across many servers.
- **CIDR** — `192.168.1.0/24` means 256 addresses.

### Common Ports

| Port | Service |
|---|---|
| 22 | SSH |
| 53 | DNS |
| 80 | HTTP |
| 443 | HTTPS |
| 25 | SMTP |
| 3306 | MySQL |
| 5432 | PostgreSQL |
| 6379 | Redis |
| 27017 | MongoDB |
| 8080 | Alt HTTP (tomcat/dev) |

### Commands

```bash
curl -v https://example.com    # verbose, shows headers and TLS
nslookup example.com          # DNS lookup
dig +short example.com         # short DNS answer
traceroute 8.8.8.8             # path to host
tcpdump -i eth0 port 80       # capture packets
ss -s                         # socket summary
ip a                          # show interfaces
ip route                      # show routing table
```

### Questions

**Q1. What happens when you type a URL in a browser?**
1. Browser checks cache.
2. DNS lookup (browser cache → OS → resolver → root → TLD → authoritative).
3. TCP handshake (3-way).
4. TLS handshake (for HTTPS).
5. HTTP request sent.
6. Server responds.
7. Browser renders HTML, requests CSS/JS/images, repeats for each.

**Q2. TCP vs UDP — when to use which?**
- TCP: web (HTTP), email, file transfer — when reliability matters.
- UDP: video streaming, VoIP, DNS, gaming — when speed matters more than perfect delivery.

**Q3. What is a reverse proxy? Why use one?**
A server that sits in front of backend servers and forwards client requests. Reasons: TLS termination, caching, compression, rate limiting, load balancing, hiding internal IPs.

**Q4. What is a load balancer? L4 vs L7?**
- **L4** — balances based on IP/port (TCP level). Fast.
- **L7** — balances based on URL, headers, cookies (HTTP level). More flexible.

**Q5. What is DNS TTL?**
Time To Live — how long a resolver caches the answer. Lower TTL = faster changes, higher load. Higher TTL = faster lookups, slower changes.

**Q6. HTTPS — how does TLS work (in short)?**
1. Client Hello (supported ciphers, random bytes).
2. Server Hello + certificate.
3. Client verifies cert with CA.
4. Key exchange (RSA or ECDHE).
5. Both sides derive a session key. All further traffic is encrypted.

**Q7. What is a subnet?**
A range of IP addresses defined by CIDR. `10.0.0.0/16` = 65,536 addresses. `10.0.0.0/24` = 256 addresses.

**Q8. What is the difference between private and public IP?**
- Private: used inside a LAN, not routable on the internet (`10.x`, `172.16-31.x`, `192.168.x`).
- Public: routable on the internet.

---

## 4. Git & Version Control

### Theory

Git is a **distributed version control system**. Every clone has the full history. You must be fluent in `git` for any DevOps / SDE interview.

Main areas:
- Local workflow: working dir → `git add` → staging → `git commit` → local repo.
- Remote workflow: `git push`, `git pull`, `git fetch`.
- Branches, merges, rebases.
- Conflict resolution.
- Tags and releases.
- Common collaboration models: GitFlow, trunk-based development.

### Commands

```bash
git init                    # init new repo
git clone <url>             # clone
git status                  # current changes
git add file                # stage file
git add -A                  # stage all
git commit -m "msg"         # commit
git log --oneline           # short log
git branch                  # list branches
git checkout -b feat/x      # create + switch
git switch main             # switch
git merge feat/x            # merge branch
git rebase main             # rebase current onto main
git push origin main        # push
git pull --rebase           # pull + rebase
git stash                   # save changes aside
git stash pop               # restore changes
git reset --hard HEAD~1     # undo last commit (dangerous!)
git revert <sha>            # safe undo (creates new commit)
git tag v1.0.0              # create tag
```

### Questions

**Q1. `git pull` vs `git fetch`?**
- `fetch` — downloads remote changes but does not merge.
- `pull` = `fetch` + `merge` (or rebase if configured).

**Q2. `git merge` vs `git rebase`?**
- `merge` — creates a merge commit, keeps history.
- `rebase` — replays your commits on top of the target branch, makes a linear history. **Never rebase commits that others have.**

**Q3. How to undo a commit?**
- If not pushed: `git reset --soft HEAD~1` (keeps changes staged) or `git reset --hard HEAD~1` (discards).
- If pushed: `git revert <sha>` (creates a new commit that undoes).

**Q4. What is a "detached HEAD"?**
When `HEAD` points to a commit instead of a branch. Happens with `git checkout <sha>`. You can still work but new commits are not on any branch unless you create one.

**Q5. What is a "conflict"? How to resolve?**
Git cannot auto-merge because both sides changed the same lines. You open the file, keep the right lines, remove conflict markers (`<<<<<<<`, `=======`, `>>>>>>>`), then `git add` + `git commit`.

**Q6. How do you squash the last N commits?**
```bash
git rebase -i HEAD~N
# mark all but the first as "squash" (s)
```

**Q7. What is a `.gitignore`?**
A file that lists patterns Git should **not** track (build outputs, secrets, IDE files).

**Q8. GitFlow vs trunk-based development?**
- **GitFlow** — long-lived branches: `main`, `develop`, `feature/*`, `release/*`, `hotfix/*`. Good for versioned releases.
- **Trunk-based** — short-lived branches off `main`, merged quickly (often with feature flags). Encouraged for continuous deployment.

---

## 5. Docker

### Theory

**Docker** packages an app + its dependencies into a **container** — a lightweight, isolated, portable unit that runs the same on any machine that has Docker.

- **Image** — read-only template (like a class).
- **Container** — running instance of an image (like an object).
- **Dockerfile** — recipe to build an image.
- **Layer** — each Dockerfile instruction creates a layer; layers are cached and shared.
- **Registry** — stores images (Docker Hub, ECR, GCR, GHCR).
- **Volume** — persistent storage outside a container's filesystem.
- **Network** — virtual network for containers to talk to each other.
- **Compose** — tool to run multi-container apps from a YAML file.

Why Docker?
- Same env from dev to prod.
- Fast startup (seconds vs minutes for VMs).
- Easy to scale.
- Smaller footprint than VMs.

### Dockerfile Example

```dockerfile
# syntax=docker/dockerfile:1.6

# ---- build stage ----
FROM node:20-alpine AS build
WORKDIR /app
COPY package*.json ./
RUN npm ci
COPY . .
RUN npm run build

# ---- runtime stage ----
FROM nginx:alpine
COPY --from=build /app/dist /usr/share/nginx/html
EXPOSE 80
CMD ["nginx", "-g", "daemon off;"]
```

### Docker CLI

```bash
docker build -t myapp:1.0 .          # build image
docker run -d -p 8080:80 myapp:1.0   # run detached, port map
docker ps                            # running containers
docker ps -a                         # all containers
docker logs -f myapp                 # follow logs
docker exec -it myapp sh             # shell into container
docker stop myapp                    # stop container
docker rm myapp                      # remove container
docker images                        # list images
docker rmi myapp:1.0                 # remove image
docker pull nginx:latest             # pull from registry
docker push myrepo/myapp:1.0         # push to registry
docker volume ls                     # list volumes
docker network ls                    # list networks
```

### docker-compose Example

```yaml
# docker-compose.yml
version: "3.9"

services:
  web:
    build: .
    ports:
      - "8080:80"
    depends_on:
      - api
    environment:
      - API_URL=http://api:3000

  api:
    image: myrepo/api:1.0
    environment:
      - DB_URL=postgres://db:5432/app
    depends_on:
      - db

  db:
    image: postgres:16
    volumes:
      - pgdata:/var/lib/postgresql/data
    environment:
      POSTGRES_PASSWORD: example

volumes:
  pgdata:
```

Run with: `docker compose up -d`.

### Questions

**Q1. Image vs Container?**
- Image: blueprint (immutable, layers).
- Container: a running instance of an image.

**Q2. What is a multi-stage Dockerfile? Why use it?**
Combines multiple `FROM` statements. First stage builds, second stage copies only the artifacts. Result: **smaller, more secure final image** (no compiler, no source code in production).

**Q3. CMD vs ENTRYPOINT?**
- `CMD` — default command, easy to override (`docker run image echo hi`).
- `ENTRYPOINT` — the executable; arguments are appended. Harder to override.
Best practice: `ENTRYPOINT ["python", "app.py"]` + `CMD ["--port", "8080"]`.

**Q4. How do you persist data in Docker?**
- **Volumes** — managed by Docker (`docker volume create`).
- **Bind mounts** — a host path mounted in the container (good for dev).
- **tmpfs** — in-memory, lost on stop.

**Q5. Difference between `COPY` and `ADD`?**
`COPY` copies files. `ADD` does the same **plus** can fetch URLs and extract local tar files. Use `COPY` unless you need those extras (keeps Dockerfile clear).

**Q6. How to reduce image size?**
- Use small base image (`alpine`, `distroless`).
- Multi-stage builds.
- Clean caches in the same `RUN` (`RUN apt-get update && apt-get install -y ... && rm -rf /var/lib/apt/lists/*`).
- `.dockerignore` to exclude `node_modules`, `.git`, etc.

**Q7. How do containers communicate with each other?**
- Same **network** (default `bridge`).
- Use **service name as hostname** when using Compose.
- Or join multiple containers to a user-defined network.

**Q8. What is the difference between a Docker volume and a bind mount?**
- Volume: stored in Docker's area, easy to back up and move.
- Bind mount: maps a host path directly, useful in dev.

**Q9. How to check why a container keeps restarting?**
```bash
docker ps -a                 # see STATUS / restart count
docker logs myapp            # app logs
docker inspect myapp         # metadata
# Look at "Last State", exit code, OOMKilled
```

**Q10. What is `docker-compose` used for?**
Run a multi-container app defined in a YAML file with one command (`docker compose up`).

**Q11. Container vs VM?**
- VM: full OS, hypervisor, GBs, slow boot.
- Container: shares host kernel, MBs, second boot. Less isolation.

**Q12. How do you scan an image for vulnerabilities?**
Tools: `docker scan`, `trivy`, `grype`, `snyk`. Example:
```bash
trivy image myapp:1.0
```

---

## 6. Kubernetes

### Theory

**Kubernetes (k8s)** is an open-source container orchestrator. It deploys, scales, and manages containers across many machines (a **cluster**).

Key concepts:
- **Cluster** = Control Plane + Worker Nodes.
- **Node** — a VM or physical machine that runs pods.
- **Pod** — smallest unit; one or more tightly-coupled containers sharing network and storage.
- **Deployment** — manages a ReplicaSet of identical pods; supports rolling updates.
- **Service** — stable network endpoint for a set of pods.
- **Ingress** — HTTP(S) routing into the cluster.
- **ConfigMap** — non-secret config as key-value pairs.
- **Secret** — base64-encoded sensitive data.
- **Namespace** — virtual cluster for isolation.
- **kubectl** — CLI to talk to the API server.
- **kubectl apply -f file.yaml** — declarative management.
- **Helm** — package manager (charts) for k8s.
- **kubectl rollout** — manage rollouts and rollbacks.

### Pod YAML Example

```yaml
apiVersion: v1
kind: Pod
metadata:
  name: hello-pod
  labels:
    app: hello
spec:
  containers:
    - name: hello
      image: myrepo/hello:1.0
      ports:
        - containerPort: 8080
      env:
        - name: LOG_LEVEL
          value: "info"
      resources:
        requests:
          cpu: "100m"
          memory: "128Mi"
        limits:
          cpu: "500m"
          memory: "256Mi"
      readinessProbe:
        httpGet:
          path: /healthz
          port: 8080
```

### Deployment YAML Example

```yaml
apiVersion: apps/v1
kind: Deployment
metadata:
  name: web
spec:
  replicas: 3
  selector:
    matchLabels:
      app: web
  template:
    metadata:
      labels:
        app: web
    spec:
      containers:
        - name: web
          image: myrepo/web:1.0
          ports:
            - containerPort: 80
```

### Service YAML Example

```yaml
apiVersion: v1
kind: Service
metadata:
  name: web-svc
spec:
  selector:
    app: web
  ports:
    - port: 80
      targetPort: 80
      nodePort: 30080
  type: NodePort
```

### Common kubectl Commands

```bash
kubectl get pods                          # list pods
kubectl get pods -A                       # all namespaces
kubectl describe pod hello                # details + events
kubectl logs hello                        # logs
kubectl logs hello -c container           # multi-container pod
kubectl exec -it hello -- sh              # shell in
kubectl apply -f deployment.yaml          # apply manifest
kubectl delete -f deployment.yaml         # delete
kubectl rollout status deploy/web         # status
kubectl rollout undo deploy/web           # rollback
kubectl scale deploy/web --replicas=5     # scale
kubectl get svc                           # services
kubectl get nodes                         # nodes
kubectl top pods                          # CPU/mem usage
kubectl config use-context prod           # switch context
kubectl get events --sort-by=.metadata.creationTimestamp
```

### Helm Example (values.yaml snippet)

```yaml
replicaCount: 3
image:
  repository: myrepo/web
  tag: "1.0.0"
service:
  type: ClusterIP
  port: 80
resources:
  limits:
    cpu: 500m
    memory: 256Mi
```

```bash
helm install myweb ./mychart -f values.yaml
helm upgrade myweb ./mychart -f values.yaml
helm rollback myweb 1
```

### Questions

**Q1. What is a Pod?**
Smallest deployable unit. Holds 1+ containers that share network namespace and storage. Usually 1 container per pod.

**Q2. Deployment vs StatefulSet?**
- Deployment — for stateless apps (web servers, APIs). Pods are interchangeable.
- StatefulSet — for stateful apps (databases). Pods have stable names and persistent storage.

**Q3. Service types?**
- **ClusterIP** — internal only (default).
- **NodePort** — exposes on each node's port.
- **LoadBalancer** — provisions an external LB (cloud).
- **ExternalName** — DNS alias to an external service.

**Q4. What is an Ingress?**
HTTP(S) routing rules for external traffic into the cluster. One Ingress can route by host or path to many services.

**Q5. ConfigMap vs Secret?**
Both inject config into pods as env vars or files. **ConfigMap** is for plain data. **Secret** is for sensitive data (base64-encoded by default).

**Q6. livenessProbe vs readinessProbe?**
- **liveness** — pod is alive? If fails, kubelet restarts the pod.
- **readiness** — pod ready to serve traffic? If fails, Service removes it from endpoints.

**Q7. How do you roll out a new version safely?**
Use a `Deployment` with a rolling update strategy:
```yaml
strategy:
  type: RollingUpdate
  rollingUpdate:
    maxUnavailable: 0
    maxSurge: 1
```
This gradually replaces old pods with new ones.

**Q8. How do you rollback?**
```bash
kubectl rollout undo deploy/web
# or to a specific revision
kubectl rollout undo deploy/web --to-revision=3
```

**Q9. What is a Namespace?**
Virtual cluster. Great for separating teams (dev/staging/prod) or environments. Use `kubectl -n NAMESPACE`.

**Q10. HorizontalPodAutoscaler (HPA)?**
Scales pods based on metrics (CPU, memory, custom). Example:
```bash
kubectl autoscale deploy/web --min=2 --max=10 --cpu-percent=70
```

**Q11. What is a node?**
A worker machine (VM or bare metal) that runs pods. Managed by the kubelet.

**Q12. What is `kubectl`?**
The official CLI to talk to the cluster's API server.

**Q13. What does the control plane do?**
Decides what runs where. Components: `kube-apiserver`, `etcd`, `kube-scheduler`, `kube-controller-manager`, `cloud-controller-manager`.

**Q14. What is `etcd`?**
A distributed key-value store that holds all cluster state (config, secrets, what runs where).

**Q15. What is a Sidecar container?**
A second container in the same Pod that supports the main one (e.g., log shipper, proxy, init helper).

**Q16. What is `kustomize`?**
A tool to manage YAML differences per environment without templating. Use case: same base Deployment, different replica counts per env.

**Q17. Names of common probes?**
`httpGet`, `tcpSocket`, `exec` (runs a command).

---

## 7. AWS (Amazon Web Services)

### Theory

**AWS** is the most-used cloud platform. DevOps interviews expect you to know the **core services** and how they fit together. Focus on: compute, storage, networking, database, identity, monitoring, IaC.

### Core Services Cheat-sheet

| Category | Services | Notes |
|---|---|---|
| **Compute** | EC2, ECS, EKS, Lambda, Fargate | VMs, containers, serverless |
| **Storage** | S3, EBS, EFS, Glacier | Object, block, file, archive |
| **Database** | RDS (Postgres/MySQL/...), DynamoDB, Aurora, ElastiCache | Managed SQL, NoSQL, in-memory |
| **Networking** | VPC, Subnet, IGW, NAT GW, Route Table, ALB, NLB, CloudFront, Route 53 | Hub & spoke |
| **IAM** | Users, Roles, Policies, Groups | Identity & access |
| **Security** | KMS, Secrets Manager, WAF, Shield | Encryption, WAF |
| **Messaging** | SQS, SNS, EventBridge, Kinesis | Queues, pub/sub, events |
| **Monitoring** | CloudWatch, CloudTrail, X-Ray | Metrics, logs, audit |
| **IaC** | CloudFormation, CDK | AWS-native templates |
| **Other** | API Gateway, Step Functions, Cognito | Glue for serverless |

### S3 Example (CLI)

```bash
aws s3 mb s3://my-bucket                    # make bucket
aws s3 cp file.txt s3://my-bucket/         # upload
aws s3 sync ./build s3://my-bucket/build/   # sync folder
aws s3 presign s3://my-bucket/file.txt      # presigned URL
```

### EC2 Quick Notes
- **Instance types** — `t3.micro` (burstable), `m5.large` (general), `c5.xlarge` (compute).
- **AMI** — image used to launch.
- **Key pair** — SSH key.
- **Security group** — virtual firewall for the instance.
- **User data** — bootstrap script that runs at first launch.

### VPC Layout (typical)
```
Internet
   |
 [IGW]
   |
 [Public Subnet] -- (NAT GW) --> [Private Subnet] --> DB Subnet
        |                              |                  |
   Bastion Host                  App Servers         RDS / Aurora
```

### IAM Policy Example

```json
{
  "Version": "2012-10-17",
  "Statement": [
    {
      "Effect": "Allow",
      "Action": ["s3:GetObject", "s3:PutObject"],
      "Resource": "arn:aws:s3:::my-bucket/*"
    }
  ]
}
```

### Questions

**Q1. EC2 vs Lambda vs ECS vs EKS?**
- EC2 — virtual machine, you manage OS.
- Lambda — serverless function, max 15 min, pay per ms.
- ECS — managed container service (Amazon's Docker orchestrator).
- EKS — managed Kubernetes.

**Q2. S3 storage classes?**
- **Standard** — frequent access.
- **Intelligent-Tiering** — auto moves objects.
- **Standard-IA** — infrequent, fast retrieval.
- **One Zone-IA** — single AZ, cheaper.
- **Glacier / Glacier Deep Archive** — archive, slow retrieval.

**Q3. S3 eventual consistency?**
For overwrite/delete of an existing key, takes a moment to propagate. Reads might briefly see the old version. Bucket-level operations are strongly consistent since 2020.

**Q4. What is an IAM Role?**
An identity with permissions that AWS services or users can **assume**. Best practice: give EC2 a role instead of access keys.

**Q5. Security group vs NACL?**
- **Security Group** — instance-level, stateful (return traffic auto-allowed), default allow.
- **NACL** — subnet-level, stateless (must allow inbound AND outbound), default allow; can deny explicitly.

**Q6. How do you keep EC2 credentials off the machine?**
Use an **IAM Instance Profile** (role attached to EC2). SDK picks up creds automatically. Or use **SSM Session Manager** to SSH without keys.

**Q7. What is a NAT Gateway?**
Sits in a public subnet. Lets **private** subnets reach the internet for outbound (e.g., apt install) without being reachable from the internet.

**Q8. ALB vs NLB vs CLB?**
- **ALB** (Application) — L7, HTTP/HTTPS, path/host routing.
- **NLB** (Network) — L4, TCP/UDP, ultra-low latency, static IPs.
- **CLB** (Classic) — legacy, avoid.

**Q9. How do you make a static website on S3?**
1. Create bucket with the domain name.
2. Enable "Static website hosting" in properties.
3. Upload `index.html`, `error.html`.
4. Set bucket policy to allow `s3:GetObject` for `*`.
5. Optional: put CloudFront in front, then Route 53 alias.

**Q10. AWS regions and AZs?**
- **Region** — geographical area (e.g., `us-east-1`).
- **Availability Zone** — one or more data centers in a region. Multiple AZs per region.

**Q11. What is CloudWatch?**
Monitoring service — metrics, logs, alarms, dashboards. Each EC2 sends CPU by default; install the **CloudWatch agent** for memory, disk, logs.

**Q12. CloudTrail vs CloudWatch?**
- **CloudTrail** — records API calls (who did what and when). Audit log.
- **CloudWatch** — runtime metrics and logs.

**Q13. How to encrypt data at rest in AWS?**
- S3: SSE-S3 (AES-256) or SSE-KMS.
- EBS: encryption flag on volume.
- RDS: encryption flag when creating.
- Use **KMS** to manage keys.

**Q14. RDS Multi-AZ vs Read Replica?**
- **Multi-AZ** — synchronous standby in another AZ, **failover**. One DNS endpoint.
- **Read Replica** — async copy for reads (scale reads). You update the read endpoint.

**Q15. How to give a Lambda access to S3?**
Add an IAM role to the Lambda with `s3:GetObject` permission.

**Q16. What is a VPC peering?**
A network connection between two VPCs (private IPs route). Non-transitive.

**Q17. Difference between SQS and SNS?**
- **SQS** — queue (pull).
- **SNS** — pub/sub (push, fanout).
- Combine for fan-out to multiple workers.

---

## 8. CI/CD (Jenkins, GitHub Actions, GitLab CI)

### Theory

**CI/CD** = automation that builds, tests, and deploys your code.

- **CI (Continuous Integration)** — every merge runs build + tests.
- **CD (Continuous Delivery / Deployment)** — every green build can be (or is automatically) deployed.

Tools differ in config style:
- **Jenkins** — Groovy pipelines, very flexible, self-hosted.
- **GitHub Actions** — YAML workflows in `.github/workflows/`, hosted by GitHub.
- **GitLab CI** — `.gitlab-ci.yml`, YAML, hosted or self-hosted.
- **CircleCI**, **Bitbucket Pipelines** — similar YAML.

A **pipeline** typically has stages:
```
lint → build → test → security scan → package → deploy (dev/stage/prod)
```

### GitHub Actions Example

```yaml
# .github/workflows/ci.yml
name: CI

on:
  push:
    branches: [main]
  pull_request:
    branches: [main]

jobs:
  build:
    runs-on: ubuntu-latest

    steps:
      - uses: actions/checkout@v4

      - name: Set up Node
        uses: actions/setup-node@v4
        with:
          node-version: 20

      - name: Install
        run: npm ci

      - name: Lint
        run: npm run lint

      - name: Test
        run: npm test -- --ci

      - name: Build
        run: npm run build

      - name: Build Docker image
        run: docker build -t myapp:${{ github.sha }} .

      - name: Push to ECR
        if: github.ref == 'refs/heads/main'
        run: |
          aws ecr get-login-password --region us-east-1 | \
          docker login --username AWS --password-stdin $ECR_REGISTRY
          docker push $ECR_REGISTRY/myapp:${{ github.sha }}
```

### Jenkins Pipeline Example (declarative)

```groovy
pipeline {
  agent any
  stages {
    stage('Checkout') { steps { git 'https://github.com/me/app.git' } }
    stage('Build')    { steps { sh 'npm ci && npm run build' } }
    stage('Test')     { steps { sh 'npm test' } }
    stage('Deploy')   { steps { sh './deploy.sh' } }
  }
  post {
    failure { mail to: 'team@x.com', subject: "Build failed: ${env.JOB_NAME}" }
  }
}
```

### GitLab CI Example

```yaml
# .gitlab-ci.yml
stages: [test, build, deploy]

test:
  image: node:20
  stage: test
  script:
    - npm ci
    - npm test

build:
  stage: build
  script:
    - docker build -t myapp:$CI_COMMIT_SHA .
    - docker push myapp:$CI_COMMIT_SHA

deploy:
  stage: deploy
  script:
    - kubectl set image deploy/web web=myapp:$CI_COMMIT_SHA
  only:
    - main
```

### Questions

**Q1. What is the difference between a "stage" and a "step/job"?**
- Stage — logical phase (test, build, deploy).
- Job/Step — the actual unit that runs in a stage.

**Q2. How do you handle secrets in CI?**
- GitHub: **Secrets** in repo settings.
- GitLab: **CI/CD variables** (masked, protected).
- Jenkins: **Credentials** plugin.
Never echo secrets in logs.

**Q3. What is a "matrix" / "strategy matrix" in CI?**
Run the same job on multiple configurations (Node 18 + Node 20, multiple OS, etc.).

**Q4. What is a "runner"?**
The agent (VM or container) that runs the jobs. Self-hosted runners give more control but need to be maintained.

**Q5. How do you speed up CI?**
- Cache dependencies (`actions/cache`).
- Run jobs in parallel.
- Smaller Docker images.
- Use matrix wisely.
- Skip unchanged paths (`paths-ignore`, `dorny/paths-filter`).

**Q6. What is "deployment gating"?**
Add a manual approval step between staging and production, so a human clicks "approve" before prod.

**Q7. Blue/Green vs Canary deployment?**
- **Blue/Green** — two identical environments; cut traffic over; rollback = switch back.
- **Canary** — route 5% of traffic to the new version, monitor, then ramp to 100%.

---

## 9. Terraform (Infrastructure as Code)

### Theory

**Terraform** by HashiCorp lets you describe infrastructure as code in HCL (HashiCorp Configuration Language). You write `.tf` files; Terraform plans, then applies the changes.

Workflow:
```
init → plan → apply → (destroy)
```
- `init` — downloads providers and modules.
- `plan` — shows what will change.
- `apply` — performs the change.
- `state` — file that tracks what exists.

### Example (AWS S3 + EC2)

```hcl
provider "aws" {
  region = "us-east-1"
}

resource "aws_s3_bucket" "site" {
  bucket = "my-static-site-${var.env}"
}

resource "aws_s3_bucket_public_access_block" "site" {
  bucket = aws_s3_bucket.site.id
  block_public_acls       = true
  block_public_policy     = true
  ignore_public_acls      = true
  restrict_public_buckets = true
}

resource "aws_instance" "web" {
  ami           = "ami-0abcdef1234567890"
  instance_type = "t3.micro"
  tags = {
    Name = "web-${var.env}"
  }
}

variable "env" {
  type    = string
  default = "dev"
}

output "instance_ip" {
  value = aws_instance.web.public_ip
}
```

```bash
terraform init
terraform plan -out plan.tfplan
terraform apply plan.tfplan
terraform destroy
```

### State Commands

```bash
terraform state list
terraform state show aws_instance.web
terraform state mv old new       # rename in state without recreating
terraform import aws_s3_bucket.x my-bucket
```

### Questions

**Q1. Terraform vs CloudFormation?**
- Terraform — multi-cloud, HCL, large community.
- CloudFormation — AWS-only, JSON/YAML, deep AWS integration.

**Q2. What is the Terraform state? Why store it remotely?**
The state tracks the real-world resources Terraform manages. Storing remotely (S3 + DynamoDB lock) allows team collaboration and prevents two people applying at once.

**Q3. `terraform plan` vs `apply`?**
`plan` is dry-run — shows diff. `apply` actually makes changes. Always plan first.

**Q4. What is a "module"?**
A reusable `.tf` package you can call multiple times (e.g., one VPC module reused per environment).

**Q5. State locking — why?**
So two engineers don't apply at the same time. Use S3 + DynamoDB, or Terraform Cloud.

**Q6. How do you handle secrets in Terraform?**
Use environment variables (`TF_VAR_db_password`), AWS Secrets Manager, HashiCorp Vault. Never commit plain secrets.

**Q7. `count` vs `for_each`?**
- `count` — index-based, count from 0..N. Brittle when the list changes (re-creates everything).
- `for_each` — uses a map or set, stable.

**Q8. Drift?**
Drift = real infra differs from Terraform state. Detect with `terraform plan`. Fix by either updating infra back to match, or running `terraform apply -refresh-only` then `apply`.

---

## 10. Ansible (Configuration Management)

### Theory

**Ansible** automates configuration, package installs, and deploys across many machines over SSH. No agents needed — just Python on the target and SSH.

Core concepts:
- **Inventory** — list of hosts (`hosts.ini` or dynamic).
- **Playbook** — YAML file with tasks.
- **Task** — single action (run a command, install a package, copy a file).
- **Role** — reusable folder of playbooks, templates, files.
- **Module** — the actual unit of work (`apt`, `copy`, `service`).
- **Idempotency** — running the same playbook twice has the same end state.

### Simple Playbook

```yaml
# site.yml
- name: Configure webservers
  hosts: web
  become: true
  tasks:
    - name: Install nginx
      ansible.builtin.apt:
        name: nginx
        state: present
        update_cache: yes

    - name: Copy index.html
      ansible.builtin.copy:
        src: files/index.html
        dest: /var/www/html/index.html
        mode: "0644"

    - name: Ensure nginx is running
      ansible.builtin.service:
        name: nginx
        state: started
        enabled: true
```

```bash
ansible-playbook -i hosts.ini site.yml
ansible all -i hosts.ini -m ping
```

### Questions

**Q1. Push vs Pull configuration management?**
- **Push** — Ansible, SSH from a control node.
- **Pull** — Chef, Puppet, Salt agents on each box poll a server.

**Q2. What is idempotency? Why important?**
A play should produce the same result no matter how many times it runs. Lets you rerun safely. `apt: state=present`, not `command: apt install -y`.

**Q3. Ansible vs Terraform?**
- Ansible — config / app state on existing machines.
- Terraform — provisions the **infrastructure** itself. They complement each other.

**Q4. What is a "handler"?**
Task that runs only when notified (e.g., restart nginx when config changes). Defined with `notify`, triggered by handlers.

**Q5. How to keep secrets out of playbooks?**
Use **Ansible Vault**:
```bash
ansible-vault encrypt_string 'my_pass' --name 'db_password'
```
Or use a vars file encrypted with `ansible-vault encrypt`.

---

## 11. Monitoring & Logging

### Theory

Two big pillars of observability:
- **Metrics** — numbers over time (CPU, requests/sec, latency).
- **Logs** — discrete events.
- **Traces** — a single request's path across services.

The three are often called the **three pillars of observability** (metrics, logs, traces).

Popular stacks:
- **Metrics**: Prometheus + Grafana.
- **Logs**: ELK (Elasticsearch + Logstash + Kibana), EFK (Fluentd), Loki.
- **Traces**: Jaeger, Zipkin, OpenTelemetry.

Key terms:
- **SLI** — Service Level Indicator (e.g., latency).
- **SLO** — Service Level Objective (target, e.g., 99.9% under 200 ms).
- **SLA** — Service Level Agreement (contract with customer).
- **Error budget** — `1 - SLO`; how much failure is allowed.

### Prometheus + Grafana

Prometheus pulls metrics from `/metrics` endpoints (pull model), stores, and alerts. Grafana draws dashboards from many sources (including Prometheus).

```yaml
# prometheus.yml (snippet)
scrape_configs:
  - job_name: "node"
    static_configs:
      - targets: ["localhost:9100"]
```

```promql
# PromQL examples
up                                     # 1 if target is up
rate(http_requests_total[5m])          # request rate per second
histogram_quantile(0.95, sum(rate(http_request_duration_seconds_bucket[5m])) by (le))
                                         # 95th percentile latency
```

### ELK quick idea
- **Logstash** — collects, parses, and ships logs.
- **Elasticsearch** — stores + indexes them.
- **Kibana** — UI for searching and dashboards.

### Loki
Like Prometheus but for logs. Each log is labeled and queried with **LogQL**.

### Questions

**Q1. What is "observability"?**
The ability to understand a system's internal state from its outputs (metrics, logs, traces).

**Q2. Metrics vs Logs vs Traces?**
- Metrics — aggregate, cheap, used for alerts.
- Logs — detailed, used for debugging.
- Traces — request flow across services, used for latency investigation.

**Q3. What is OpenTelemetry?**
Open standard + SDK to **collect** metrics, logs, and traces from apps. Backends (Prometheus, Jaeger, etc.) sit behind it.

**Q4. What is a "health check"?**
An HTTP endpoint (e.g., `/healthz`) that returns 200 if the app is up and ready. Used by load balancers and orchestrators.

**Q5. Whitebox vs Blackbox monitoring?**
- Whitebox — inside view, app exposes metrics.
- Blackbox — outside view, you probe the app from the outside (e.g., ping `/login`).

**Q6. SLI vs SLO vs SLA?**
- SLI — the metric.
- SLO — internal target.
- SLA — external contract.

---

## 12. Security & DevSecOps

### Theory

**DevSecOps** = DevOps with security baked in at every step, not as an afterthought.

Common practices:
- **SAST** — Static Application Security Testing (analyze source code, e.g., SonarQube, Semgrep).
- **DAST** — Dynamic testing against a running app (e.g., OWASP ZAP).
- **Container scanning** — find CVEs in images (Trivy, Snyk).
- **Dependency scanning** — `npm audit`, Snyk, Dependabot.
- **Secret scanning** — find leaked keys in repos (GitGuardian, TruffleHog).
- **Least privilege** — give the smallest IAM role needed.
- **SBOM** — Software Bill of Materials, list of all libs in your image.

### Questions

**Q1. What is the principle of least privilege?**
Give every user/service only the permissions it needs — no more.

**Q2. What is a CVE?**
A public ID for a known vulnerability. Tools like Trivy map your image's libs to CVEs.

**Q3. What is "shift-left security"?**
Run security checks in CI (lint, SAST, dependency check) before deploy, not after.

**Q4. What is a "secret"?**
An API key, DB password, private key, etc. Should be stored in **Secrets Manager / Vault**, never in code or `.env` committed to git.

**Q5. What is HTTPS / TLS? Why important?**
Encrypts data in transit. Prevents eavesdropping and tampering. Without it, login data is plain text.

**Q6. What is "OAuth 2.0"?**
A delegation protocol — lets a user grant a third-party app limited access without sharing a password.

**Q7. What is "mTLS"?**
Mutual TLS — both client and server present certificates. Common for service-to-service in zero-trust networks (e.g., Istio, Linkerd).

**Q8. SSH key vs password login?**
SSH keys are much safer than passwords. Generate: `ssh-keygen -t ed25519`. Disable password auth in `/etc/ssh/sshd_config`.

---

## 13. Coding / Scenario Questions

These are common DevOps interview problems. You can solve them in **any scripting language** (Python, Bash). C++ is rarely required here.

### 1. Write a script that finds the top 5 largest files in a directory tree.

```bash
#!/bin/bash
find "$1" -type f -printf '%s\t%p\n' | sort -nr | head -5
```

### 2. Write a script to tail a log and alert (print) when "ERROR" appears 5 times in 60 seconds.

```bash
#!/bin/bash
LOG=${1:-/var/log/app.log}
tail -F "$LOG" | awk '
/ERROR/ { count++; start = (start ? start : systime()); if (systime() - start > 60) {count=0; start=systime()} if (count >= 5) { print "ALERT: 5 errors in 60s"; count=0; start=systime() } }
'
```

> Simpler version: use `grep` counts in a loop with `sleep 60`.

### 3. Find the IP that hit /login the most in an Nginx log.

```bash
awk '$7 ~ /login/ {print $1}' /var/log/nginx/access.log | sort | uniq -c | sort -nr | head -5
```

### 4. Write a Dockerfile for a Python Flask app.

```dockerfile
FROM python:3.12-slim
WORKDIR /app
COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt
COPY . .
ENV PORT=8080
EXPOSE 8080
CMD ["gunicorn", "-b", "0.0.0.0:8080", "app:app"]
```

### 5. Write a Kubernetes Deployment + Service manifest for the same app.

```yaml
apiVersion: apps/v1
kind: Deployment
metadata: { name: flask-app }
spec:
  replicas: 3
  selector: { matchLabels: { app: flask } }
  template:
    metadata: { labels: { app: flask } }
    spec:
      containers:
        - name: flask
          image: myrepo/flask:1.0
          ports: [{ containerPort: 8080 }]
          readinessProbe:
            httpGet: { path: /health, port: 8080 }
---
apiVersion: v1
kind: Service
metadata: { name: flask-svc }
spec:
  selector: { app: flask }
  ports: [{ port: 80, targetPort: 8080 }]
```

### 6. Terraform to create an S3 bucket with versioning and encryption.

```hcl
resource "aws_s3_bucket" "logs" {
  bucket = "company-logs-${var.env}"
}

resource "aws_s3_bucket_versioning" "logs" {
  bucket = aws_s3_bucket.logs.id
  versioning_configuration { status = "Enabled" }
}

resource "aws_s3_bucket_server_side_encryption_configuration" "logs" {
  bucket = aws_s3_bucket.logs.id
  rule {
    apply_server_side_encryption_by_default { sse_algorithm = "AES256" }
  }
}
```

### 7. A service is down — how do you debug?

Approach (interview answer):
1. Check status: `systemctl status myservice`.
2. Check logs: `journalctl -u myservice -n 200` or app log file.
3. Is the process running? `ps aux | grep myservice`.
4. Listening on the right port? `ss -tulpn | grep 8080`.
5. Try to hit it locally: `curl localhost:8080`.
6. Resource limits? `top`, `free -m`, `df -h`.
7. Recent deploys? `git log`, image tags.
8. Network: `curl` from another pod/host.
9. Health-check endpoint failures (`/healthz`)?

### 8. A pod is stuck in `CrashLoopBackOff` — what now?

- `kubectl describe pod <pod>` — see events, last state, exit code.
- `kubectl logs <pod> --previous` — logs from the last crashed container.
- Common causes: bad config, missing env var, OOMKilled, failing healthcheck, image pull error (`ErrImagePull`).

### 9. How do you handle a stuck deployment?

- Check `kubectl rollout status deploy/web`.
- `kubectl describe deploy/web` — events.
- If clearly bad: `kubectl rollout undo deploy/web`.
- Delete the bad ReplicaSet if needed: `kubectl delete rs <name>`.

### 10. Write a GitHub Action that runs tests and posts the result to Slack.

```yaml
name: tests
on: [push]
jobs:
  test:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - run: npm ci && npm test
      - name: Slack on failure
        if: failure()
        uses: slackapi/slack-github-action@v1
        with:
          payload: { text: "Tests failed on ${{ github.repository }}" }
        env:
          SLACK_WEBHOOK_URL: ${{ secrets.SLACK_WEBHOOK }}
```

---

## 14. Quick Revision Cheatsheet

### Bash one-liners
| Task | Command |
|---|---|
| Memory usage | `free -h` |
| Disk usage | `df -h` |
| Top CPU processes | `top` then `P` |
| Listening ports | `ss -tulpn` |
| Public IP | `curl ifconfig.me` |
| Running services | `systemctl list-units --type=service --state=running` |
| Search file content | `grep -r "needle" .` |
| Replace in place | `sed -i 's/old/new/g' file` |

### Docker one-liners
| Task | Command |
|---|---|
| List containers | `docker ps -a` |
| Exec shell | `docker exec -it c1 sh` |
| Build | `docker build -t app:1.0 .` |
| Scan image | `trivy image app:1.0` |
| Compose up | `docker compose up -d` |
| Prune all | `docker system prune -a` |

### kubectl one-liners
| Task | Command |
|---|---|
| Get pods | `kubectl get pods -A` |
| Logs | `kubectl logs pod -f` |
| Shell | `kubectl exec -it pod -- sh` |
| Restart | `kubectl rollout restart deploy/x` |
| Scale | `kubectl scale --replicas=5 deploy/x` |
| Switch context | `kubectl config use-context prod` |
| Apply | `kubectl apply -f x.yaml` |
| Get nodes | `kubectl get nodes -o wide` |

### AWS one-liners
| Task | Command |
|---|---|
| Who am I | `aws sts get-caller-identity` |
| List S3 | `aws s3 ls` |
| EC2 list | `aws ec2 describe-instances` |
| Tail logs | `aws logs tail /aws/lambda/x --follow` |

### Terraform
```bash
terraform fmt -check          # format
terraform validate            # syntax + types
terraform init -upgrade
terraform plan -out=tfplan
terraform apply tfplan
terraform output
terraform destroy
```

### CI/CD stages everyone remembers
```
lint → unit test → build → integration test → security scan → deploy (dev/stage/prod)
```

### Top 20 Interview One-liners
1. **CI/CD** = build, test, deploy on every commit.
2. **Containers** package an app + deps; **Kubernetes** runs them at scale.
3. **IaC** (Terraform/CloudFormation) makes infra reproducible.
4. **Monitoring** is metrics + logs + traces.
5. **SLO** is the reliability target; **error budget** is what you spend.
6. **Blue/green** deploys switch traffic between two identical envs.
7. **Canary** sends 1–10% traffic to the new version first.
8. **Shift-left security** = test security early in CI.
9. **Immutable infra** = rebuild, never patch.
10. **GitOps** = Git is the single source of truth; ArgoCD/Flux applies it.
11. **Service mesh** (Istio/Linkerd) handles mTLS, retries, observability.
12. **Sidecar proxy** = small helper container in the same Pod.
13. **Readiness probe** = "ready to serve?" **Liveness** = "still alive?"
14. **HPA** = scale pods on metrics. **Cluster Autoscaler** = scale nodes.
15. **StatefulSet** = stable, sticky identities for stateful workloads.
16. **CIDR /24** = 256 IPs, /16 = 65,536.
17. **Layered caching** in Docker — order instructions from least-changing to most.
18. **Trivy** scans images; **Snyk/Dependabot** watch dependencies.
19. **Git revert** = safe undo. **Git reset --hard** = dangerous undo.
20. **Prometheus** pulls metrics; **Fluentd/Loki** handles logs.

---

## Coverage Summary

This guide covers the topics most often asked in DevOps interviews:

- Linux, Shell Scripting, Networking
- Git & Version Control
- Docker & docker-compose
- Kubernetes (Pods, Deployments, Services, Ingress, Probes, HPA)
- AWS (EC2, S3, IAM, VPC, RDS, Lambda, CloudWatch, CloudTrail, ALB/NLB)
- CI/CD (Jenkins, GitHub Actions, GitLab CI)
- Terraform & Ansible (IaC & config management)
- Monitoring & Logging (Prometheus, Grafana, ELK, OpenTelemetry)
- Security & DevSecOps (SAST, DAST, scanning, secrets, OAuth/TLS)
- Coding & troubleshooting scenarios
- A print-friendly cheat sheet

Keep this file open during your last-day revision.
