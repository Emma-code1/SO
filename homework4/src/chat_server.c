#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#include "../include/chat.h"
#include "../include/ipc.h"
#include "../include/message.h"

extern int ipc_socket_server_init(const char *, int);
extern int ipc_socket_accept_client(int);
extern int ipc_socket_send(int, const message_t *);
extern int ipc_socket_recv(int, message_t *);
extern void ipc_socket_server_cleanup(const char *, int);

extern int ipc_mq_init(const char *);
extern int ipc_mq_send_cmd(const char *, const message_t *);
extern int ipc_mq_receive_cmd(const char *, message_t *);
extern int ipc_mq_cleanup(const char *);

static client_t *clients = NULL;
static pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;
static int next_client_id = 1;
static int server_fd = -1;
static int running = 1;

void log_server(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    printf("[SERVER] ");
    vprintf(fmt, ap);
    printf("\n");
    va_end(ap);
}

void add_client(client_t *c) {
    pthread_mutex_lock(&clients_mutex);
    c->next = clients;
    clients = c;
    pthread_mutex_unlock(&clients_mutex);
}

void remove_client_by_fd(int fd) {
    pthread_mutex_lock(&clients_mutex);
    client_t **cur = &clients;
    while (*cur) {
        if ((*cur)->fd == fd) {
            client_t *tofree = *cur;
            *cur = tofree->next;
            close(tofree->fd);
            free(tofree);
            break;
        }
        cur = &((*cur)->next);
    }
    pthread_mutex_unlock(&clients_mutex);
}

client_t *find_client_by_name(const char *name) {
    pthread_mutex_lock(&clients_mutex);
    client_t *cur = clients;
    while (cur) {
        if (strncmp(cur->username, name, USERNAME_MAX) == 0) {
            pthread_mutex_unlock(&clients_mutex);
            return cur;
        }
        cur = cur->next;
    }
    pthread_mutex_unlock(&clients_mutex);
    return NULL;
}

void broadcast_message(const message_t *m) {
    pthread_mutex_lock(&clients_mutex);
    client_t *cur = clients;
    while (cur) {
        ipc_socket_send(cur->fd, m);
        cur = cur->next;
    }
    pthread_mutex_unlock(&clients_mutex);
}

void send_private(const char *to, const message_t *m) {
    client_t *c = find_client_by_name(to);
    if (c) {
        ipc_socket_send(c->fd, m);
    }
}

void handle_client_recv(client_t *c) {
    message_t msg;
    while (running) {
        int r = ipc_socket_recv(c->fd, &msg);
        if (r == 1) {
            // disconnected
            log_server("[INFO] Client '%s' disconnected (FD: %d)", c->username, c->fd);
            message_t leave;
            leave.type = MSG_LEAVE;
            strncpy(leave.from_user, c->username, USERNAME_MAX-1);
            broadcast_message(&leave);
            remove_client_by_fd(c->fd);
            return;
        } else if (r == -2) {
            continue;
        } else if (r == -1) {
            // error
            log_server("[ERROR] recv from %s", c->username);
            remove_client_by_fd(c->fd);
            return;
        } else {
            // process message
            if (msg.type == MSG_BROADCAST) {
                log_server("[MSG] %s -> ALL: %s", msg.from_user, msg.text);
                broadcast_message(&msg);
            } else if (msg.type == MSG_PRIVATE) {
                log_server("[MSG] %s -> %s: %s", msg.from_user, msg.to_user, msg.text);
                send_private(msg.to_user, &msg);
            } else if (msg.type == MSG_LIST_USERS) {
                // build response
                message_t resp;
                resp.type = MSG_USER_LIST;
                resp.timestamp = time(NULL);
                char buf[MSG_MAX_SIZE] = {0};
                int first = 1;
                pthread_mutex_lock(&clients_mutex);
                client_t *cur = clients;
                while (cur) {
                    if (!first) strncat(buf, ", ", sizeof(buf)-strlen(buf)-1);
                    strncat(buf, cur->username, sizeof(buf)-strlen(buf)-1);
                    first = 0;
                    cur = cur->next;
                }
                pthread_mutex_unlock(&clients_mutex);
                strncpy(resp.text, buf, MSG_MAX_SIZE-1);
                ipc_socket_send(c->fd, &resp);
            } else if (msg.type == MSG_PING) {
                message_t p;
                p.type = MSG_PONG;
                ipc_socket_send(c->fd, &p);
            }
        }
    }
}

void *client_thread(void *arg) {
    client_t *c = (client_t *)arg;
    handle_client_recv(c);
    return NULL;
}

