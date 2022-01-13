#include "../inc/ush.h"

t_list **mx_get_list_procs(void) {
    static t_list *list = NULL;

    return &list;
}

t_map **mx_get_lenv(void) {
    static t_map *len_v = NULL;
    return &len_v;
}

struct termios *mx_get_tty(void) {
    static struct termios tty;
    return &tty;
}

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

char *mx_get_var_val(t_var_list key, char *var) {
    t_list **list = mx_get_var_list(key);
    t_list *cur = *list;
    char *find = mx_strjoin(var, "=");
    int length = strlen(find);

    while (cur) {
        if (strncmp(cur->data, find, length) != 0) {
            cur = cur->next;
        } else {
            break;
        }
    }
    mx_strdel(&find);
    if (cur == NULL) {
        return NULL;
    }
    return (char *) (cur->data) + length;
}
