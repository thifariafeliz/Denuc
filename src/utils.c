#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#include "../inc/utils.h"

IntParseResult parse_int(const char *str) {
    if (str == NULL) {
        return (IntParseResult){.value = 0, .status = PARSE_NULL_ARG};
    }

    errno = 0;
    char *end = NULL;
    long num = strtol(str, &end, 10);

    if (end == str) {
        return (IntParseResult){.value = 0, .status = PARSE_EMPTY};
    }

    while (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r') {
        end++;
    }

    if (*end != '\0') {
        return (IntParseResult){.value = 0, .status = PARSE_INVALID};
    }

    if (errno == ERANGE || num > INT_MAX || num < INT_MIN) {
        return (IntParseResult){.value = 0, .status = PARSE_OVERFLOW};
    }

    return (IntParseResult){.value = (int)num, .status = PARSE_OK};
}
