#include "libmx.h"

bool mx_by_lex(void *a, void *b) {
    if (!a || !b) {
        return true;
    }
    return mx_strcmp(a, b) > 0;
}

bool mx_by_null(void *a, void *b) {
    return a != NULL && b == NULL;
}
