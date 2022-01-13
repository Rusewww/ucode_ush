#include "../inc/ush.h"

static bool mx_is_built_in(char *com) {
    if (!strcmp(com, "pwd") || !strcmp(com, "cd") || !strcmp(com, "fg") || !strcmp(com, "env") ||
        !strcmp(com, "echo") || !strcmp(com, "exit") || !strcmp(com, "unset") ||
        !strcmp(com, "color") || !strcmp(com, "which") || !strcmp(com, "export")) {
        return 1;
    }
    return 0;
}

bool mx_built_in(char *file, int *mode) {
    if (mx_is_built_in(file)) {
        if (*mode != 2) {
            dprintf(0, "%s: shell built-in comand\n", file);
        }
        if (*mode != 1) {
            *mode = 2;
        }
        return 0;
    }
    return 1;
}
