#include "ush.h"

int mx_unset_var(char *var) {
    int res = 0;
    res = unsetenv(var);
    if (res != -1 && mx_match(var, MX_UNSET_ARG)) {
        mx_var_list_delete(SHELL, var);
        mx_var_list_delete(EXP, var);
    } else {
        res = -1;
    }
    return res;
}
