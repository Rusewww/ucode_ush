#include "libmx.h"

void *mx_memchr(const void *s, int c, size_t n) {
    const unsigned char *ptr = s;
    for (unsigned long i = 0; i < n; i++) {
        if (ptr[i] == c) return (void *) (ptr + i);
    }
    return NULL;
}
