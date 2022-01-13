#include "../inc/ush.h"

void mx_put_env(char *var) {
    static char name[1024];
    static char val[1024];
    char *received_n = mx_get_var_info(var, 0);
    char *received_v = mx_get_var_info(var, 1);
    sprintf(name, "%s", received_n);
    sprintf(val, "%s", received_v);
    setenv(name, val, 1);
    mx_strdel(&received_n);
    mx_strdel(&received_v);
}
