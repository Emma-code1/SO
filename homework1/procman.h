#ifndef PROCMAN_H
#define PROCMAN_H

#include <sys/types.h>
#include <time.h>

#define MAX_PROCESSES 10

typedef struct {
    pid_t pid;
    char command[256];
    time_t start_time;
    int status;  // 0=running, 1=terminated, -1=error
} process_info_t;

extern process_info_t process_table[MAX_PROCESSES];
extern int process_count;

int create_process(const char *command, char *args[]);
int check_process_status(pid_t pid);
int terminate_process(pid_t pid, int force);

#endif
