#ifndef ERRORS_H
#define ERRORS_H

typedef enum List_Error {
    LIST_OK = 0,
    LIST_IS_EMPTY,
    LIST_ARG_IS_NULL,
    LIST_FAILED_ALLOC,
    LIST_UNDEFINED_ERROR,
    LIST_NEXT_SNB_NULL
} List_Error;

typedef enum File_Error {
    FILE_OK,
    FILE_ARG_IS_NULL,
    FILE_PARSE_ERROR,
    FILE_FAILED_ALLOC,
    FILE_FAILED_ENQUEUE,
} File_Error;

typedef enum Stringo_Error {
    STRINGO_OK = 0,
    STRINGO_EARLY_EOF,
    STRINGO_ARG_IS_NULL,
    STRINGO_FAILED_ALLOC,
} Stringo_Error;

#endif
