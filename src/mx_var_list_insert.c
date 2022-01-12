#include "ush.h"

static void get_names(char *var_one, char **name_one, char *var_two, char **name_two) {
    mx_get_name(var_one, name_one);
    mx_get_name(var_two, name_two);
    if (!strcmp(*name_two, "OLDPWD")) {
        t_map **map = mx_get_lenv();
        mx_put_map(map, "OLDPWD", mx_get_var_info(var_two, 1));
    }
}

static void change_var_value(t_list **list, t_list *node, char **name_one, char **name_two) {
    mx_var_list_replace_var(list, node);
    mx_delete_names(name_one, name_two, node);
}

void mx_var_list_insert(t_var_list key, char *arg) {
    t_list **list = mx_get_var_list(key);
    t_list *node = mx_create_node(strdup(arg));
    char *a_name = NULL;
    char *v_name = NULL;
    if (*list) {
        get_names((*list)->data, &v_name, arg, &a_name);
        if (strcmp(v_name, a_name) > 0) {
            mx_var_list_push_front(&node, &list, &a_name, &v_name);
            return;
        } else if (!strcmp(v_name, a_name)) {
            change_var_value(list, node, &v_name, &a_name);
            return;
        }
        mx_var_list_push_mid(list, &node, a_name, &v_name);
        mx_delete_names(&v_name, &a_name, NULL);
    } else {
        *list = node;
    }
}
