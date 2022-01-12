#include "ush.h"

void mx_clearenv(void) {
    extern char **environ;
    char **del_var = NULL;
    char *del_n = NULL;

    if (environ[0] != NULL) {
        del_var = mx_env_copy();
        int i = 0;
        while (del_var[i]) {
            mx_get_name(del_var[i], &del_n);
            unsetenv(del_n);
            i++;
        }
        if (del_var[0]) {
            mx_del_strarr(&del_var);
        }
        if (del_n != NULL) {
            mx_strdel(&del_n);
        }
    }
}
