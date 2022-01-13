#include "libmx.h"

static void *get_value(t_node *node, char *key);

void *mx_get_map(t_map **map, char *key) {
    int index = mx_hash(key) % (*map)->size;

    return get_value((*map)->nodes[index], key);
}

static void *get_value(t_node *node, char *key) {
    while (node) {
        if (!mx_strcmp(node->key, key)) {
            return node->data;
        }
        node = node->next;
    }
    return NULL;
}
