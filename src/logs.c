#include <time.h>
#include <stdio.h>
#include <string.h>

#include "../inc/logs.h"

void log_to_file(FILE *logfile, LogInfo loginfo) {
    time_t now = time(NULL);
    char *now_s = ctime(&now);
    now_s[strcspn(now_s, "\n")] = '\0';
    
    switch (loginfo.level) {
        case LOG_INFO:
            fprintf(logfile, "[%s]\n-----%s-----\n%s\n%s\n", now_s, "INFO", loginfo.message, loginfo.detail);
            break;

        case LOG_WARNING:
            fprintf(logfile, "[%s]\n-----%s-----\n%s\n%s\n", now_s, "WARNING", loginfo.message, loginfo.detail);
            break;

        case LOG_ERROR:
            fprintf(logfile, "[%s]\n-----%s-----\n%s\n%s\n", now_s, "ERROR", loginfo.message, loginfo.detail);
            break;

        case LOG_FATAL:
            fprintf(logfile, "[%s]\n-----%s-----\n%s\n%s\n", now_s, "FATAL", loginfo.message, loginfo.detail);
            break;
    }

    return;
}
