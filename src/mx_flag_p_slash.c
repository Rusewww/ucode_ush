#include "ush.h"

void mx_flag_p_slash(char *nd, t_map **map) {
    if (nd == NULL) {
        mx_change_dir(nd, map, 1);
    } else if (nd[0] == '/') {
        char *str = realpath(nd, NULL);
        if (mx_check_dir_exists(nd) == false) {
            fprintf(stderr, "cd: %s: No such file or directory\n", nd);
            mx_strdel(&str);
        } else if (strcmp(nd, str) == 0) {
            mx_change_dir(nd, map, 1);
        } else {
            mx_change_dir(str, map, 1);
        }
        mx_strdel(&str);
    } else {
        mx_flag_p_full_path(map, nd);
    }
}
