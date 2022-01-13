#include "libmx.h"

t_list *mx_create_node(void *data) {
    t_list *node = malloc(sizeof *node);
    node->data = data;
    node->next = NULL;
    return node;
}
