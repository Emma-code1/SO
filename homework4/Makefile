CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -pthread
LDFLAGS = -pthread -lrt

SRC = src/chat_server.c src/chat_client.c src/ipc_sockets.c src/ipc_mq.c src/message.c src/utils.c
OBJ = $(SRC:.c=.o)
BIN_DIR = bin

.PHONY: all clean

all: $(BIN_DIR) server client

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

server: $(SRC) $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/chat_server src/chat_server.c src/ipc_sockets.c src/ipc_mq.c src/message.c src/utils.c $(LDFLAGS)

client: $(SRC) $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/chat_client src/chat_client.c src/ipc_sockets.c src/message.c src/utils.c $(LDFLAGS)

clean:
	rm -rf $(BIN_DIR) *.o
