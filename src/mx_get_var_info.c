#include "ush.h"

char *mx_get_var_info(char *arg, bool type) {
    char *out = NULL;
    if (type == 0) {
        if (arg[0] == '=') {
            out = strndup(arg, mx_get_char_index(&arg[1], '='));
        } else {
            out = strndup(arg, mx_get_char_index(arg, '='));
        }
    } else {
        out = strdup(arg + mx_get_char_index(arg, '=') + 1);
    }
    return out;
}
