#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>

#include "list.h"

typedef List Queue;

#define queue_init list_init
#define queue_size list_size
#define queue_destroy list_destroy

List_Error queue_enqueue(Queue *queue, const void *data);
List_Error queue_dequeue(Queue *queue, void **data);

#define queue_peek(queue) ((queue)->head == NULL ? NULL : (queue)->head->data)

#endif
