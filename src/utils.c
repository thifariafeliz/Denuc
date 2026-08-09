#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#include "../inc/utils.h"

IntParseResult parse_int(const char *str) {
    errno = 0;
    char *end = NULL;
    double num = strtol(str, &end, 10);

    if (end == str) {
        return (IntParseResult){.value = 0, .status = PARSE_EMPTY};
    }

    if (*end == '\n' || *end == '\0') {
        return (IntParseResult){.value = 0, .status = PARSE_INVALID};
    }

    if (errno == ERANGE || errno > INT_MAX || errno < INT_MIN) {
        return (IntParseResult){.value = 0, .status = PARSE_OVERFLOW};
    }

    return (IntParseResult){.value = (int)num, .status = PARSE_OK};
}
