#include "libmx.h"

char *mx_strstr(const char *haystack, const char *needle) {
    int hay_len = mx_strlen(haystack);
    int needle_len = mx_strlen(needle);
    if (needle_len == 0) return (char *) haystack;
    for (int i = 0; i < hay_len; i++) {
        for (int j = 0; j < needle_len; j++) {
            if (haystack[i + j] != needle[j]) break;
            if (j == needle_len - 1) {
                return (char *) &haystack[i];
            }
        }
    }
    return NULL;
}
