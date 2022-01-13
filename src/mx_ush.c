#include "../inc/ush.h"

bool check_stdin(int *exit_code) {
    char *buff = NULL;
    size_t linecap = 0;
    ssize_t linelen = 0;

    if (isatty(STDIN_FILENO))
        return false;
    buff = mx_strnew(ARG_MAX + 1);
    while ((linelen = getline(&buff, &linecap, stdin)) > 0) {
        buff[linelen] = '\0';
        if (buff[linelen - 1] == '\n') {
            buff[linelen - 1] = '\0';
        }
        mx_handle_cmnd(buff, exit_code);
    }
    mx_strdel(&buff);
    return true;
}

