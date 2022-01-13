#include "libmx.h"

void mx_push_back(t_list **list, void *data) {
    t_list *tail = mx_create_node(data);
    if (*list == NULL) {
        *list = tail;
        return;
    }
    t_list *curr = *list;
    while (curr->next != NULL) {
        curr = curr->next;
    }
    curr->next = tail;
}
