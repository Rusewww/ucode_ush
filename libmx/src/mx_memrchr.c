#include "libmx.h"

void *mx_memrchr(const void *s, int c, size_t n) {
    const unsigned char *ptr = s;
    for (unsigned long i = n; i > 0; i--) {
        if (ptr[i - 1] == c) return (void *) (ptr + i - 1);
    }
    return NULL;
}
