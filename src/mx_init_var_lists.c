#include "ush.h"

static void copy_environ(char **copy, char **environ) {
    int i = 0;
    while (environ[i]) {
        copy[i] = strdup(environ[i]);
        if (environ[i + 1] == NULL) {
            copy[i + 1] = NULL;
        }
        i++;
    }
}

static char **get_sorted_environ(void) {
    char **sort = NULL;
    extern char **environ;
    int len = 0;

    if (*environ == NULL) {
        mx_putenv("SHLVL=0");
    }
    for (; environ[len]; len++);

    sort = malloc((len + 1) * sizeof(char *));
    copy_environ(sort, environ);
    mx_bubble_sort(sort, len);
    return sort;
}

void mx_init_var_lists(void) {
    t_list **shell_list = mx_get_var_list(SHELL);
    t_list **exp_list = mx_get_var_list(EXP);
    char **sorted_environ = get_sorted_environ();
    int i = 0;

    while (sorted_environ[i]) {
        mx_push_back(shell_list, strdup(sorted_environ[i]));
        mx_push_back(exp_list, strdup(sorted_environ[i]));
        i++;
    }
    mx_del_strarr(&sorted_environ);
}
