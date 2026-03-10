#include <stdio.h>
#include <pthread.h>       // threads for accept loop
#include "server.h"
#include <sys/socket.h>    // socket, bind, listen, accept
#include <netinet/in.h>    // sockaddr_in, htons
//#include <openssl/ssl.h>   // SSL_CTX, SSL_new, SSL_accept etc
//#include <openssl/err.h>   // ERR_print_errors_fp for debugging
#include <unistd.h>        // close()
#include <arpa/inet.h>       //inet()

static int listen_fd;
static int running = 1;


/*
 * server_init - set up SSL context and start listening
 * - SSL_CTX_new(TLS_server_method())
 * - SSL_CTX_use_certificate_file, SSL_CTX_use_PrivateKey_file
 * - socket, setsockopt, bind, listen
 * - pthread_create for server_accept_loop
 */
int server_init(int port)
{
    struct sockaddr_in addr;

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        printf("Socket failure\n");
        return -1;
    }

    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port
    );

    int binder = bind(listen_fd, (struct sockaddr*)&addr, sizeof(addr));
    if (binder < 0) {
        perror("Bind Failed");
        return -1;
    }

    int listener = listen(listen_fd, 10);
    if (listener < 0) {
        perror("Listen Failed");
        return -1;
    }

    printf("Server listening on port %d\n", port);
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
    int client_fd;
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);

    while (running) {
        printf("Waiting for connection...\n");
        fflush(stdout); 
        client_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &addr_len);
        if (client_fd < 0) {
            perror("Accept failed");
            continue;
        }

        printf("New connection from %s\n", inet_ntoa(client_addr.sin_addr));
        close(client_fd);
    }
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


int main() {
    server_init(8080);
    server_accept_loop(NULL);
    return 0;
}