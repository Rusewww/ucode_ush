#include "../inc/ush.h"

static int parse(char **flags, int *idx, int *mode) {
    for (*idx = 0; flags[*idx] && flags[*idx][0] == '-' && strcmp(flags[*idx], "--") != 0;) {
        if (!mx_match(flags[*idx], "^-[ea]+$")) {
            fprintf(stderr, "unset: invalid option\n");
            fprintf(stderr, "usage: unset [-ea] var ...\n");
            return 1;
        }
        if (mx_match(flags[*idx], "a")) {
            *mode = 2;
        }
        *mode = *mode == 2 ? 2 : 1;
        (*idx)++;
    }
    if (flags[*idx] && !strcmp(flags[*idx], "--")) {
        (*idx)++;
    }
    return 0;
}

int mx_unset(char **args) {
    int res = 0;
    int idx;
    int mode = 0;
    res = parse(args, &idx, &mode);
    while (args[idx] && res != 1) {
        if (mx_unset_var(args[idx]) == -1 && !res) {
            fprintf(stderr, "unset: %s: invalid parameter name\n", args[idx]);
            res = 1;
        }
        idx++;
    }
    if (mode && res != 1) {
        if (mode == 2) {
            t_list **shell = mx_get_var_list(SHELL);
            t_list *curr = *shell;
            char *var = NULL;
            while (curr) {
                mx_get_name(curr->data, &var);
                mx_var_list_delete(EXP, var);
                curr = curr->next;
            }
            if (var) {
                mx_strdel(&var);
            }
        }
        mx_clear_env();
    }
    return res;
}

int mx_unset_var(char *var) {
    int res = 0;
    res = unsetenv(var);
    if (res != -1 && mx_match(var, MX_UNSET_ARG)) {
        mx_var_list_delete(SHELL, var);
        mx_var_list_delete(EXP, var);
    } else {
        res = -1;
    }
    return res;
}
