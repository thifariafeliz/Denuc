#ifndef STRINGO_H
#define STRINGO_H

#include <stddef.h>
#include "errors.h"

typedef struct Stringo {
    char *data;
    size_t length;
    size_t capacity;
} Stringo;

Stringo *stringo_create(const char *cstr);
Stringo_Error stringo_take_input(Stringo *stringo);

#endif
