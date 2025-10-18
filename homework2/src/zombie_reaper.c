#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void reap_explicit(void) {
    while (1) {
        pid_t pid = waitpid(-1, NULL, WNOHANG);
        if (pid <= 0) break;
        printf("Reaped zombie: %d\n", pid);
    }
}

void sigchld_handler(int sig) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

void setup_auto_reaper(void) {
    struct sigaction sa;
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigaction(SIGCHLD, &sa, NULL);
}

void setup_ignore_reaper(void) {
    signal(SIGCHLD, SIG_IGN);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <strategy: 1, 2, or 3>\n", argv[0]);
        return -1;
    }

    int strategy = atoi(argv[1]);

    // Create 10 children
    for (int i = 0; i < 10; i++) {
        if (fork() == 0) {
            sleep(rand() % 3);
            exit(i);
        }
    }

    // Use chosen strategy
    switch(strategy) {
        case 1: reap_explicit(); break;
        case 2: setup_auto_reaper(); break;
        case 3: setup_ignore_reaper(); break;
        default: fprintf(stderr, "Invalid strategy\n"); return -1;
    }

    sleep(5);  // Wait for all children

    // Verify no zombies
    system("echo 'Remaining zombies:'; ps aux | grep Z | grep -v grep");

    return 0;
}
