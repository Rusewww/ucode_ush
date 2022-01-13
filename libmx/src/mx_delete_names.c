#include "../inc/libmx.h"

void mx_delete_names(char **n_one, char **n_two, t_list *node) {
    if (n_one) {
        mx_strdel(n_one);
    }
    if (n_two) {
        mx_strdel(n_two);
    }
    free(node);
}
