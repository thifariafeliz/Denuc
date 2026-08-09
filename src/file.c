#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/logs.h"
#include "../inc/errors.h"
#include "../inc/queue.h"
#include "../inc/list.h"
#include "../inc/utils.h"

#define BUFFER_SIZE 1024

// reads the file and append the denounces to the queue
File_Error append_to_queue(FILE *file, Queue *queue, LogInfo *loginfo) {
    if (file == NULL) {
        return FILE_ARG_IS_NULL;
    }

    char buffer[BUFFER_SIZE];
    const char *delimiter = " | ";

    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
        buffer[strcspn(buffer, "\r\n")] = '\0';

        if (buffer[0] == '\0') {
            continue;
        }

        char *id_str = strtok(buffer, delimiter);
        if (id_str == NULL) {
            continue;
        }

        char *title_str = strtok(NULL, delimiter);
        if (title_str == NULL) {
            continue;
        }

        char *status_str = strtok(NULL, delimiter);
        if (status_str == NULL) {
            continue;
        }


        IntParseResult id_result = parse_int(id_str);
        switch (id_result.status) {
            case PARSE_OK:
                break;


            case PARSE_NULL_ARG:

            case PARSE_EMPTY:
            case PARSE_INVALID:

            case PARSE_OVERFLOW:
        }
    }


    return FILE_OK;
}
