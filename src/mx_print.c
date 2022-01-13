#include "../inc/ush.h"

void mx_print_env(int fd) {
    extern char **environ;
    int i = 0;
    while (environ[i]) {
        dprintf(fd, "%s\n", environ[i]);
        i++;
    }
}

int mx_print_env_error(char option, char *err_arg, int error) {
    if (error == 2) {
        fprintf(stderr, "env: unsetenv %s: Invalid argument\n", err_arg);
    } else {
        if (error == 0) {
            fprintf(stderr, "env: illegal option -- %c\n", option);
        }
        if (error == 1) {
            fprintf(stderr, "env: option requires an argument -- %c\n", option);
        }
        fprintf(stderr, "usage: env [-i] [-P utilpath] [-u name]\n");
        fprintf(stderr, "           [name=value ...] [utility [argument ...]]\n");
    }
    return 1;
}

void mx_print_var_list(t_var_list key, int fd) {
    t_list **var = mx_get_var_list(key);
    t_list *cur = *var;
    while (cur) {
        dprintf(fd, "%s\n", cur->data);
        if (cur->next) {
            cur = cur->next;
        } else {
            cur = NULL;
        }
    }
}
