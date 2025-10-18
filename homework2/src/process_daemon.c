#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <syslog.h>

void daemonize(void) {
    pid_t pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);

    if (setsid() < 0) exit(EXIT_FAILURE);

    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    pid = fork();
    if (pid < 0) exit(EXIT_FAILURE);
    if (pid > 0) exit(EXIT_SUCCESS);

    umask(0);
    chdir("/");

    int fd;
    for (fd = sysconf(_SC_OPEN_MAX); fd > 0; fd--) {
        close(fd);
    }

    openlog("process_daemon", LOG_PID, LOG_DAEMON);
    syslog(LOG_NOTICE, "Daemon started");
}

void spawn_workers(void) {
    while (1) {
        pid_t pid = fork();
        if (pid == 0) {
            syslog(LOG_INFO, "Worker %d started", getpid());
            sleep(2);
            syslog(LOG_INFO, "Worker %d exiting", getpid());
            exit(0);
        }
        sleep(5);
    }
}

void sigterm_handler(int sig) {
    syslog(LOG_NOTICE, "Daemon exiting");
    closelog();
    exit(EXIT_SUCCESS);
}

int main() {
    daemonize();
    signal(SIGTERM, sigterm_handler);

    spawn_workers();

    return 0;
}
