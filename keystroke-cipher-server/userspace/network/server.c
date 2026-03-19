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
#include "../messaging/direct.h"
#include <fcntl.h>



static int listen_fd;
static int running = 1;
static SSL_CTX *ssl_ctx;
static pthread_t accept_thread;

#define DEVICE_PATH "/dev/keycipher_in"
#define ENC_QUEUE   "/tmp/keycipher_enc_queue"

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
    socklen_t addr_len = sizeof(client_addr);

    while (running) {
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
    }

    return NULL;
}

void *server_handle_connection(void *arg)
{
    int client_fd = *(int*)arg;
    free(arg); // was malloc'd before pthread_create

    char hdrbuf[2048];
    char sender_ip[64] = "unknown";
    char is_chatroom[8] = "0";
    char content_len_str[16] = "0";

    SSL *ssl = SSL_new(ssl_ctx);
    SSL_set_fd(ssl, client_fd);

    if (SSL_accept(ssl) <= 0) {
        ERR_print_errors_fp(stderr);
        SSL_free(ssl);
        close(client_fd);
        return NULL;
    }

    // Read until we have the full headers (\r\n\r\n) to find body
    int total = 0;
    while (total < (int)sizeof(hdrbuf) - 1) {
        int n = SSL_read(ssl, hdrbuf + total, sizeof(hdrbuf) - 1 - total);
        if (n <= 0) {
            SSL_shutdown(ssl);
            SSL_free(ssl);
            close(client_fd);
            return NULL;
        }
        total += n;
        hdrbuf[total] = '\0';
        if (strstr(hdrbuf, "\r\n\r\n")) break;
    }

    parse_header(hdrbuf, "X-Sender-IP:",    sender_ip,       sizeof(sender_ip));
    parse_header(hdrbuf, "X-Chatroom:",     is_chatroom,     sizeof(is_chatroom));
    parse_header(hdrbuf, "Content-Length:", content_len_str, sizeof(content_len_str));

    int msg_len = atoi(content_len_str);

    // Locate where body starts 
    const char *hdr_end = strstr(hdrbuf, "\r\n\r\n");
    if (!hdr_end || msg_len <= 0) {
        const char *bad = "HTTP/1.1 400 Bad Request\r\n\r\n";
        SSL_write(ssl, bad, strlen(bad));
        SSL_shutdown(ssl);
        SSL_free(ssl);
        close(client_fd);
        return NULL;
    }
    hdr_end += 4; // skip past \r\n\r\n

    int body_in_hdr = total - (int)(hdr_end - hdrbuf);

    // Allocate exact-size body buffer
    char *body = malloc(msg_len);
    if (!body) {
        SSL_shutdown(ssl);
        SSL_free(ssl);
        close(client_fd);
        return NULL;
    }
    if (body_in_hdr > 0)
        memcpy(body, hdr_end, body_in_hdr < msg_len ? body_in_hdr : msg_len);

    // Read any remaining body bytes that hadn't arrived yet
    int body_read = body_in_hdr;
    while (body_read < msg_len) {
        int n = SSL_read(ssl, body + body_read, msg_len - body_read);
        if (n <= 0) {
            free(body);
            SSL_shutdown(ssl);
            SSL_free(ssl);
            close(client_fd);
            return NULL;
        }
        body_read += n;
    }

    printf("KeyCipher: message from %s | chatroom = %s | len = %d\n", sender_ip, is_chatroom, msg_len);

    int ret;
    if (atoi(is_chatroom)) {
        ret = chatroom_receive(body, sender_ip);
    } else {
        if (msg_len >= (int)sizeof(kernel_msg_t)) {
            kernel_msg_t *kmsg = (kernel_msg_t *)body;

            // Inject sender IP into author field 
            strncpy(kmsg->author, sender_ip, sizeof(kmsg->author) - 1);
            kmsg->author[sizeof(kmsg->author) - 1] = '\0';

            // Save encrypted data to queue file before kernel decrypts it 
            FILE *enc_f = fopen(ENC_QUEUE, "ab");
            if (enc_f) {
                char enc_record[256] = {0};
                int enc_len = kmsg->len > 0 && kmsg->len < 256 ? kmsg->len : 0;
                memcpy(enc_record, kmsg->data, enc_len);
                fwrite(enc_record, 256, 1, enc_f);
                fclose(enc_f);
            }
        }

        int dev_fd = open(DEVICE_PATH, O_WRONLY | O_NONBLOCK);
        if (dev_fd < 0) {
            perror("KeyCipher: failed to open device");
            free(body);
            const char *err = "HTTP/1.1 503 Service Unavailable\r\n\r\n";
            SSL_write(ssl, err, strlen(err));
            SSL_shutdown(ssl);
            SSL_free(ssl);
            close(client_fd);
            return NULL;
        }
        ret = write(dev_fd, body, msg_len);
        close(dev_fd);

        // Register in userspace inbox so API can show it in the frontend 
        if (ret > 0 && msg_len >= (int)sizeof(kernel_msg_t))
            direct_add_to_inbox((kernel_msg_t *)body);
    }

    free(body);

    if (ret < 0) {
        printf("KeyCipher: FIFO full, sending 429 to %s\n", sender_ip);
        const char *busy = "HTTP/1.1 429 Too Many Requests\r\n\r\n";
        SSL_write(ssl, busy, strlen(busy));
    } else {
        printf("KeyCipher: Received and accepted message from %s\n", sender_ip);
        const char *ok = "HTTP/1.1 200 OK\r\nContent-Length: 0\r\n\r\n";
        SSL_write(ssl, ok, strlen(ok));
    }

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
