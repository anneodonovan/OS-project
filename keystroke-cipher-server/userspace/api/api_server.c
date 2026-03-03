#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <netinet/in.h>
#include "api_server.h"
#include "handlers.h"

/*
 * api_server_start - start HTTP server on localhost:8080 for Node.js bridge
 * - bind to 127.0.0.1:8080 only (not exposed externally, Node.js bridges it)
 * - listen and accept connections in a loop
 * - for each connection: parse HTTP method + path, route to correct handler
 * - supported routes:
 *     GET  /api/stats           → handle_get_stats
 *     GET  /api/messages        → handle_get_messages
 *     GET  /api/chatroom        → handle_get_chatroom
 *     POST /api/read/:id        → handle_read_one
 *     POST /api/read/all        → handle_read_all
 *     POST /api/send            → handle_send_direct
 *     POST /api/send/chatroom   → handle_send_chatroom
 */
void *api_server_start(void *arg)
{
    /* TODO: implement simple HTTP routing server on port 8080 */
    return NULL;
}
