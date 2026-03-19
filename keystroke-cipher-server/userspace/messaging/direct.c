#include <stdio.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include "direct.h"
#include "../network/client.h"

#define DEVICE_OUT "/dev/keycipher_out"
#define DEVICE_IN "/dev/keycipher_in"


static user_msg_t inbox[MAX_MESSAGES];
static int inbox_count = 0;
static int next_id = 1;
static pthread_mutex_t inbox_lock  = PTHREAD_MUTEX_INITIALIZER;

int direct_get_message_count(void) {
    pthread_mutex_lock(&inbox_lock);
    int count = inbox_count;
    pthread_mutex_unlock(&inbox_lock);
    return count;
}

int direct_get_messages(user_msg_t *out, int max) {
    int n;
    pthread_mutex_lock(&inbox_lock);
    if (inbox_count < max) {
        n = inbox_count;
    } else {
        n = max;
    }
    memcpy(out, inbox, n * sizeof(user_msg_t));
    pthread_mutex_unlock(&inbox_lock);
    return n;
}


int direct_send(peer_t *peer, const char *plaintext)
{
    kernel_msg_t msg;
    kernel_msg_t encrypted;
    int dev_fd;
    int ret;
    int bytes;

    memset(&msg, 0, sizeof(msg));
    strncpy(msg.data, plaintext, sizeof(msg.data) - 1);
    msg.len = strlen(plaintext);

    dev_fd = open(DEVICE_OUT, O_WRONLY);
    if (dev_fd < 0) {
        perror("direct_send: Couldn't open device");
        return -1;
    }

    ret = write(dev_fd, &msg, sizeof(msg));
    close(dev_fd);
    if (ret < 0) {
        perror("direct_send: Write failure");
        return -1;
    }

    dev_fd = open(DEVICE_OUT, O_RDONLY);
    if (dev_fd < 0) {
        perror("direct_send: Couldn't read encrypted message");
        return -1;
    }

    bytes = read(dev_fd, &encrypted, sizeof(encrypted));
    close(dev_fd);
    if (bytes < 0) {
        perror("direct_send: Read encrypted failure");
        return -1;
    }

    //Hand over to client in network
    return client_send_message(peer, (const char*)&encrypted, 0);
}

void *direct_receive_loop(void *arg)
{
    kernel_msg_t msg;
    int dev_fd;
    int bytes;

    dev_fd = open(DEVICE_IN, O_RDONLY);
    if (dev_fd < 0) {
        perror("direct_receive_loop: Open device failure");
        return NULL;
    }

    printf("direct_receive_loop: Started, waiting for messages...\n");

    while (1) {
        bytes = read(dev_fd, &msg, sizeof(msg));
        if (bytes < 0) {
            perror("direct_receive_loop: Read message failure");
            break;
        }
        if (bytes == 0) continue;

        printf("%s: %.*s\n", msg.author, msg.len, msg.data);
        
        pthread_mutex_lock(&inbox_lock);
        if (inbox_count < MAX_MESSAGES) {
            inbox[inbox_count].id = next_id++;
            strncpy(inbox[inbox_count].sender, msg.author, 63);
            inbox[inbox_count].timestamp = msg.tv_sec;
            strncpy(inbox[inbox_count].encrypted_preview, msg.data, 255);
            inbox_count++;
        } else {
            printf("direct_receive_loop: Inbox full, dropping message\n");
        }
        pthread_mutex_unlock(&inbox_lock);
    
    }

    close(dev_fd);
    return NULL;
}
