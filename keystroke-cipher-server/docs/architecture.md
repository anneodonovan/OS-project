# System Architecture

## Overview
KeyCipher is a kernel-level encrypted P2P messaging system.
Plaintext never leaves the kernel — all encryption/decryption happens in kernel space via cipher.c.

## Where Encryption Lives
```
kernel/cipher.c   ← THE ONLY place ROT13 encrypt/decrypt runs
```
- Called by keycipher_write() before storing to outgoing FIFO
- Called by keycipher_read()  after pulling from incoming FIFO
- Userspace never touches plaintext in transit

## Component Diagram
```
[Keyboard] → hid_intercept.c → cipher.c (encrypt) → fifo_buffer (out) → [Network]
                                                                               ↓
[Browser UI] ← Node.js bridge ← C API ← cipher.c (decrypt on READ) ← fifo_buffer (in)
```

## Data Flow
Outgoing:
  userspace writes plaintext → /dev/keycipher_out
  kernel: rot13_encrypt() → store in outgoing FIFO
  network layer reads encrypted text and sends to peers

Incoming:
  peer sends encrypted text → server.c writes to /dev/keycipher_in
  kernel stores encrypted text in incoming FIFO
  user clicks READ → ioctl → kernel: rot13_decrypt() → copy_to_user
  userspace receives plaintext

## Key Design Decisions
- Cipher in kernel: plaintext never exposed to any userspace process
- Two FIFOs: clean in/out separation, prevents double-encryption
- HTTP 429 for backpressure: standard semantics, no custom protocol
- Small chatroom buffer (3 slots): makes semaphore blocking visible in demo
- JS ROT13 in browser: display-only preview in compose box, not the real cipher
