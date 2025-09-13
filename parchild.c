#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <string.h>

int main() {
    // Commands to execute
    char *commands[][3] = {
        {"ls", "-l", NULL},
        {"date", NULL, NULL},
        {"whoami", NULL, NULL},
        {"pwd", NULL, NULL},
        {"ps", "aux", NULL}
    };
    int num_commands = 5;
    pid_t child_pids[num_commands];
    struct timeval start_time, end_time;
    double total_time;

    // Get start time
    gettimeofday(&start_time, NULL);

    printf("=== Process Monitor ===\n");
    printf("Parent PID: %d\n", getpid());
    printf("Creating child processes...\n\n");

    // Process creation loop
    for (int i = 0; i < num_commands; i++) {
        pid_t rc = fork();

        if (rc < 0) {
            // Handle fork failure
            fprintf(stderr, "Fork failed for command %s\n", commands[i][0]);
            exit(1);
        } else if (rc == 0) {
            // Child process
            printf("Child %d (PID: %d) executing: %s\n", i, getpid(), commands[i][0]);
            execvp(commands[i][0], commands[i]);

            // If execvp returns, it failed
            fprintf(stderr, "Exec failed for command: %s\n", commands[i][0]);
            exit(1);
        } else {
            // Parent process
            child_pids[i] = rc;
            printf("Parent created child %d with PID: %d\n", i, rc);
        }
    }

    printf("\nChild processes executing...\n");

    // Process monitoring loop
    for (int i = 0; i < num_commands; i++) {
        int status;
        pid_t wc = wait(&status);

        if (wc == -1) {
            fprintf(stderr, "Wait failed\n");
            continue;
        }

        // Find which child this was
        int child_index = -1;
        for (int j = 0; j < num_commands; j++) {
            if (child_pids[j] == wc) {
                child_index = j;
                break;
            }
        }

        
        if (WIFEXITED(status)) {
            printf("Child %d (PID: %d) completed with exit code: %d\n",
                   child_index, wc, WEXITSTATUS(status));
        } else {
            printf("Child %d (PID: %d) terminated abnormally\n",
                   child_index, wc);
        }
    }

    
    gettimeofday(&end_time, NULL);
    total_time = (end_time.tv_sec - start_time.tv_sec) +
                 (end_time.tv_usec - start_time.tv_usec) / 1e6;

    printf("\nTotal execution time: %.3f seconds\n", total_time);

    return 0;
}
