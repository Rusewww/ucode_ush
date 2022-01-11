#include "ush.h"

int mx_cd(char **split, int fd) {
    t_map **map = mx_get_lenv();

    int check_on_flags = 1;

    if (!strcmp(split[0], "-P") || !strcmp(split[0], "-sP") || !strcmp(split[0], "-Ps")) {
        mx_cd_flags(split[0], map, split[1]);
    } else if (!strcmp(split[0], "-s")) {
        mx_cd_flags("-s", map, split[1]);
    } else if (!strcmp(split[0], "-") && split[1] == NULL) {
        mx_change_dir("~OLDPWD", map, fd);
    } else if ((!strcmp(split[0], "-") && split[1] != NULL)
               || (split[0][0] == '-' && split[1] != NULL)) {
        fprintf(stderr, "cd: string not in pwd: %s\n", split[0]);
    } else if (!strcmp(split[0], "/")) {
        mx_change_dir(split[0], map, 1);
    } else {
        check_on_flags = 0;
    }

    if (!mx_arr_size(split))
        mx_change_dir(NULL, map, fd);
    else if (!check_on_flags)
        mx_change_dir(split[0], map, fd);
    return 1;
}
