#include <stdio.h>
#include <time.h>
#include <stdarg.h>

void log_info(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm);
    printf("[%s] ", buf);
    vprintf(fmt, ap);
    printf("\n");
    va_end(ap);
}
