#include "procman.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <signal.h>
#include <ctype.h>

process_info_t process_table[MAX_PROCESSES];
int process_count = 0;

int create_process(const char *command, char *args[]) {
    if (process_count >= MAX_PROCESSES) {
        fprintf(stderr, "Maximum number of processes reached\n");
        return -1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return -1;
    } else if (pid == 0) {
        execvp(command, args);
        perror("execvp failed");
        exit(EXIT_FAILURE);
    } else {
        strncpy(process_table[process_count].command, command, sizeof(process_table[process_count].command) - 1);
        for (int i = 1; args[i] != NULL; i++) {
            strncat(process_table[process_count].command, " ", sizeof(process_table[process_count].command) - strlen(process_table[process_count].command) - 1);
            strncat(process_table[process_count].command, args[i], sizeof(process_table[process_count].command) - strlen(process_table[process_count].command) - 1);
        }
        process_table[process_count].pid = pid;
        process_table[process_count].start_time = time(NULL);
        process_table[process_count].status = 0;
        process_count++;
        return pid;
    }
}

int check_process_status(pid_t pid) {
    int status;
    if (waitpid(pid, &status, WNOHANG) == 0) {
        return 1; // Process is still running
    } else if (waitpid(pid, &status, WNOHANG) == pid) {
        return 0; // Process has terminated
    } else {
        perror("waitpid failed");
        return -1; // Error
    }
}

int terminate_process(pid_t pid, int force) {
    int sig = force ? SIGKILL : SIGTERM;
    if (kill(pid, sig) < 0) {
        perror("kill failed");
        return -1;
    }
    waitpid(pid, NULL, 0);
    for (int i = 0; i < process_count; i++) {
        if (process_table[i].pid == pid) {
            process_table[i].status = 1;
        }
    }
    return 0;
}

void list_processes(void) {
    printf("PID\tCOMMAND\t\t\tRUNTIME\t\tSTATUS\n");
    printf("----\t----------------\t--------\t--------\n");
    for (int i = 0; i < process_count; i++) {
        time_t current_time = time(NULL);
        double runtime = difftime(current_time, process_table[i].start_time);
        int minutes = (int)runtime / 60;
        int seconds = (int)runtime % 60;
        printf("%d\t%s\t%02d:%02d\t%s\n",
               process_table[i].pid,
               process_table[i].command,
               minutes, seconds,
               process_table[i].status == 0 ? "Running" : "Terminated");
    }
}

void wait_all_processes(void) {
    for (int i = 0; i < process_count; i++) {
        if (process_table[i].status == 0) {
            waitpid(process_table[i].pid, NULL, 0);
            process_table[i].status = 1;
        }
    }
    printf("All processes have terminated\n");
}

void sigint_handler(int signum) {
    (void)signum;
    printf("\nShutting down gracefully...\n");
    for (int i = 0; i < process_count; i++) {
        if (process_table[i].status == 0) {
            terminate_process(process_table[i].pid, 0);
        }
    }
    exit(EXIT_SUCCESS);
}

void sigchld_handler(int signum) {
    (void)signum;
    pid_t pid;
    while ((pid = waitpid(-1, NULL, WNOHANG)) > 0) {
        for (int i = 0; i < process_count; i++) {
            if (process_table[i].pid == pid) {
                process_table[i].status = 1;
            }
        }
    }
}

void setup_signal_handlers(void) {
    struct sigaction sa_int, sa_chld;

    sa_int.sa_handler = sigint_handler;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;
    sigaction(SIGINT, &sa_int, NULL);

    sa_chld.sa_handler = sigchld_handler;
    sigemptyset(&sa_chld.sa_mask);
    sa_chld.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigaction(SIGCHLD, &sa_chld, NULL);
}

