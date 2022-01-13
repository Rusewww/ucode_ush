#include "libmx.h"

void mx_print_strarr(char **arr, const char *delim) {
    if (delim == NULL) return;
    while (*arr != NULL) {
        mx_printstr(*arr);
        if (arr[1] == NULL) {
            mx_printchar('\n');
            break;
        }
        mx_printstr(delim);
        arr++;
    }
}
