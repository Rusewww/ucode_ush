#include "libmx.h"

char *mx_strcat(char *restrict s1, const char *restrict s2) {
    int s1_len = mx_strlen(s1);
    int s2_len = mx_strlen(s2);
    for (int i = 0; i <= s2_len; i++) {
        s1[s1_len + i] = s2[i];
    }
    return s1;
}
