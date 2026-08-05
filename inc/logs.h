#ifndef LOGS_H
#define LOGS_H

#include <stdio.h>

typedef enum LogLevel {
    LOG_INFO = 0,
    LOG_WARNING,
    LOG_ERROR,
    LOG_FATAL,
} LogLevel;

typedef struct LogInfo {
    char *message;
    char *detail;
    LogLevel level;
} LogInfo;

void log_to_file(FILE *logfile, LogInfo loginfo);

#endif
