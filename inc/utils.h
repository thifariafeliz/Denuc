#ifndef UTILS_H
#define UTILS_H

typedef enum ParseResult {
    PARSE_OK = 0,
    PARSE_EMPTY,
    PARSE_INVALID,
    PARSE_OVERFLOW,
    PARSE_NULL_ARG,
} ParseResult;

typedef struct IntParseResult {
    int value;
    ParseResult status;
} IntParseResult;

IntParseResult parse_int(const char *str);

#endif
