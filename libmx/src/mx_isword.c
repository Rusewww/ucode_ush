#include "libmx.h"

bool mx_isword(const char *s) {
    while (*s != '\0') {
        if (!mx_isalpha(*s)) {
            return false;
        }
        s++;
    }
    return true;
}
