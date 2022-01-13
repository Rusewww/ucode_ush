#include "../inc/libmx.h"

bool mx_check_exists(char *dir) {
    if (chdir(dir) < 0) {
        return false;
    } else {
        chdir("-");
    }
    return true;
}

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

bool mx_check_colons(char **cmnd, int *code) {
    unsigned int index = 0;
    char *str = NULL;
    while (cmnd[index]) {
        str = mx_strtrim(cmnd[index]);
        if (!strlen(str)) {
            *code = 1;
            fprintf(stderr, "%s: parse error near \'", MX_SHELL_NAME);
            if (!index) {
                fprintf(stderr, ";\'");
            } else {
                fprintf(stderr, ";;\'");
            }
            fprintf(stderr, "\n");
            mx_strdel(&str);
            return false;
        }
        mx_strdel(&str);
        index++;
    }
    return true;
}
