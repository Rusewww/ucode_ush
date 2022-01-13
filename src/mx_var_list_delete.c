#include "../inc/ush.h"

static void pop_curr_node(t_list **list, t_list **prev, t_list **current) {
    if (*prev == NULL) {
        *prev = (*current)->next;
        *list = *prev;
    } else {
        (*prev)->next = (*current)->next;
    }
    free((*current)->data);
    free(*current);
    *current = NULL;
}

void mx_var_list_delete(t_var_list key, char *del_n) {
    t_list **list = mx_get_var_list(key);
    t_list *curr = *list;
    t_list *prev = NULL;
    char *curr_name = NULL;

    while (curr) {
        mx_get_name(curr->data, &curr_name);
        if (strcmp(del_n, curr_name) == 0) {
            pop_curr_node(list, &prev, &curr);
            break;
        }
        prev = curr;
        curr = curr->next;
        if (curr) {
            mx_get_name(curr->data, &curr_name);
        }
    }
    mx_strdel(&curr_name);
}
