#include "libmx.h"

void mx_del_list(t_list **list) {
    while (*list != NULL) {
        mx_pop_front(list);
    }
}
