#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "../include/ipc.h"

int ipc_mq_init(const char *mq_name) {
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(message_t);
    attr.mq_curmsgs = 0;

    mqd_t mq = mq_open(mq_name, O_CREAT | O_RDONLY | O_NONBLOCK, 0644, &attr);
    if (mq == (mqd_t)-1) {
        if (errno == EEXIST) {
            mq = mq_open(mq_name, O_RDONLY | O_NONBLOCK);
            if (mq == (mqd_t)-1) { perror("mq_open existing"); return -1; }
            mq_close(mq);
            return 0;
        }
        perror("mq_open");
        return -1;
    }
    mq_close(mq);
    return 0;
}

int ipc_mq_send_cmd(const char *mq_name, const message_t *msg) {
    mqd_t mq = mq_open(mq_name, O_WRONLY);
    if (mq == (mqd_t)-1) {
        perror("mq_open send");
        return -1;
    }
    if (mq_send(mq, (const char*)msg, sizeof(message_t), 0) == -1) {
        perror("mq_send");
        mq_close(mq);
        return -1;
    }
    mq_close(mq);
    return 0;
}

/* Blocking receive (but caller can handle non-blocking) */
int ipc_mq_receive_cmd(const char *mq_name, message_t *msg) {
    mqd_t mq = mq_open(mq_name, O_RDONLY);
    if (mq == (mqd_t)-1) {
        perror("mq_open recv");
        return -1;
    }
    ssize_t r = mq_receive(mq, (char*)msg, sizeof(message_t), NULL);
    if (r == -1) {
        if (errno == EAGAIN) { mq_close(mq); return 1; }
        perror("mq_receive");
        mq_close(mq);
        return -1;
    }
    mq_close(mq);
    return 0;
}

int ipc_mq_cleanup(const char *mq_name) {
    if (mq_unlink(mq_name) == -1) {
        if (errno != ENOENT) { perror("mq_unlink"); return -1; }
    }
    return 0;
}
