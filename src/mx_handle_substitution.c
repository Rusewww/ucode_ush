#include "../inc/ush.h"

t_list *mx_handle_substitution(t_list *arg) {
    t_list *res = NULL;

    for (; arg; arg = arg->next) {
        mx_push_back(&res, strdup(arg->data));
    }
    return res;
}
