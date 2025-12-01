#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "../include/ipc.h"
#include "../include/message.h"

extern int ipc_socket_server_init(const char *, int);
extern int ipc_socket_send(int, const message_t *);
extern int ipc_socket_recv(int, message_t *);
extern int ipc_socket_accept_client(int);

/* client-only helper to connect to unix socket */
int connect_unix_socket(const char *socket_path) {
    int fd;
    struct sockaddr_un addr;
    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return -1;
    }
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);
    if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("connect");
        close(fd);
        return -1;
    }
    return fd;
}

int sockfd = -1;
char username[USERNAME_MAX];

void *recv_thread(void *arg) {
    (void)arg;
    message_t msg;
    while (1) {
        int r = ipc_socket_recv(sockfd, &msg);
        if (r == 1) {
            printf("[SYSTEM] Server disconnected\n");
            close(sockfd);
            exit(0);
        } else if (r != 0) {
            usleep(100000);
            continue;
        } else {
            if (msg.type == MSG_BROADCAST) {
                if (strlen(msg.from_user) > 0)
                    printf("[%s] %s\n", msg.from_user, msg.text);
                else
                    printf("[SERVER] %s\n", msg.text);
            } else if (msg.type == MSG_PRIVATE) {
                printf("[ %s -> you ] %s\n", msg.from_user, msg.text);
            } else if (msg.type == MSG_USER_LIST) {
                printf("[SYSTEM] Users online: %s\n", msg.text);
            } else if (msg.type == MSG_PONG) {
                /* ignore for now */
            }
        }
    }
    return NULL;
}

void print_help() {
    printf("Commands:\n");
    printf("/help              - show this\n");
    printf("/users             - list online users\n");
    printf("/msg <user> <msg>  - private message\n");
    printf("/quit              - disconnect\n");
}

int main(int argc, char *argv[]) {
    (void)argc; (void)argv;
    printf("=== Chat Client ===\nUsername: ");
    if (fgets(username, sizeof(username), stdin) == NULL) return 1;
    username[strcspn(username, "\\n")] = 0;

    sockfd = connect_unix_socket(SERVER_SOCKET_PATH);
    if (sockfd < 0) {
        fprintf(stderr, "Failed to connect to server\n");
        return 1;
    }
    // send join
    message_t join;
    join.type = MSG_JOIN;
    strncpy(join.from_user, username, USERNAME_MAX-1);
    ipc_socket_send(sockfd, &join);

    printf("Connected to server!\n");
    message_t sys;
    if (ipc_socket_recv(sockfd, &sys) == 0) {
        if (sys.type == MSG_BROADCAST) {
            printf("[SYSTEM] %s\n", sys.text);
        }
    }

    pthread_t tid;
    pthread_create(&tid, NULL, recv_thread, NULL);

    char line[512];
    print_help();
    while (1) {
        printf("%s> ", username);
        if (!fgets(line, sizeof(line), stdin)) break;
        line[strcspn(line, "\\n")] = 0;
        if (strncmp(line, "/help", 5) == 0) {
            print_help();
        } else if (strncmp(line, "/users", 6) == 0) {
            message_t req;
            req.type = MSG_LIST_USERS;
            ipc_socket_send(sockfd, &req);
        } else if (strncmp(line, "/msg ", 5) == 0) {
            char to[USERNAME_MAX];
            char text[MSG_MAX_SIZE];
            if (sscanf(line + 5, "%31s %[^\n]", to, text) >= 1) {
                message_t pm;
                pm.type = MSG_PRIVATE;
                strncpy(pm.from_user, username, USERNAME_MAX-1);
                strncpy(pm.to_user, to, USERNAME_MAX-1);
                strncpy(pm.text, text, MSG_MAX_SIZE-1);
                ipc_socket_send(sockfd, &pm);
                printf("[You -> %s] %s\n", to, text);
            }
        } else if (strncmp(line, "/quit", 5) == 0) {
            message_t leave;
            leave.type = MSG_LEAVE;
            strncpy(leave.from_user, username, USERNAME_MAX-1);
            ipc_socket_send(sockfd, &leave);
            close(sockfd);
            printf("[SYSTEM] Goodbye!\n");
            exit(0);
        } else {
            message_t b;
            b.type = MSG_BROADCAST;
            strncpy(b.from_user, username, USERNAME_MAX-1);
            strncpy(b.text, line, MSG_MAX_SIZE-1);
            ipc_socket_send(sockfd, &b);
        }
    }

    return 0;
}
