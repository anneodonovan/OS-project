#include <stdio.h>
#include <string.h>
#include "handlers.h"
#include "../messaging/chatroom.h"
#include "../messaging/direct.h"
#include "../network/peer_manager.h"

/*
 * handle_get_stats - return JSON with current system state
 * - open and parse /proc/keycipher/stats for buffer levels
 * - call peer_manager_get_all() for peer statuses
 * - call chatroom_get_semaphore_count() for live semaphore value
 * - build JSON: { incoming_used, incoming_free, chatroom_used,
 *                 chatroom_free, semaphore_count, peers: [...] }
 * - write HTTP 200 + JSON to client_fd
 */
void handle_get_stats(int client_fd)
{
    /* TODO: implement stats JSON response */
}

/*
 * handle_get_messages - return JSON list of messages in incoming FIFO
 * - read message queue from userspace buffer (populated by direct_receive_loop)
 * - build JSON array: [{ id, sender, timestamp, encrypted_preview }]
 * - write HTTP 200 + JSON to client_fd
 * NOTE: messages are NOT decrypted here, only previewed as encrypted
 *       decryption happens when user clicks READ
 */
void handle_get_messages(int client_fd)
{
    /* TODO: implement message list JSON response */
}

/*
 * handle_get_chatroom - return JSON list of chatroom messages
 * - similar to handle_get_messages but for chatroom buffer
 * - include semaphore_count in response so UI can show live counter
 */
void handle_get_chatroom(int client_fd)
{
    /* TODO: implement chatroom message list */
}

/*
 * handle_read_one - decrypt and return one specific message
 * - find message by id in userspace queue
 * - open /dev/keycipher_in, trigger ioctl for single read
 * - kernel decrypts and returns plaintext
 * - this frees one FIFO slot: semaphore count increases
 * - any blocked remote peer that was waiting for space will now unblock
 * - return JSON: { id, sender, plaintext, timestamp }
 */
void handle_read_one(int client_fd, const char *message_id)
{
    /* TODO: implement single message decrypt via ioctl */
}

/*
 * handle_read_all - flush entire incoming FIFO
 * - open /dev/keycipher_in
 * - ioctl(fd, KEYCIPHER_FLUSH_IN)
 * - kernel decrypts all messages and returns them
 * - all FIFO slots freed: semaphore jumps to full capacity
 * - all blocked remote peers unblock simultaneously
 * - return JSON array of all decrypted messages
 */
void handle_read_all(int client_fd)
{
    /* TODO: implement KEYCIPHER_FLUSH_IN ioctl + return all messages */
}

/*
 * handle_send_direct - send a direct message to a specific peer
 * - parse body JSON for { target_ip, message }
 * - find peer by ip in peers[]
 * - call direct_send(peer, message)
 * - return JSON: { status, encrypted_preview }
 */
void handle_send_direct(int client_fd, const char *body)
{
    /* TODO: implement direct send handler */
}

/*
 * handle_send_chatroom - broadcast a message to all peers via chatroom
 * - parse body JSON for { message }
 * - call chatroom_send(message)
 * - return JSON: { status, encrypted_preview, peers_notified }
 */
void handle_send_chatroom(int client_fd, const char *body)
{
    /* TODO: implement chatroom send handler */
}
