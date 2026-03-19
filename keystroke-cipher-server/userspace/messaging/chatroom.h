#ifndef CHATROOM_H
#define CHATROOM_H

#define MAX_CHAT_MESSAGES 64

//Userspace msg struct
typedef struct {
    int id;
    char sender[64];
    long timestamp;
    char encrypted_preview[256];  
} chat_msg_t;

int chatroom_send(const char *plaintext);
int chatroom_receive(const char *encrypted_msg, const char *sender_ip);
void *chatroom_read_loop(void *arg);
int chatroom_get_semaphore_count(void);
int chatroom_get_message_count(void);
chat_msg_t *chatroom_get_messages(void);

#endif 
