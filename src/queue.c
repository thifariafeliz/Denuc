#include <stdlib.h>

#include "../inc/list.h"
#include "../inc/queue.h"

List_Error queue_enqueue(Queue *queue, const void *data) {
    if (queue == NULL) {
        return LIST_ARG_IS_NULL;
    }

    return list_ins_next(queue, queue->tail, data);
}

List_Error queue_dequeue(Queue *queue, void **data) {
    if (queue == NULL) {
        return LIST_ARG_IS_NULL;
    }

    return list_rem_next(queue, NULL, data);
}
