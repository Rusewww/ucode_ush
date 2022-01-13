#include "libmx.h"

void *mx_memset(void *b, int c, size_t len) {
    unsigned char *ptr = b;
    for (unsigned long i = 0; i < len; i++) {
        ptr[i] = c;
    }
    return b;
}
