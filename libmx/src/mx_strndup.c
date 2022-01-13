#include "libmx.h"

char *mx_strndup(const char *s1, size_t n) {
    unsigned int s_len = mx_strlen(s1);
    if (s_len < n) n = s_len;
    char *dup = mx_strnew(n);
    return mx_strncpy(dup, s1, n);
}
