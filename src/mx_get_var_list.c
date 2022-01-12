#include "ush.h"

t_list **mx_get_var_list(t_var_list key) {
    static t_list *exp_l = NULL;
    static t_list *shell_l = NULL;
    if (key == EXP) {
        return &exp_l;
    } else if (key == SHELL) {
        return &shell_l;
    }
    return NULL;
}
