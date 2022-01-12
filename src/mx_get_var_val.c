#include "ush.h"

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
