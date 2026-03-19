#ifndef HANDLERS_H
#define HANDLERS_H

void handle_get_stats(int client_fd);
void handle_get_messages(int client_fd);
void handle_get_outbox(int client_fd);
void handle_get_chatroom(int client_fd);
void handle_read_one(int client_fd, const char *message_id);
void handle_read_all(int client_fd);
void handle_send_direct(int client_fd, const char *body);
void handle_send_chatroom(int client_fd, const char *body);

#endif
