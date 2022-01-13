#include "../inc/ush.h"

void mx_home(t_map **map) {
    int res = chdir(getenv("HOME"));
    char *dir = getenv("HOME");
    if (res < 0) {
        if (dir == NULL) {
            fprintf(stderr, "cd: HOME not set\n");
        } else {
            fprintf(stderr, "cd: %s: ", dir);
            perror("");
        }
        return;
    }
    mx_change_map(map, dir);
}