void handle_admin_command(const message_t *cmd) {
    if (cmd->type != MSG_BROADCAST && cmd->type != MSG_ERROR && cmd->type != MSG_USER_LIST) {
        // interpret text as admin command
    }
    // commands encoded in text: /list, /stats, /kick <user>, /shutdown, /broadcast <msg>
    if (strncmp(cmd->text, "/shutdown", 9) == 0) {
        log_server("[ADMIN] shutdown received");
        running = 0;
    } else if (strncmp(cmd->text, "/list", 5) == 0) {
        pthread_mutex_lock(&clients_mutex);
        client_t *cur = clients;
        log_server("[ADMIN] Connected users:");
        while (cur) {
            log_server(" - %s (FD: %d)", cur->username, cur->fd);
            cur = cur->next;
        }
        pthread_mutex_unlock(&clients_mutex);
    } else if (strncmp(cmd->text, "/kick ", 6) == 0) {
        char uname[USERNAME_MAX];
        sscanf(cmd->text + 6, "%31s", uname);
        client_t *c = find_client_by_name(uname);
        if (c) {
            log_server("[ADMIN] Kicking %s", uname);
            remove_client_by_fd(c->fd);
        } else {
            log_server("[ADMIN] user %s not found", uname);
        }
    } else if (strncmp(cmd->text, "/broadcast ", 11) == 0) {
        message_t msg;
        msg.type = MSG_BROADCAST;
        strncpy(msg.from_user, "SERVER", USERNAME_MAX-1);
        strncpy(msg.text, cmd->text + 11, MSG_MAX_SIZE-1);
        broadcast_message(&msg);
    } else if (strncmp(cmd->text, "/stats", 6) == 0) {
        int count = 0;
        pthread_mutex_lock(&clients_mutex);
        client_t *cur = clients;
        while (cur) { count++; cur = cur->next; }
        pthread_mutex_unlock(&clients_mutex);
        log_server("[ADMIN] Active users: %d", count);
    }
}

void *admin_mq_thread(void *arg) {
    (void)arg;
    message_t cmd;
    while (running) {
        int r = ipc_mq_receive_cmd(SERVER_MQ_NAME, &cmd);
        if (r == 0) {
            handle_admin_command(&cmd);
        } else {
            // sleep a bit to avoid busy loop
            usleep(200000);
        }
    }
    return NULL;
}

void sigint_handler(int sig) {
    (void)sig;
    running = 0;
}

int main(int argc, char *argv[]) {
    (void)argc; (void)argv;
    signal(SIGINT, sigint_handler);
    log_server("Chat server started on port %d", SERVER_PORT);
    log_server("Using IPC methods: UNIX_SOCKETS + POSIX_MQ");

    if (ipc_mq_init(SERVER_MQ_NAME) != 0) {
        log_server("[WARN] could not init message queue admin interface");
    }

    server_fd = ipc_socket_server_init(SERVER_SOCKET_PATH, 10);
    if (server_fd < 0) {
        log_server("[ERROR] failed to start unix socket server");
        return 1;
    }

    pthread_t admin_thread;
    pthread_create(&admin_thread, NULL, admin_mq_thread, NULL);

    fd_set readfds;
    while (running) {
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);
        struct timeval tv = {1, 0}; // 1 second timeout
        int maxfd = server_fd;

        int sel = select(maxfd+1, &readfds, NULL, NULL, &tv);
        if (sel > 0 && FD_ISSET(server_fd, &readfds)) {
            int client_fd = ipc_socket_accept_client(server_fd);
            if (client_fd >= 0) {
                // initial handshake: expect MSG_JOIN with username
                message_t join;
                int r = ipc_socket_recv(client_fd, &join);
                if (r == 0 && join.type == MSG_JOIN) {
                    client_t *c = calloc(1, sizeof(client_t));
                    c->fd = client_fd;
                    c->id = next_client_id++;
                    strncpy(c->username, join.from_user, USERNAME_MAX-1);
                    add_client(c);
                    log_server("[INFO] Client '%s' connected (FD: %d)", c->username, c->fd);

                    // notify others
                    message_t notice;
                    notice.type = MSG_JOIN;
                    strncpy(notice.from_user, c->username, USERNAME_MAX-1);
                    broadcast_message(&notice);

                    // send welcome to client
                    message_t welcome;
                    welcome.type = MSG_BROADCAST;
                    strncpy(welcome.text, "Connected to server!", MSG_MAX_SIZE-1);
                    ipc_socket_send(c->fd, &welcome);

                    pthread_t tid;
                    pthread_create(&tid, NULL, client_thread, c);
                } else {
                    // invalid handshake
                    close(client_fd);
                }
            }
        } else if (sel == 0) {
            // timeout - allow loop to check running
            continue;
        }
    }

    // shutdown: notify clients and cleanup
    message_t sd;
    sd.type = MSG_BROADCAST;
    strncpy(sd.from_user, "SERVER", USERNAME_MAX-1);
    strncpy(sd.text, "Server is shutting down", MSG_MAX_SIZE-1);
    broadcast_message(&sd);

    // cleanup
    ipc_socket_server_cleanup(SERVER_SOCKET_PATH, server_fd);
    ipc_mq_cleanup(SERVER_MQ_NAME);

    log_server("Server shutdown complete");
    return 0;
}
