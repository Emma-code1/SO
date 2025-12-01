#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "../include/ipc.h"

int ipc_socket_server_init(const char *socket_path, int backlog) {
    int server_fd;
    struct sockaddr_un addr;

    if ((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return -1;
    }

    unlink(socket_path); // remove if exists

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind");
        close(server_fd);
        return -1;
    }

    if (listen(server_fd, backlog) == -1) {
        perror("listen");
        close(server_fd);
        return -1;
    }

    return server_fd;
}

int ipc_socket_accept_client(int server_fd) {
    int client_fd = accept(server_fd, NULL, NULL);
    if (client_fd == -1) {
        if (errno != EINTR)
            perror("accept");
        return -1;
    }
    return client_fd;
}

int ipc_socket_send(int client_fd, const message_t *msg) {
    ssize_t sent = send(client_fd, msg, sizeof(message_t), 0);
    if (sent != sizeof(message_t)) {
        if (sent == -1) perror("send");
        return -1;
    }
    return 0;
}

int ipc_socket_recv(int client_fd, message_t *msg) {
    ssize_t r = recv(client_fd, msg, sizeof(message_t), 0);
    if (r == 0) return 1; // disconnected
    if (r == -1) {
        if (errno == EINTR) return -2;
        perror("recv");
        return -1;
    }
    if (r != sizeof(message_t)) {
        // partial read (unlikely), treat as error
        return -1;
    }
    return 0;
}

void ipc_socket_close_client(int client_fd) {
    if (client_fd >= 0) close(client_fd);
}

void ipc_socket_server_cleanup(const char *socket_path, int server_fd) {
    if (server_fd >= 0) close(server_fd);
    unlink(socket_path);
}
