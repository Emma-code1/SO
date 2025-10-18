#include "zombie.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>

static int zombies_created = 0;
static int zombies_reaped = 0;
static pthread_mutex_t stats_mutex = PTHREAD_MUTEX_INITIALIZER;

void sigchld_handler(int sig) {
    while (waitpid(-1, NULL, WNOHANG) > 0) {
        pthread_mutex_lock(&stats_mutex);
        zombies_reaped++;
        pthread_mutex_unlock(&stats_mutex);
    }
}

void zombie_init(void) {
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigaction(SIGCHLD, &sa, NULL);
}

pid_t zombie_safe_fork(void) {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        return 0;
    } else if (pid > 0) {
        // Parent process
        pthread_mutex_lock(&stats_mutex);
        zombies_created++;
        pthread_mutex_unlock(&stats_mutex);
    }
    return pid;
}

int zombie_safe_spawn(const char *command, char *args[]) {
    pid_t pid = fork();
    if (pid == 0) {
        execvp(command, args);
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        pthread_mutex_lock(&stats_mutex);
        zombies_created++;
        pthread_mutex_unlock(&stats_mutex);
        return pid;
    }
    return -1;
}

void zombie_get_stats(zombie_stats_t *stats) {
    pthread_mutex_lock(&stats_mutex);
    stats->zombies_created = zombies_created;
    stats->zombies_reaped = zombies_reaped;
    stats->zombies_active = zombies_created - zombies_reaped;
    pthread_mutex_unlock(&stats_mutex);
}
