#ifndef LIST_H
#define LIST_H

#include <stddef.h>
#include "errors.h"

typedef struct Node {
    void *data;
    struct Node *next;
} Node;

typedef struct List {
    int size;
    void (*destroy)(void *data);

    Node *head;
    Node *tail;
} List;

List_Error list_init(List *list, void (*destroy)(void *data));
List_Error list_destroy(List *list);
List_Error list_ins_next(List *list, Node *node, const void *data);
List_Error list_rem_next(List *list, Node *node, void **data);

static inline int list_size(List *list) {
    if (list == NULL) {
        return -1;
    }

    return list->size;
}

static inline void *list_next(Node *node) {
    if (node == NULL) {
        return NULL;
    }

    return node->next;
}

static inline void *list_data(Node *node) {
    if (node == NULL) {
        return NULL;
    }

    return node->data;
}

static inline void* list_head(List *list) {
    if (list == NULL) {
        return NULL;
    }

    return list->head;
}

static inline void *list_tail(List *list) {
    if (list == NULL) {
        return NULL;
    }

    return list->tail;
}

#define list_is_head(list, node) ((node) == (list)->head ? 1 : 0)
#define list_is_tail(node) ((node)->next == NULL ? 1 : 0)

#endif
