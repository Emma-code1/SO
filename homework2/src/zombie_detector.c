#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>

int find_zombies(int *zombie_pids, int max_zombies) {
    DIR *proc_dir = opendir("/proc");
    if (!proc_dir) {
        perror("Failed to open /proc");
        return -1;
    }

    struct dirent *entry;
    int zombie_count = 0;

    while ((entry = readdir(proc_dir)) != NULL && zombie_count < max_zombies) {
        if (entry->d_type == DT_DIR && isdigit(entry->d_name[0])) {
            char stat_path[256];
            snprintf(stat_path, sizeof(stat_path), "/proc/%s/stat", entry->d_name);

            FILE *stat_file = fopen(stat_path, "r");
            if (stat_file) {
                char stat_line[1024];
                if (fgets(stat_line, sizeof(stat_line), stat_file)) {
                    char state;
                    int pid, ppid;
                    char comm[256];

                    sscanf(stat_line, "%d %s %c %d", &pid, comm, &state, &ppid);

                    if (state == 'Z') {
                        zombie_pids[zombie_count++] = pid;
                    }
                }
                fclose(stat_file);
            }
        }
    }
    closedir(proc_dir);
    return zombie_count;
}

void print_zombie_info(int pid) {
    char stat_path[256], cmdline_path[256];
    snprintf(stat_path, sizeof(stat_path), "/proc/%d/stat", pid);
    snprintf(cmdline_path, sizeof(cmdline_path), "/proc/%d/cmdline", pid);

    FILE *stat_file = fopen(stat_path, "r");
    FILE *cmdline_file = fopen(cmdline_path, "r");

    if (stat_file && cmdline_file) {
        char stat_line[1024], cmdline[1024] = {0};
        int ppid;
        char comm[256], state;

        fgets(stat_line, sizeof(stat_line), stat_file);
        sscanf(stat_line, "%*d %s %c %d", comm, &state, &ppid);

        fread(cmdline, sizeof(cmdline), 1, cmdline_file);
        if (strlen(cmdline) == 0) strcpy(cmdline, "[defunct]");

        printf("%-6d %-6d %-15s %c\n", pid, ppid, cmdline, state);

        fclose(stat_file);
        fclose(cmdline_file);
    }
}

int main() {
    int zombie_pids[100];
    int count = find_zombies(zombie_pids, 100);

    printf("=== Zombie Process Report ===\n");
    printf("Total Zombies: %d\n", count);
    printf("PID     PPID    Command         State\n");
    printf("-----   -----   -------------   -----\n");

    for (int i = 0; i < count; i++) {
        print_zombie_info(zombie_pids[i]);
    }

    return 0;
}
