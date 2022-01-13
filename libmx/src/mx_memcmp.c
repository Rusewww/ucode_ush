#include "libmx.h"

int mx_memcmp(const void *s1, const void *s2, size_t n) {
    const unsigned char *one = s1;
    const unsigned char *two = s2;
    while (*one - *two == 0 && (unsigned long) (one - (unsigned char *) s1) <= n) {
        one++;
        two++;
    }
    return *one - *two;
}
