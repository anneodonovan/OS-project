#include "cipher.h"

void rot13_encrypt(char *buf, int len)
{
    for (int i=0; i < len; i++) {
        char current = buf[i];

        //Detects whether upper or lowercase alphabet, shifts by 13 within respective alphabet range
        if (current >= 'A' && current <= 'Z') {
            buf[i] = ((current - 'A' + 13) % 26) + 'A';
        } else if (current >= 'a' && current <= 'z') {
            buf[i] = ((current - 'a' + 13) % 26) + 'a';
        }
        //else: leave unchanged (punctuation, etc.)
    }
}

void rot13_decrypt(char *buf, int len)
{
    rot13_encrypt(buf, len); //symmetric with encrypt, so encrypt again decrypts
}
