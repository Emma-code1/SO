#ifndef IPC_H
#define IPC_H

#include "message.h"

/*
 * IPC abstraction: we provide two implementations:
 * - Unix domain sockets (clients <-> server)
 * - POSIX message queue (admin -> server commands, optional)
 */

/* Socket IPC functions */
int ipc_socket_server_init(const char *socket_path, int backlog);
int ipc_socket_accept_client(int server_fd);
int ipc_socket_send(int client_fd, const message_t *msg);
int ipc_socket_recv(int client_fd, message_t *msg);
void ipc_socket_close_client(int client_fd);
void ipc_socket_server_cleanup(const char *socket_path, int server_fd);

/* Message queue IPC (for admin/monitoring) */
int ipc_mq_init(const char *mq_name);
int ipc_mq_send_cmd(const char *mq_name, const message_t *msg);
int ipc_mq_receive_cmd(const char *mq_name, message_t *msg); // blocking
int ipc_mq_cleanup(const char *mq_name);

#endif // IPC_H
