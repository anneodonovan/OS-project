#ifndef SERVER_H
#define SERVER_H

int server_init(int port);
void *server_accept_loop(void *arg);
void *server_handle_connection(void *arg);
void server_stop(void);

#endif 
