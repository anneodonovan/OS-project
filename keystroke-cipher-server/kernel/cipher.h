#ifndef CIPHER_H
#define CIPHER_H

/*
 * rot13_encrypt - apply ROT13 transform to a buffer in-place
 * called by keycipher_write() before storing into outgoing FIFO
 * - iterate each byte in buf up to len
 * - if a-z: shift by 13 wrapping at z
 * - if A-Z: shift by 13 wrapping at Z
 * - all other bytes pass through unchanged
 */
void rot13_encrypt(char *buf, int len);

/*
 * rot13_decrypt - ROT13 is symmetric, identical to encrypt
 * called by keycipher_read() after pulling from incoming FIFO
 * - simply calls rot13_encrypt internally
 */
void rot13_decrypt(char *buf, int len);

#endif /* CIPHER_H */
