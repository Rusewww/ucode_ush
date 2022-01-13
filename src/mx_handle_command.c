#include "../inc/ush.h"

void mx_handle_command(char *cmnd, int *code) {
    char **cmnds = mx_parse_command(cmnd, code);
    char **argum = NULL;
    t_map **map = mx_get_lenv();

    if (*code || !cmnds) {
        mx_put_map(map, "?", mx_itoa(*code));
        return;
    }
    unsigned int i = 0;
    while (cmnds[i]) {
        if (!(argum = mx_interpretate(cmnds[i], code))) {
            mx_put_map(map, "?", mx_itoa(*code));
            continue;
        }
        *code = mx_exec_command(argum, 1);
        mx_put_map(map, "?", mx_itoa(*code));
        mx_del_strarr(&argum);
        i++;
    }
    mx_del_strarr(&cmnds);
}
