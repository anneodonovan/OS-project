#ifndef DIRECT_H
#define DIRECT_H
#define MAX_MESSAGES 64

#include "../network/peer_manager.h"


int direct_send(peer_t *peer, const char *plaintext);
void *direct_receive_loop(void *arg);

//Kernel message struct 
typedef struct {
    long long tv_sec;
    long tv_nsec;
    char author[64];
    char data[256];
    int len;
} kernel_msg_t;

//Userspace message struct
typedef struct {
    int id;
    char sender[64];
    long timestamp;
    char encrypted_preview[256];
} user_msg_t;

int direct_get_message_count(void);
int direct_get_messages(user_msg_t *out, int max);

#endif 
