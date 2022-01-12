#include "ush.h"

void mx_check_s_slash(char *dir, t_map **map) {
    char *str = realpath(dir, NULL);
    char *str_new = mx_parse_path(NULL, dir, map);

    if (strcmp(str_new, str) == 0) {
        mx_change_dir(dir, map, 1);
        mx_strdel(&str);
    } else {
        fprintf(stderr, "cd: %s: is not a directory.\n", dir);
        mx_strdel(&str);
        str = mx_get_map(map, "PWD");
        chdir("/");
        chdir(str);
    }
}
