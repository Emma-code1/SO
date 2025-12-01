#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../include/message.h"

void message_init(message_t *m) {
    if (!m) return;
    m->type = MSG_ERROR;
    memset(m->from_user, 0, USERNAME_MAX);
    memset(m->to_user, 0, USERNAME_MAX);
    memset(m->text, 0, MSG_MAX_SIZE);
    m->timestamp = time(NULL);
    m->client_id = -1;
}
