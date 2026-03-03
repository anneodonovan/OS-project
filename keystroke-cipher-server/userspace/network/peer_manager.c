#include "peer_manager.h"

static peer_t peers[MAX_PEERS];
static int    peer_count = 0;

int peer_manager_init(const char *config_file)
{
    /* open config_file (peers.conf)
       parse each non-comment line as an IP address
       populate peers[] with ip, default port 8443, PEER_OFFLINE status
       return number of peers loaded */
}

peer_t *peer_get_all(int *count)
{
    /* set *count = peer_count
       return peers[] array pointer
       used by handlers.c for GET /api/peers */
}

void peer_set_status(const char *ip, peer_status_t status)
{
    /* find peer in peers[] by IP string match
       update peer->status
       PEER_BLOCKED causes frontend to show spinner on that peer
       PEER_CONNECTED resumes normal send flow */
}

void peer_manager_cleanup(void)
{
    /* close all open socket_fd connections
       reset peer_count to 0 */
}
