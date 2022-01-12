#include "ush.h"

int mx_color(char **args) {
    int color = 0;

    if (!args[0]) {
        fprintf(stderr, "usage: color [1234567]\n");
        return 1;
    }
    if (mx_arr_size(args) > 1) {
        fprintf(stderr, "color: too many arguments\n");
        return 1;
    }
    if (strlen(args[0]) > 1 || !isnumber(args[0][0])) {
        fprintf(stderr, "color: invalid argument: %s\n", args[0]);
        return 1;
    }

    color = args[0][0] - '0';
    if (color > 7 || color < 0) {
        fprintf(stderr, "color: no such color [%d]\n", color);
        return 1;
    }

    mx_print_color(color);
    return 0;
}

void mx_print_color(int color) {
    static int s_color = -1;

    if (color != -1) {
        printf("\x1b[0;3%dm", color);
        s_color = color;
    } else if (s_color != -1) {
        printf("\x1b[0;3%dm", s_color);
    } else {
        printf("\x1b[0m");
    }
}

