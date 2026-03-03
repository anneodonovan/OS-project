CC     = gcc
CFLAGS = -Wall -Wextra -pthread -lssl -lcrypto

SRCS = main.c \
       network/server.c \
       network/client.c \
       network/peer_manager.c \
       messaging/direct.c \
       messaging/chatroom.c \
       api/api_server.c \
       api/handlers.c

TARGET = keycipher_daemon

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)