void print_process_tree(pid_t root_pid, int depth) {
    char stat_path[256];
    snprintf(stat_path, sizeof(stat_path), "/proc/%d/stat", root_pid);

    FILE *stat_file = fopen(stat_path, "r");
    if (!stat_file) {
        return;
    }

    char stat_line[1024];
    if (fgets(stat_line, sizeof(stat_line), stat_file)) {
        char comm[256];
        char *temp = strtok(stat_line, " ");
        temp = strtok(NULL, " "); // Skip pid
        strncpy(comm, strtok(NULL, " "), sizeof(comm));
        size_t len = strlen(comm);
        if (len > 0 && comm[len-1] == ')') {
            comm[len-1] = '\0'; // Remove trailing ')'
        }

        for (int i = 0; i < depth; i++) {
            printf("  ");
        }
        if (depth > 0) {
            printf("├─ ");
        }
        printf("[%d] %s\n", root_pid, comm);

        DIR *proc_dir = opendir("/proc");
        if (proc_dir) {
            struct dirent *entry;
            while ((entry = readdir(proc_dir)) != NULL) {
                if (entry->d_type == DT_DIR) {
                    char *endptr;
                    long pid = strtol(entry->d_name, &endptr, 10);
                    if (*endptr == '\0') { // It's a numeric directory
                        char child_stat_path[256];
                        snprintf(child_stat_path, sizeof(child_stat_path), "/proc/%ld/stat", pid);

                        FILE *child_stat_file = fopen(child_stat_path, "r");
                        if (child_stat_file) {
                            char child_stat_line[1024];
                            if (fgets(child_stat_line, sizeof(child_stat_line), child_stat_file)) {
                                char child_comm[256];
                                strtok(child_stat_line, " "); // Skip pid
                                strncpy(child_comm, strtok(NULL, " "), sizeof(child_comm));
                                size_t child_len = strlen(child_comm);
                                if (child_len > 0 && child_comm[child_len-1] == ')') {
                                    child_comm[child_len-1] = '\0';
                                }

                                // Skip remaining tokens until ppid (4th token)
                                for (int i = 0; i < 2; i++) {
                                    strtok(NULL, " ");
                                }
                                int child_ppid = atoi(strtok(NULL, " "));

                                if (child_ppid == root_pid) {
                                    print_process_tree(pid, depth + 1);
                                }
                            }
                            fclose(child_stat_file);
                        }
                    }
                }
            }
            closedir(proc_dir);
        }
    }
    fclose(stat_file);
}

void print_help() {
    printf("Available commands:\n");
    printf("  create <command> [args...] - Create new process\n");
    printf("  list                       - List all processes\n");
    printf("  kill <pid> [force]         - Terminate process\n");
    printf("  tree                       - Show process tree\n");
    printf("  wait                       - Wait for all processes\n");
    printf("  quit                       - Exit program\n");
}

int main() {
    setup_signal_handlers();

    char input[256];
    printf("ProcMan> ");
    while (fgets(input, sizeof(input), stdin)) {
        input[strcspn(input, "\n")] = '\0';

        char *token = strtok(input, " ");
        if (!token) {
            printf("ProcMan> ");
            continue;
        }

        if (strcmp(token, "create") == 0) {
            char *command = strtok(NULL, " ");
            if (!command) {
                printf("Usage: create <command> [args...]\n");
            } else {
                char *args[256];
                args[0] = command;
                int i = 1;
                while ((args[i] = strtok(NULL, " ")) != NULL) {
                    i++;
                }
                pid_t pid = create_process(command, args);
                if (pid > 0) {
                    printf("Created process %d\n", pid);
                }
            }
        } else if (strcmp(token, "list") == 0) {
            list_processes();
        } else if (strcmp(token, "kill") == 0) {
            char *pid_str = strtok(NULL, " ");
            char *force_str = strtok(NULL, " ");
            if (!pid_str) {
                printf("Usage: kill <pid> [force]\n");
            } else {
                pid_t pid = atoi(pid_str);
                int force = force_str && strcmp(force_str, "force") == 0;
                terminate_process(pid, force);
            }
        } else if (strcmp(token, "tree") == 0) {
            print_process_tree(getpid(), 0);
        } else if (strcmp(token, "wait") == 0) {
            wait_all_processes();
        } else if (strcmp(token, "quit") == 0) {
            break;
        } else if (strcmp(token, "help") == 0) {
            print_help();
        } else {
            printf("Unknown command: %s\n", token);
        }
        printf("ProcMan> ");
    }

    printf("Shutting down...\n");
    return 0;
}
