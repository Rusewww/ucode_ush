#include "libmx.h"

void *mx_memmem(const void *big, size_t big_len, const void *little, size_t little_len) {
    const unsigned char *haystack = big;
    const unsigned char *needle = little;
    for (unsigned long i = 0; i < big_len; i++) {
        for (unsigned long j = 0; j < little_len; j++) {
            if (haystack[i + j] != needle[j]) break;
            if (j == little_len - 1) {
                return (void *) (haystack + i);
            }
        }
    }
    return NULL;
}
