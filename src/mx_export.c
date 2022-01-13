#include "../inc/ush.h"

static bool parse_error(char *arg) {
    char *a_name = mx_get_var_info(arg, 0);

    if (mx_match(arg, "^[-+]")) {
        fprintf(stderr, "export: does not accept any options: %c%c\n", arg[0], arg[1]);
    } else if (mx_match(arg, "^[0-9]")) {
        fprintf(stderr, "export: not an identifier: %s\n", a_name);
    } else {
        fprintf(stderr, "export: not valid in this context: %s\n", a_name);
    }
    mx_strdel(&a_name);
    return 1;
}

static void add_to_lists(char *arg) {
    mx_var_list_insert(SHELL, arg);
    mx_var_list_insert(EXP, arg);
    mx_strdel(&arg);
}

static void export_to_lists(char *arg) {
    t_list **s_list = mx_get_var_list(SHELL);
    t_list *cur = *s_list;
    char *a_name = NULL;
    char *v_name = NULL;

    mx_get_name(arg, &a_name);
    while (cur) {
        mx_get_name(cur->data, &v_name);
        if (strcmp(a_name, v_name) == 0) {
            mx_var_list_insert(EXP, cur->data);
            mx_putenv(cur->data);
            break;
        }
        cur = cur->next;
        if (cur) {
            mx_get_name(cur->data, &v_name);
        }
    }
    if (!cur) {
        add_to_lists(mx_strjoin(arg, "="));
    }
    mx_delete_names(&v_name, &a_name, NULL);
}


int mx_export(char **args, int fd) {
    bool stop = 0;
    if (args[0] == NULL) {
        mx_print_var_list(EXP, fd);
    } else {
        int i = 0;
        while (args[i] && !stop) {
            if (mx_match(args[i], MX_EXPORT_ARG)) {
                if (!mx_match(args[i], "=")) {
                    export_to_lists(args[i]);
                } else {
                    mx_putenv(args[i]);
                    add_to_lists(strdup(args[i]));
                }
            } else {
                stop = parse_error(args[i]);
            }
            i++;
        }
    }
    return 0;
}
