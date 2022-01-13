#include "../inc/ush.h"

static bool parse_flags(char **flags, bool *mode) {
    bool stop_fl = 0;
    int i = 0;

    while (flags[i]) {
        if (flags[i][0] == '-' && !stop_fl) {
            stop_fl = mx_is_flag_stop(flags[i]);
            int j = 1;
            while (flags[i][j] != '\0') {
                if (flags[i][j] == 'P') {
                    *mode = 1;
                } else if (flags[i][j] != 'L' && (flags[i][j] != '-' || (flags[i][j] == '-' && j == 2))) {
                    fprintf(stderr, "pwd: bad option: -%c\n", flags[i][j]);
                    return 1;
                }
                j++;
            }
        } else {
            fprintf(stderr, "pwd: too many arguments\n");
            return 1;
        }
        i++;
    }
    return 0;
}

int mx_pwd(char **flags, int fd) {
    t_map **map = mx_get_lenv();
    bool mode = 0;
    char *pwd = mx_get_map(map, "PWD");
    if (!parse_flags(flags, &mode)) {
        if (mode) {
            char *rpath = realpath(pwd, NULL);
            dprintf(fd, "%s\n", rpath);
            mx_strdel(&rpath);
        } else {
            dprintf(fd, "%s\n", pwd);
        }
        return 0;
    }
    return 1;
}

