#include "libmx.h"

char *mx_strchr(char *str, char c) {
    while (*str != '\0') {
        if (*str == c)
            return str;
        str++;
    }
    if (*str == c)
        return str;
    return NULL;
}
