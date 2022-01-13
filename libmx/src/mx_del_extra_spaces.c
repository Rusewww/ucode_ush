#include "libmx.h"

char *mx_del_extra_spaces(const char *str) {
    if (str == NULL) return NULL;
    int count = 0;
    char *trimmed = mx_strtrim(str);
    for (int i = 0; trimmed[i] != '\0'; i++) {
        if (!mx_isspace(trimmed[i])) {
            count++;
            if (mx_isspace(trimmed[i + 1])) count++;
        }
    }
    char *clean = mx_strnew(count);
    int j = 0;
    for (int i = 0; trimmed[i] != '\0'; i++) {
        if (!mx_isspace(trimmed[i])) {
            clean[j] = trimmed[i];
            j++;
            if (mx_isspace(trimmed[i + 1])) {
                clean[j] = ' ';
                j++;
            }
        }
    }
    mx_strdel(&trimmed);
    return clean;
}
