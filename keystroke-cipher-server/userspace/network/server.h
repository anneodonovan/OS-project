#ifndef SERVER_H
#define SERVER_H

/*
 * server_init - start HTTPS server listening for incoming peer messages
 * - create SSL context, load cert and key
 * - bind to 0.0.0.0:8443
 * - listen for connections
 * - spawn accept loop thread
 * returns 0 on success, -1 on failure
 */
int server_init(int port);

/*
 * server_accept_loop - runs in its own thread, accepts peer connections
 * - SSL_accept on each incoming connection
 * - spawn a handler thread per connection
 */
void *server_accept_loop(void *arg);

/*
 * server_handle_connection - handle one peer's HTTP request
 * - read HTTP method and path
 * - route to handle_incoming_message or handle_chatroom_message
 * - if our incoming FIFO is full: respond HTTP 429 (backpressure signal)
 * - if space available: write to fifo, respond HTTP 200
 */
void *server_handle_connection(void *arg);

/*
 * server_stop - gracefully shut down the server
 * - close listening socket
 * - SSL_CTX_free
 */
void server_stop(void);

#endif /* SERVER_H */
