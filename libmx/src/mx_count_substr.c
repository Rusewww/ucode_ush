#include "libmx.h"

int mx_count_substr(const char *str, const char *sub) {
    if (str == NULL || sub == NULL) return -1;
    if (mx_strlen(sub) == 0) return 0;
    int count = -1;
    while (str != NULL) {
        str = mx_strstr(str, sub);
        str = str == NULL ? str : str + 1;
        count++;
    }
    return count;
}
