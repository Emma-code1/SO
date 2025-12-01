#ifndef CHAT_H
#define CHAT_H

#include "message.h"

#define SERVER_SOCKET_PATH "/tmp/chat.sock"
#define SERVER_PORT 8888 /* unused for unix socket but kept for logs */
#define SERVER_MQ_NAME "/chat_admin_mq"
#define MAX_CLIENTS 64

typedef struct client {
    int fd;
    int id;
    char username[USERNAME_MAX];
    struct client *next;
} client_t;

#endif // CHAT_H
