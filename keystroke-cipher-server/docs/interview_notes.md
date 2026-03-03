# Interview Notes - OS Concepts by Component

## kernel/fifo_buffer.c
- **Semaphores**: sem_empty counts free slots, sem_full counts used slots
- **Blocking**: down_interruptible blocks the calling process until semaphore > 0
- **Wakeup**: up() on sem_empty wakes a blocked remote sender
- **Spinlock**: protects head/tail/count from concurrent access (different from semaphore)

## kernel/keycipher.c
- **Character device**: registered with register_chrdev, accessed via file operations
- **ioctl**: out-of-band control - mode switching, flush, stats query
- **copy_to_user / copy_from_user**: safe kernel↔userspace data transfer

## kernel/hid_intercept.c
- **Input subsystem**: kernel framework for hardware input devices
- **Event handler**: registered callback, fires on every key event
- **Interrupt context**: key events arrive asynchronously

## userspace/messaging/chatroom.c
- **Counting semaphore**: chatroom buffer has N slots, semaphore counts free slots
- **Deadlock scenario**: Machine A full waiting for B to read, B full waiting for A
- **Deadlock avoidance**: timeout + drop oldest message strategy

## The Demo Explanation
"When a user clicks READ, our Node.js bridge calls POST /api/read/:id,
which the C API server handles by issuing an ioctl to /dev/keycipher_in.
The kernel decrypts the message and calls up() on the semaphore,
freeing one slot. Any remote peer that was blocked trying to send
to us is sleeping in down_interruptible() — our up() wakes them,
their write succeeds, and the message arrives in our buffer."
