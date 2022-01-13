#include "libmx.h"

t_list *mx_sort_list(t_list *list, bool (*cmp)(void *, void *)) {
    if (list == NULL) return NULL;
    void *buf = NULL;
    t_list *curr = list;
    t_list *head = list;
    while (list != NULL) {
        curr = list->next;
        while (curr != NULL) {
            if (cmp(list->data, curr->data)) {
                buf = list->data;
                list->data = curr->data;
                curr->data = buf;
            }
            curr = curr->next;
        }
        list = list->next;
    }
    return head;
}
