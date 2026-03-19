#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>       
#include <sys/socket.h>    
#include <netinet/in.h>    
#include <openssl/ssl.h>   
#include <openssl/err.h>   
#include <unistd.h>        
#include <arpa/inet.h>       
#include "server.h"
#include "../messaging/chatroom.h"
#include <fcntl.h>



static int listen_fd;
static int running = 1;
static SSL_CTX *ssl_ctx;
static pthread_t accept_thread;

#define DEVICE_PATH "/dev/keycipher_in"

//Helpers for server_handle_connection
static int parse_header(const char *request, const char *header_name, char *out, int out_len) {
    const char *p = strstr(request, header_name);
    if (!p) return 0;

    p += strlen(header_name);
    while (*p == ' ') {
        p++;
    }
    
    int i = 0;
    while (*p && *p != '\r' && *p != '\n' && i < out_len -1) {
        out[i++] = *p++;
    } 
    out[i] = '\0';
    return 1;
}

static const char *parse_body(const char *buf) {
    const char *body = strstr(buf, "\r\n\r\n");
    if (!body) {
        return NULL;
    }
    return body + 4; 
}

int server_init(int port)
{
    struct sockaddr_in addr;

    //SSL setup
    SSL_library_init();
    OpenSSL_add_all_algorithms();

    ssl_ctx = SSL_CTX_new(TLS_server_method());
    if (!ssl_ctx) {
        ERR_print_errors_fp(stderr);
        return -1;
    }

    int cert_file = SSL_CTX_use_certificate_file(ssl_ctx, "cert.pem", SSL_FILETYPE_PEM);
    if (cert_file <= 0) {
        ERR_print_errors_fp(stderr);
        return -1;
    }

    int priv_key_file = SSL_CTX_use_PrivateKey_file(ssl_ctx, "key.pem", SSL_FILETYPE_PEM);
    if (priv_key_file <= 0) {
        ERR_print_errors_fp(stderr);
        return -1;
    }

    //Socket Setup

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

    //Make accept threads

    pthread_create(&accept_thread, NULL, server_accept_loop, NULL);
    pthread_detach(accept_thread);

    printf("Server listening on port %d\n", port);
    return 0;
}

void *server_accept_loop(void *arg)
{
    int client_fd;
    struct sockaddr_in client_addr;

    while (running) {
        socklen_t addr_len = sizeof(client_addr);

        printf("Waiting for connection...\n");
        fflush(stdout);
        
        client_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &addr_len);
        if (client_fd < 0) {
            perror("Accept failed");
            continue;
        }

        int *client_fd_ptr = malloc(sizeof(int));
        if (!client_fd_ptr) {
            perror("Malloc failure");
            close(client_fd);
            continue;
        }
        *client_fd_ptr = client_fd;
        pthread_t thread; //Handle's one peer http request
        pthread_create(&thread, NULL, server_handle_connection, client_fd_ptr);
        pthread_detach(thread);

        printf("New connection from %s\n", inet_ntoa(client_addr.sin_addr));
        fflush(stdout);
    }

    return NULL;
}


void *server_handle_connection(void *arg)
{
    int client_fd = *(int*)arg;
    free(arg); // was malloc'd before pthread_create

    char buf[1024];
    char sender_ip[64] = "unknown";
    char is_chatroom[8] = "0";
    char content_len[16] = "0";


    SSL *ssl = SSL_new(ssl_ctx);
    SSL_set_fd(ssl, client_fd);

    int ssl_accept = SSL_accept(ssl);
    if (ssl_accept <= 0) {
        ERR_print_errors_fp(stderr);
        SSL_free(ssl);
        close(client_fd);
        return NULL;
    }

    int bytes = SSL_read(ssl, buf, sizeof(buf) - 1);
    if (bytes <= 0) {
        SSL_shutdown(ssl);
        SSL_free(ssl);
        close(client_fd);
        return NULL;
    }
    buf[bytes] = '\0';
    
    parse_header(buf, "X-Sender-IP:", sender_ip, sizeof(sender_ip));
    parse_header(buf, "X-Is-Chatroom:",  is_chatroom,  sizeof(is_chatroom));
    parse_header(buf, "Content-Length:", content_len,  sizeof(content_len));

    int msg_len = atoi(content_len);

    printf("KeyCipher: message from %s | chatroom = %s | len = %d\n", sender_ip, is_chatroom, msg_len);
    const char *body = parse_body(buf);
    if (!body || msg_len <= 0) {
        const char *bad = "HTTP/1.1 400 Bad Request\r\n\r\n";
        SSL_write(ssl, bad, strlen(bad));
        SSL_shutdown(ssl);
        SSL_free(ssl);
        close(client_fd);
        return NULL;
    }

    //Want to send HTTP 429 instead of blocking here so client blocks
    int ret;

    if (atoi(is_chatroom)) {
        // chatroom message - goes to chatroom FIFO and chatroom_receive returns -ENOBUFS if full  
        ret = chatroom_receive(body, sender_ip);
    } else {
        // direct message - goes to inbox FIFO 
        int dev_fd = open(DEVICE_PATH, O_WRONLY | O_NONBLOCK);
        if (dev_fd < 0) {
            perror("KeyCipher: failed to open device");
            const char *err = "HTTP/1.1 503 Service Unavailable\r\n\r\n";
            SSL_write(ssl, err, strlen(err));
            SSL_shutdown(ssl);
            SSL_free(ssl);
            close(client_fd);
            return NULL;
        }
        ret = write(dev_fd, body, msg_len);
        close(dev_fd);
    }

    if (ret < 0) { //FIFO FULL
        printf("KeyCipher: FIFO full, sending 429 to %s\n", sender_ip);
        const char *busy = "HTTP/1.1 429 Too Many Requests\r\n\r\n";
        SSL_write(ssl, busy, strlen(busy));
        SSL_shutdown(ssl);
        SSL_free(ssl);
        close(client_fd);
        return NULL;
    }

    printf("KeyCipher: Received and accepted message from %s\n", sender_ip);

    const char *response = "HTTP/1.1 200 OK\r\n" "Content-Length: 0\r\n" "\r\n"; 
    SSL_write(ssl, response, strlen(response));
    

    //Clean up
    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(client_fd);
    return NULL;

}

void server_stop(void)
{
    running = 0;
    close(listen_fd);
    SSL_CTX_free(ssl_ctx);
    printf("KeyCipher: Server stopped\n");
}
