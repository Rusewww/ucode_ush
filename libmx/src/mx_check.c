#include "../inc/libmx.h"

bool mx_check_exists(char *dir) {
    if (chdir(dir) < 0) {
        return false;
    } else {
        chdir("-");
    }
    return true;
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
