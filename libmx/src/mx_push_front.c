#include "libmx.h"

void mx_push_front(t_list **list, void *data) {
    t_list *head = mx_create_node(data);
    head->next = *list;
    *list = head;
}
