#include "libmx.h"

int mx_get_substr_index(const char *str, const char *sub) {
    if (str == NULL || sub == NULL) return -2;
    char *ptr_in_str = mx_strstr(str, sub);
    return ptr_in_str == NULL ? -1 : ptr_in_str - str;
}
