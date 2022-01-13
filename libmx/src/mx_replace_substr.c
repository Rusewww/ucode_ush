#include "libmx.h"

char *mx_replace_substr(const char *str, const char *sub, const char *replace) {
    if (str == NULL || sub == NULL || replace == NULL) return NULL;
    char *res = NULL;
    char *prev = res;
    char *cur = NULL;
    int sub_len = mx_strlen(sub);
    int sub_idx = 0;
    while (str != NULL) {
        sub_idx = mx_get_substr_index(str, sub);
        if (sub_idx == -1) {
            sub_idx = mx_strlen(str);
        }
        cur = mx_strndup(str, sub_idx);
        str = mx_strstr(str, sub);
        res = mx_strjoin(res, cur);
        mx_strdel(&cur);
        mx_strdel(&prev);
        if (str == NULL) break;
        prev = res;
        res = mx_strjoin(res, replace);
        mx_strdel(&prev);
        prev = res;
        if (*(str + sub_len) == '\0') break;
        if (str != NULL) str += sub_len;
    }
    return res;
}
