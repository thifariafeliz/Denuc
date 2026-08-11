#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../inc/errors.h"
#include "../inc/stringo.h"

Stringo *stringo_create(const char *cstr) {
    Stringo *stringo = malloc(sizeof(Stringo));
    if (stringo == NULL) {
        stringo->data = NULL;
        stringo->length = 0;
        stringo->capacity = 0;

        return stringo;
    }

    stringo->length = strlen(cstr);
    stringo->capacity = stringo->length + 1;

    stringo->data = malloc(stringo->capacity);
    if (stringo->data == NULL) {
        stringo->length = 0;
        stringo->capacity = 0;
        return stringo;
    }

    strcpy(stringo->data, cstr);

    return stringo;
}

Stringo_Error stringo_take_input(Stringo *stringo) {
    if (stringo == NULL || stringo->data == NULL) {
        return STRINGO_ARG_IS_NULL;
    }

    int c;

    while (1) {
        c = getchar();

        if (c == EOF) {
            if (stringo->length == 0) {
                return STRINGO_EARLY_EOF;
            }
            break;
        }

        if (c == '\n') {
            break;
        }

        if (stringo->length + 1 >= stringo->capacity) {
            stringo->capacity *= 2;
            char *temp = realloc(stringo->data, stringo->capacity);
            if (temp == NULL) {
                return STRINGO_FAILED_ALLOC;
            }

            stringo->data = temp;
        }

        stringo->data[stringo->length++] = (char) c;
    }

    stringo->data[stringo->length] = '\0';

    stringo->capacity = stringo->length + 1;
    char *temp = realloc(stringo->data, stringo->capacity);
    if (temp == NULL) {
        return STRINGO_FAILED_ALLOC;
    }

    stringo->data = temp;

    return STRINGO_OK;
}
