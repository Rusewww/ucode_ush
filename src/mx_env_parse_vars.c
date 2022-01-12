#include "ush.h"

void mx_env_parse_vars(char **argv, char **path, int *idx) {
    char *name = NULL;
    bool replace = false;

    while (argv[*idx]) {
        if (mx_match(argv[*idx], MX_ENV_VAR)) {
            mx_get_name(argv[*idx], &name);
            mx_putenv(argv[*idx]);
            if (!strcmp(name, "PATH") && (!(*path) || replace)) {
                replace = true;
                if (*path) {
                    mx_strdel(path);
                }
                *path = mx_get_var_info(argv[*idx], 1);
            }
        } else {
            break;
        }
        (*idx)++;
    }
    if (!(*path) && mx_get_var_val(SHELL, "PATH")) {
        *path = strdup(mx_get_var_val(SHELL, "PATH"));
    }
    mx_strdel(&name);
}
