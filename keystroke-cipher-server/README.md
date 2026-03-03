# KeyCipher — Kernel-Level Encrypted P2P Messaging

A Linux kernel module (LKM) that intercepts keystrokes, encrypts/decrypts
messages at kernel level, and sends them peer-to-peer over HTTPS.

## Architecture
- Keystroke interception via /dev/input/eventX
- ROT13 encryption/decryption in kernel space
- Two FIFO buffers (in/out) with semaphore-based blocking I/O
- P2P HTTPS messaging between devices
- Shared chatroom with counting semaphore demonstration
- Live /proc/keycipher/stats
- React frontend

## Quick Start
```bash
nano peers.conf           # add peer IPs
make                      # build kernel module + userspace
sudo ./scripts/load.sh    # load kernel module
./userspace/keycipherd    # start userspace daemon
cd frontend && npm run dev  # start React frontend
```

## Build Requirements
- linux-headers: apt install linux-headers-$(uname -r)
- GCC, Make
- OpenSSL: apt install libssl-dev
- Node.js + npm (for frontend)
