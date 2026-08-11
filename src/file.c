#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/logs.h"
#include "../inc/errors.h"
#include "../inc/queue.h"
#include "../inc/list.h"
#include "../inc/utils.h"
#include "../inc/denuncia.h"
#include "../inc/file.h"

#define BUFFER_SIZE 1024

// reads the file and append the denounces to the queue
File_Error append_to_queue(FILE *file, Queue *queue, LogInfo *loginfo) {
    if (file == NULL) {
        return FILE_ARG_IS_NULL;
    }

    char buffer[BUFFER_SIZE];
    const char *delimiter = "|";

    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
        buffer[strcspn(buffer, "\r\n")] = '\0';

        if (buffer[0] == '\0') {
            continue;
        }

        char *id_str = strtok(buffer, delimiter);
        if (id_str == NULL) {
            continue;
        }
        while (*id_str == ' ' || *id_str == '\t') id_str++;
        char *id_end = id_str + strlen(id_str) - 1;
        while (id_end > id_str && (*id_end == ' ' || *id_end == '\t')) *id_end-- = '\0';

        char *title_str = strtok(NULL, delimiter);
        if (title_str == NULL) {
            continue;
        }
        while (*title_str == ' ' || *title_str == '\t') title_str++;
        char *title_end = title_str + strlen(title_str) - 1;
        while (title_end > title_str && (*title_end == ' ' || *title_end == '\t')) *title_end-- = '\0';

        char *status_str = strtok(NULL, delimiter);
        if (status_str == NULL) {
            continue;
        }
        while (*status_str == ' ' || *status_str == '\t') status_str++;
        char *status_end = status_str + strlen(status_str) - 1;
        while (status_end > status_str && (*status_end == ' ' || *status_end == '\t')) *status_end-- = '\0';


        IntParseResult id_result = parse_int(id_str);
        switch (id_result.status) {
            case PARSE_OK:
                break;
            case PARSE_NULL_ARG:
                *loginfo = (LogInfo){.message = "Argument is null.", .detail = "`parse_int(const char *str)` needs a not null argument.", .level = LOG_FATAL};
                return FILE_PARSE_ERROR;
            case PARSE_EMPTY:
                *loginfo = (LogInfo){.message = "ID is empty", .detail = "ID cannot be empty.", .level = LOG_FATAL};
                return FILE_PARSE_ERROR;
            case PARSE_INVALID:
                *loginfo = (LogInfo){.message = "ID is invalid", .detail = "`append_to_queue` needs a valid number for ID.", .level = LOG_FATAL};
                return FILE_PARSE_ERROR;
            case PARSE_OVERFLOW:
                *loginfo = (LogInfo){.message = "Overflow in ID.", .detail = "ID has a number greater or lower than the possible value for integers.", .level = LOG_FATAL};
                return FILE_PARSE_ERROR;
        }

        IntParseResult status_result = parse_int(status_str);
        switch (status_result.status) {
            case PARSE_OK:
                break;
            case PARSE_NULL_ARG:
                *loginfo = (LogInfo){.message = "Argument is null.", .detail = "`parse_int(const char *str)` needs a not null argument.", .level = LOG_FATAL};
                return FILE_PARSE_ERROR;
            case PARSE_EMPTY:
                *loginfo = (LogInfo){.message = "ID is empty", .detail = "ID cannot be empty.", .level = LOG_FATAL};
                return FILE_PARSE_ERROR;
            case PARSE_INVALID:
                *loginfo = (LogInfo){.message = "ID is invalid", .detail = "`append_to_queue` needs a valid number for ID.", .level = LOG_FATAL};
                return FILE_PARSE_ERROR;
            case PARSE_OVERFLOW:
                *loginfo = (LogInfo){.message = "Overflow in ID.", .detail = "ID has a number greater or lower than the possible value for integers.", .level = LOG_FATAL};
                return FILE_PARSE_ERROR;
        }

        Denuncia *nova_denuncia = malloc(sizeof(Denuncia));
        if (nova_denuncia == NULL) {
            *loginfo = (LogInfo){.level = LOG_FATAL, .message = "`malloc` failed.", .detail = "Failed to alloc new denounce."};
            return FILE_FAILED_ALLOC;
        }

        nova_denuncia->id = id_result.value;
        nova_denuncia->status = status_result.value;
        nova_denuncia->title = stringo_create(title_str);
        if (nova_denuncia->title == NULL) {
            *loginfo = (LogInfo){.level = LOG_FATAL, .message = "Failed to stringify report.", .detail = "`nova_denuncia->title = stringo_create(title_str)` failed."};
            return FILE_FAILED_ALLOC;
        }
        
        switch (queue_enqueue(queue, (void*)nova_denuncia)) {
            case LIST_OK:
                break;
            case LIST_ARG_IS_NULL:
                *loginfo = (LogInfo){.level = LOG_FATAL, .message = "Failed to enqueue new report.", .detail = "Queue passed to `queue_enqueue` while `append_to_queue` is null."};
                return FILE_FAILED_ENQUEUE;
            case LIST_FAILED_ALLOC:
                *loginfo = (LogInfo){.level = LOG_FATAL, .message = "Failed to enqueue new report.", .detail = "Failed to alloc space for new report in the queue."};
                return FILE_FAILED_ENQUEUE;
        }
    }

    loginfo = NULL;
    return FILE_OK;
}

// Writes the queue content to the file
File_Error write_to_file(FILE *file, Queue *queue, LogInfo *loginfo) {
    if (file == NULL || queue == NULL) {
        *loginfo = (LogInfo){.level = LOG_FATAL, .message = "One or more arguments are null.", .detail = "`write_to_file` cannot have null arguments for `FILE *file` and `Queue *queue` parameters."};
        return FILE_ARG_IS_NULL;
    }
    
    Node *node = queue->head;

    for (int i = 0; i < queue_size(queue); i++) {
        Denuncia *denuncia = (Denuncia*)node->data;

        fprintf(file, "%d | %s | %d\n", denuncia->id, denuncia->title->data, denuncia->status);
        node = node->next;
    }

    return FILE_OK;
}
