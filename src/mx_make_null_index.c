#include "ush.h"

char **mx_make_null_index(char **split, int index) {
    mx_strdel(&split[index]);
    split[index] = strdup("");
    int i = index - 1;
    while (split[i] && i > -1) {
        if (strcmp(split[i], "") != 0) {
            mx_strdel(&split[i]);
            split[i] = strdup("");
            break;
        }
        i--;
    }
    return split;
}
