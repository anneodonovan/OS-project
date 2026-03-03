#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "client.h"
#include "peer_manager.h"

/*
 * client_connect - TCP + SSL handshake to peer
 * - socket(AF_INET, SOCK_STREAM, 0)
 * - connect() to peer->ip:peer->port
 * - SSL_new, SSL_set_fd, SSL_connect
 * - on success: peer->status = PEER_CONNECTED
 * - on failure: peer->status = PEER_DISCONNECTED, return -1
 */
int client_connect(peer_t *peer)
{
    /* TODO: implement HTTPS connection */
    return 0;
}

/*
 * client_send_message - HTTP POST encrypted message to peer
 * - build HTTP POST request with encrypted_msg as body
 * - SSL_write the request
 * - read response code from SSL_read
 * - if 429: set PEER_BLOCKED, sleep and retry (backpressure)
 * - if 200: set PEER_CONNECTED (unblocked), return 0
 * - if 503: set PEER_DISCONNECTED, return -1
 */
int client_send_message(peer_t *peer, const char *encrypted_msg, int is_chatroom)
{
    /* TODO: implement HTTP POST with backpressure handling */
    return 0;
}

/*
 * client_broadcast - send to all connected peers concurrently
 * - for each peer in peers[] where status == PEER_CONNECTED
 *   - call client_send_message(peer, encrypted_msg, is_chatroom)
 * NOTE: each peer has its own thread so blocking on one peer
 *       does not block delivery to other peers
 */
void client_broadcast(const char *encrypted_msg, int is_chatroom)
{
    /* TODO: implement broadcast */
}

/*
 * client_connect_thread - runs in a loop maintaining connection to one peer
 * - cast arg to peer_t*
 * - while(1): try client_connect
 *   - if fails: sleep(5) and retry (reconnection backoff)
 *   - if succeeds: stay connected, handle reconnect on disconnect
 */
void *client_connect_thread(void *arg)
{
    /* TODO: implement persistent connection thread */
    return NULL;
}

/*
 * client_disconnect - graceful SSL teardown
 * - SSL_shutdown(peer->ssl)
 * - SSL_free(peer->ssl)
 * - close(peer->socket_fd)
 * - peer->status = PEER_DISCONNECTED
 */
void client_disconnect(peer_t *peer)
{
    /* TODO: implement disconnect */
}
