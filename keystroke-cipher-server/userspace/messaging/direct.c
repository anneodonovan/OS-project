#include <fcntl.h>
#include <unistd.h>
#include "direct.h"

/*
 * direct_send - write plaintext to /dev/keycipher_out
 * - open("/dev/keycipher_out", O_WRONLY)
 * - write(fd, plaintext, len) - kernel encrypts and puts in outgoing FIFO
 * - close(fd)
 * - client layer then reads outgoing FIFO and sends to peer
 */
int direct_send(peer_t *peer, const char *plaintext)
{
    /* TODO: implement direct message send via kernel device */
    return 0;
}

/*
 * direct_receive_loop - blocking read loop on /dev/keycipher_in
 * - open("/dev/keycipher_in", O_RDONLY)
 * - while(1): read(fd, buf, MAX_MESSAGE_LEN)
 *   - read BLOCKS if FIFO is empty (kernel semaphore)
 *   - kernel decrypts before returning data to userspace
 *   - store message in a userspace queue for API to serve to frontend
 */
void *direct_receive_loop(void *arg)
{
    /* TODO: implement blocking receive loop */
    return NULL;
}
