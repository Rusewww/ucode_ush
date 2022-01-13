#include "../inc/ush.h"

static bool is_number_arg(char *arg) {
    unsigned int i = 0;
    while (i < strlen(arg)) {
        if (!isnumber(arg[i])) {
            return false;
        }
        i++;
    }
    return true;
}

int mx_exit(char **args) {
    int exitcode = 0;

    if (mx_arr_size(args) > 1 && is_number_arg(args[0])) {
        fprintf(stderr, "exit: too many arguments\n");
        return 1;
    }

    mx_deinit();

    if (!*args) {
        exit(exitcode);
    }

    unsigned int i = 0;

    while (i < strlen(args[0])) {
        if (!isnumber(args[0][i])) {
            fprintf(stderr, "exit: bad argument\n");
            exit(1);
        }
        i++;
    }

    exitcode = atoi(args[0]);
    exit(exitcode);
}
