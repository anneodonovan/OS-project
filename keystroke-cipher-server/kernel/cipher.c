#include "cipher.h"

/*
 * rot13_encrypt - ROT13 applied in-place inside the kernel
 * this is the ONLY place encryption/decryption happens in the entire system
 * userspace never sees plaintext in transit — only before write and after read
 *
 * implementation:
 * - loop i from 0 to len
 * - if buf[i] >= 'a' && buf[i] <= 'z': buf[i] = 'a' + (buf[i] - 'a' + 13) % 26
 * - if buf[i] >= 'A' && buf[i] <= 'Z': buf[i] = 'A' + (buf[i] - 'A' + 13) % 26
 * - else: leave unchanged
 */
void rot13_encrypt(char *buf, int len)
{
    /* TODO: implement ROT13 */
}

/*
 * rot13_decrypt - symmetric with encrypt, just calls rot13_encrypt
 */
void rot13_decrypt(char *buf, int len)
{
    /* TODO: call rot13_encrypt */
}
