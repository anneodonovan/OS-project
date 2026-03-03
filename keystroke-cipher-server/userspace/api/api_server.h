#ifndef API_SERVER_H
#define API_SERVER_H

#define API_PORT 8080

/*
 * api_server_start - pthread entry point, starts HTTP server on port 8080
 * called from main.c via pthread_create
 */
void *api_server_start(void *arg);

#endif /* API_SERVER_H */
