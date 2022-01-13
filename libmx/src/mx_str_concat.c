#include "libmx.h"

char *mx_str_concat(char **s1, char *s2) {
    char *res = mx_strjoin(*s1, s2);
    mx_strdel(s1);
    *s1 = res;
    return res;
}
