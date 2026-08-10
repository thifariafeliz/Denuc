#include <stdlib.h>
#include <string.h>

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
