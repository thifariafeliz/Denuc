#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/logs.h"
#include "../inc/errors.h"
#include "../inc/queue.h"
#include "../inc/list.h"

// reads the file and append the denounces to the queue
File_Error append_to_queue(FILE *file, Queue *queue) {
    if (file == NULL) {
        return FILE_ARG_IS_NULL;
    }



    return FILE_OK;
}
