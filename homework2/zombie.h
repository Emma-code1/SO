#ifndef ZOMBIE_H
#define ZOMBIE_H
#include <sys/types.h>

typedef struct {
    int zombies_created;
    int zombies_reaped;
    int zombies_active;
} zombie_stats_t;

void zombie_init(void);
pid_t zombie_safe_fork(void);
int zombie_safe_spawn(const char *command, char *args[]);
void zombie_get_stats(zombie_stats_t *stats);

#endif
