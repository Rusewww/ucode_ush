#include "libmx.h"

int mx_count_words(const char *str, char c) {
    if (str == NULL) return -1;
    int len = mx_strlen(str);
    int count = 0;
    if (str[0] != c && str[0] != '\0') count++;
    for (int i = 1; i < len; i++) {
        if (str[i - 1] == c && str[i] != c) {
            count++;
        }
    }
    return count;
}
