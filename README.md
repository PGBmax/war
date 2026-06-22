<div align="center">
  <h1>⚔️ War</h1>
  <p><em>An ELF virus written in C — 42 Post-CC cybersecurity project</em></p>
</div>

<br/>

<div align="center">

![Static Badge](https://img.shields.io/badge/os-linux-orange)
![Static Badge](https://img.shields.io/badge/language-C99-blue)
![Static Badge](https://img.shields.io/badge/C2%20port-6942%20-red)
![Static Badge](https://img.shields.io/badge/auth-SHA--256-purple)
![Static Badge](https://img.shields.io/badge/norm-42-green)

</div>

---

> **⚠️ Educational purposes only. Do not deploy on systems you do not own or have explicit permission to test. This project is a controlled academic exercise in malware analysis and low-level systems programming.**

---

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Architecture](#architecture)
- [C2 Shell Commands](#c2-shell-commands)
- [Build](#build)
- [How It Works](#how-it-works)
- [Anti-Analysis Mechanisms](#anti-analysis-mechanisms)
- [Authors](#authors)

---

## Overview

**War** is a self-replicating ELF virus for Linux, developed as part of the **42 Post-CC** (Post Common Core) curriculum. It demonstrates advanced concepts in system programming: ELF binary infection, process daemonization, anti-debugging techniques, code obfuscation, machine fingerprinting, and remote command & control via a TCP bind shell.

---

## Features

| Feature | Description |
|---|---|
| **ELF Infection** | Appends itself to valid ELF binaries found on the filesystem |
| **Directory Crawling** | Recursively traverses directories to find infection targets |
| **Payload Execution** | Extracts and executes the original binary after infection |
| **Daemonization** | Detaches from the terminal and silently runs in the background |
| **Systemd Persistence** | Installs itself as a fake `Anti aerial turret driver` systemd service on startup |
| **C2 Server** | TCP bind shell on port `6967` (weak) / `4242` (root), with SHA-256 password auth |
| **Machine Fingerprinting** | Generates a unique fingerprint embedded in the virus signature |
| **Anti-Debug** | Refuses to run under a debugger |
| **Process Blocking** | Halts execution if a specific blocking process is detected |
| **Code Obfuscation** | Critical routines (crawl, infect) are obfuscated to resist static analysis |
| **File Encryption** | Encrypt/decrypt arbitrary files over the C2 channel |

---

## Architecture

```
war/
├── src/
│   ├── main.c            # Entry point — anti-debug, fork, daemonize
│   ├── crawl.c           # Obfuscated directory crawler
│   ├── infect.c          # Obfuscated ELF infection routine
│   ├── payload.c         # Payload packing / extraction / execution
│   ├── check.c           # ELF header & signature validation
│   ├── daemon.c          # Process daemonization & output muting
│   ├── service.c         # Systemd service persistence
│   ├── sha256.c          # SHA-256 implementation (auth & encryption)
│   ├── utils.c           # String helpers
│   ├── run_check.c       # Anti-debugger detection
│   ├── fingerprint.c     # Machine fingerprint generation
│   ├── server/
│   │   ├── server.c          # TCP server (bind shell, poll-based)
│   │   ├── server_clients.c  # Client connection handling
│   │   ├── server_update.c   # Command dispatch loop
│   │   └── server_utils.c    # Utilities
│   └── list/
│       ├── list.c        # Generic linked list
│       └── list_node.c
├── inc/
│   ├── war.h             # Core types, macros, signatures
│   ├── service.h         # Persistence & lock file definitions
│   ├── sha256.h
│   ├── server/server.h   # C2 server types & command strings
│   └── list/
└── test/
    └── anti_virus.c      # Reference anti-virus process (for testing)
```

### Infection footer format

Each infected binary gets a footer appended at the end:

```
[ original binary ] [ viral payload ] [ t_footer ]
```

```c
typedef struct s_footer {
    uint64_t  magic;          // 0x4242424242424242
    char      signature[N];   // "War version 1.0 (c)oded by mbatty-pboucher - <fingerprint>"
    uint64_t  payload_size;
} t_footer;
```

---

## C2 Shell Commands

Once connected and authenticated (SHA-256 password), the following commands are available:

| Command | Description |
|---|---|
| `help` | List all available commands |
| `ls` | List current directory contents |
| `cd [path]` | Change current directory |
| `getcwd` | Print current working directory |
| `clear` | Clear the terminal |
| `delete <path>` | Delete a file |
| `encrypt <password> <path>` | Encrypt a file |
| `decrypt <password> <path>` | Decrypt a file |
| `quit` | Disconnect |

---

## Build

```bash
# Build the virus binary
make

# Run the test anti-virus (for development testing)
cd test && make
```

**Requirements:** `gcc` / `cc`, Linux, C99.

---

## How It Works

1. **Launch** — Checks for a debugger and the blocking process (`super_anti_virus`). Exits silently if either is found.
2. **Fork** — The parent extracts and executes the original payload (transparent to the user). The child continues as the virus.
3. **Daemonize** — Double-fork, setsid, redirect stdout/stderr to `/dev/null`.
4. **Crawl** — Recursively scan directories for ELF binaries not yet infected (checked via footer signature).
5. **Infect** — Pack the virus binary as a payload and append it to each target ELF with a `t_footer` embedding the machine fingerprint.
6. **Persist** — Install a systemd service disguised as `Anti aerial turret driver` (`/etc/systemd/system/war.service`) for auto-start on reboot.
7. **Serve** — Spin up the C2 TCP bind shell on port `6967` (weak) or `4242` (root) and wait for operator connections.

---

## Anti-Analysis Mechanisms

- **Anti-debugger** — `runningUnderDebugger()` detects tracing via `/proc/self/status` (`TracerPid`).
- **Process blocking** — `is_process_running("super_anti_virus")` scans `/proc` at startup.
- **Code obfuscation** — The `crawl.c` and `infect.c` routines wrap every sensitive syscall (`opendir`, `readdir`, `open`, `rename`, etc.) through multiple layers of indirected function pointers and opaque XOR gadgets, making static analysis and disassembly significantly harder.

---

## Authors

Made with 🧨 by **[mbatty](https://github.com/scorpionnem)** & **[pboucher](https://github.com/pgbmax)**.
