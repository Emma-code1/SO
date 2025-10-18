#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int create_zombies(int count) {
    for (int i = 0; i < count; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // Child process exits immediately
            exit(i);
        } else if (pid < 0) {
            perror("fork failed");
            return -1;
        } else {
            // Parent continues
            printf("Created zombie: PID %d (exit code %d)\n", pid, i);
        }
    }
    printf("Press Enter to exit and clean up zombies...\n");
    getchar();
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_zombies>\n", argv[0]);
        return -1;
    }
    int count = atoi(argv[1]);
    return create_zombies(count);
}
