#include "libmx.h"

void *mx_memccpy(void *restrict dst, const void *restrict src, int c, size_t n) {
    unsigned char *d = dst;
    const unsigned char *s = src;
    for (unsigned long i = 0; i < n; i++) {
        *d = s[i];
        if (*d == c) break;
        d++;
    }
    if (*d != c) return NULL;
    else return ++d;
}
