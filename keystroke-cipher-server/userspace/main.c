#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include "network/server.h"
#include "network/peer_manager.h"
#include "messaging/direct.h"
#include "messaging/chatroom.h"

static volatile int running = 1;

/*
 * signal_handler - catch SIGINT/SIGTERM for clean shutdown
 * - set running = 0
 * - server_stop()
 * - peer_manager_cleanup()
 */
static void signal_handler(int sig)
{
    /* TODO: implement clean shutdown */
    running = 0;
}

/*
 * main - entry point for userspace daemon
 * 1. register signal handlers (SIGINT, SIGTERM)
 * 2. peer_manager_init("../peers.conf") - load peer list
 * 3. server_init(8443) - start HTTPS server for incoming messages
 * 4. peer_manager_connect_all() - connect to all peers
 * 5. pthread_create: direct_receive_loop  - handles /dev/keycipher_in reads
 * 6. pthread_create: chatroom_read_loop   - handles chatroom FIFO reads
 * 7. start C API server on port 8080 for Node.js bridge
 * 8. wait for shutdown signal
 * 9. clean up all resources
 */
int main(int argc, char *argv[])
{
    /* TODO: implement daemon startup sequence */
    printf("KeyCipher daemon starting...\n");
    return 0;
}
