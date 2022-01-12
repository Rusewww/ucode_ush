#include "ush.h"

void mx_print_env(int fd) {
    extern char **environ;
    int i = 0;
    while (environ[i]) {
        dprintf(fd, "%s\n", environ[i]);
        i++;
    }
}
