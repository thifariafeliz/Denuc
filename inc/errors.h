#ifndef ERRORS_H
#define ERRORS_H

typedef enum List_Error {
    LIST_OK = 0,
    LIST_FAILED_ALLOC,
    LIST_ARG_IS_NULL,
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

#endif
