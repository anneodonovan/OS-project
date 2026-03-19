#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "api_server.h"
#include "handlers.h"

#define PORT 8080
#define BUF_SIZE 4096

static void parse_request_line(const char *req, char *method, char *path) {
    sscanf(req, "%s %s", method, path); //extracts: method=GET or POST; path='/api/...'
}

static const char *extract_body(const char *req) {
    const char *body = strstr(req, "\r\n\r\n"); //skips blank line before JSON
    return body ? body + 4 : "";
}

//starts HTTP server (localhost:8080) to communicate with NodeJS frontend
void *api_server_start(void *arg) {
    //new socket setup
    int server_fd, client_fd;
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return NULL;
    }

    //prevents address already in use
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    //local server
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);  //binds to all interfaces (both its 192.168.1.x addr and 127.0.0.1 localhost)
    addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(server_fd);
        return NULL;
    }

    //tries to open to TCP connection attempts post set up; tests 10 queued connections
    if (listen(server_fd, 10) < 0) {
        perror("listen");
        close(server_fd);
        return NULL;
    }

    printf("api: C HTTP server running on http://127.0.0.1:%d\n", PORT); //debug

    //live server:
    while (1) {
        //accepted client requests
        client_fd = accept(server_fd, (struct sockaddr*)&addr, &addrlen);
        if (client_fd < 0) {
            perror("accept");
            continue;
        }

        char buffer[BUF_SIZE] = {0};
        read(client_fd, buffer, BUF_SIZE - 1);

        char method[8] = {0};
        char path[256] = {0};
        parse_request_line(buffer, method, path);

        const char *body = extract_body(buffer);

        /* ---------------- ROUTING ---------------- */

        if (strcmp(method, "GET") == 0 && strcmp(path, "/api/stats") == 0) {
            handle_get_stats(client_fd);
        }

        else if (strcmp(method, "GET") == 0 && strcmp(path, "/api/messages") == 0) {
            handle_get_messages(client_fd);
        }

        else if (strcmp(method, "GET") == 0 && strcmp(path, "/api/chatroom") == 0) {
            handle_get_chatroom(client_fd);
        }

        else if (strcmp(method, "GET") == 0 && strcmp(path, "/api/outbox") == 0) {
            handle_get_outbox(client_fd);
        }

        else if (strcmp(method, "POST") == 0 && strcmp(path, "/api/read/all") == 0) {
            handle_read_all(client_fd);
        }

        /* POST /api/read/:id */
        else if (strcmp(method, "POST") == 0 && strncmp(path, "/api/read/", 10) == 0) {
            const char *id = path + 10;  // everything after /api/read/
            handle_read_one(client_fd, id);
        }

        else if (strcmp(method, "POST") == 0 && strcmp(path, "/api/send") == 0) {
            handle_send_direct(client_fd, body);
        }

        else if (strcmp(method, "POST") == 0 && strcmp(path, "/api/send/chatroom") == 0) {
            handle_send_chatroom(client_fd, body);
        }

        else {
            const char *msg = "HTTP/1.1 404 Not Found\r\n"
                              "Content-Type: text/plain\r\n"
                              "Content-Length: 9\r\n\r\nNot Found";
            write(client_fd, msg, strlen(msg)); //sends error to client
        }

        close(client_fd);
    }

    close(server_fd);
    return NULL;
}
