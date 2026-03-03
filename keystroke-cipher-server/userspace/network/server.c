#include <stdio.h>
#include <pthread.h>
#include "server.h"

/*
 * server_init - set up SSL context and start listening
 * - SSL_CTX_new(TLS_server_method())
 * - SSL_CTX_use_certificate_file, SSL_CTX_use_PrivateKey_file
 * - socket, setsockopt, bind, listen
 * - pthread_create for server_accept_loop
 */
int server_init(int port)
{
    /* TODO: implement HTTPS server setup */
    return 0;
}

/*
 * server_accept_loop - continuously accept incoming peer connections
 * - while(running): SSL_accept on new connections
 * - for each connection: pthread_create(server_handle_connection)
 * - handle thread cleanup to avoid zombie threads
 */
void *server_accept_loop(void *arg)
{
    /* TODO: implement accept loop */
    return NULL;
}

/*
 * server_handle_connection - process one peer HTTP request
 * - parse HTTP headers to get Content-Length, X-Sender-IP, X-Is-Chatroom
 * - read body (the encrypted message)
 * - check target FIFO (incoming or chatroom) for space
 * - if FIFO full: SSL_write HTTP 429 response and close
 * - if space: write message to FIFO, SSL_write HTTP 200 response
 * this is where backpressure is enforced at the receiving end
 */
void *server_handle_connection(void *arg)
{
    /* TODO: implement request handler */
    return NULL;
}

/*
 * server_stop - shut down server cleanly
 * - set running = 0
 * - close listen_fd
 * - SSL_CTX_free(ssl_ctx)
 */
void server_stop(void)
{
    /* TODO: implement server shutdown */
}
