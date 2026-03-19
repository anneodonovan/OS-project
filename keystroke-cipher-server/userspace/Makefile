CC      = gcc
CFLAGS  = -Wall -Wextra
LDFLAGS = -pthread -lssl -lcrypto

SRCS = main.c \
       network/server.c \
       network/client.c \
       network/peer_manager.c \
       messaging/direct.c \
       messaging/chatroom.c \
       api/api_server.c \
       api/handlers.c

TARGET  = keycipher_daemon
INBOX   = inbox_terminal

all: $(TARGET) $(INBOX)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(INBOX): inbox_terminal.c
	$(CC) -Wall -o $@ $^

clean:
	rm -f $(TARGET) $(INBOX)
