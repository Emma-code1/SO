#include "zombie.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    zombie_init();

    zombie_stats_t stats;
    for (int i = 0; i < 5; i++) {
        pid_t pid = zombie_safe_fork();
        if (pid == 0) {
            sleep(1);
            exit(0);
        }
    }

    sleep(2);
    zombie_get_stats(&stats);
    printf("Zombies created: %d\n", stats.zombies_created);
    printf("Zombies reaped: %d\n", stats.zombies_reaped);
    printf("Zombies active: %d\n", stats.zombies_active);

    return 0;
}
