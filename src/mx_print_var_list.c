#include "../inc/ush.h"

void mx_print_var_list(t_var_list key, int fd) {
    t_list **var = mx_get_var_list(key);
    t_list *cur = *var;
    while (cur) {
        dprintf(fd, "%s\n", cur->data);
        if (cur->next) {
            cur = cur->next;
        } else {
            cur = NULL;
        }
    }
}
