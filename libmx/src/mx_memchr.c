#include "libmx.h"

void *mx_memchr(const void *s, int c, size_t n) {
    const unsigned char *s_c = s;
    
    while (n--) {
        if (*s_c == c) {
            return (void*)s_c;
        }
        s_c++;
    }
    return NULL;
}
