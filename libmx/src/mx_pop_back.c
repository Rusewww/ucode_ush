#include "libmx.h"

void mx_pop_back(t_list **head) {
    if (head == NULL || *head == NULL) return;
    if ((*head)->next == NULL) {
        free(*head);
        *head = NULL;
        return;
    }
    t_list *curr = *head;
    while (curr->next->next != NULL) {
        curr = curr->next;
    }
    free(curr->next);
    curr->next = NULL;
}
