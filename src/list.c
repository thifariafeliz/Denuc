#include <stdlib.h>
#include <string.h>

#include "../inc/list.h"
#include "../inc/errors.h"

List_Error list_init(List *list, void (*destroy)(void *data)) {
    if (list == NULL) {
        return LIST_ARG_IS_NULL;
    }

    list->size = 0;
    list->head = NULL;
    list->tail = NULL;
    list->destroy = destroy;

    return LIST_OK;
}

List_Error list_destroy(List* list) {
    if (list == NULL) {
        return LIST_ARG_IS_NULL;
    }

    void *data = NULL;

    while (list_size(list) > 0) {
        if (list->destroy != NULL) {
            switch (list_rem_next(list, NULL, (void**)&data)) {
                case LIST_OK:
                    list->destroy(data);

                case LIST_ARG_IS_NULL:
                    return LIST_ARG_IS_NULL;

                default:
                    return LIST_UNDEFINED_ERROR;
            }
        }
    }

    memset(list, 0, sizeof(List));

    return LIST_OK;
}

List_Error list_ins_next(List *list, Node *node, const void *data) {
    if (list == NULL) {
        return LIST_ARG_IS_NULL;
    }

    Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL) {
        return LIST_FAILED_ALLOC;
    }

    new_node->data = (void*)data;

    if (node == NULL) {
        if (list_size(list) == 0) {
            list->tail = new_node;
        }

        new_node->next = list->head;
        list->head = new_node;
    }
    else {
        if (node->next == NULL) {
            list->tail = new_node;
        }

        new_node->next = node->next;
        node->next = new_node;
    }

    list->size++;

    return LIST_OK;
}

List_Error list_rem_next(List *list, Node *node, void **data) {
    if (list == NULL) {
        return LIST_ARG_IS_NULL;
    }

    Node *old_node = NULL;

    if (node == NULL) {
        *data = list->head->data;
        old_node = list->head;
        list->head = list->head->next;

        if (list_size(list) == 1) {
            list->tail = NULL;
        }
    }
    else {
        if (node->next == NULL) {
            return LIST_NEXT_SNB_NULL;
        }

        *data = node->next->data;
        old_node = node->next;
        node->next = node->next->next;

        if (node->next == NULL) {
            list->tail = node;
        }
    }

    free(old_node);

    list->size--;

    return LIST_OK;
}
