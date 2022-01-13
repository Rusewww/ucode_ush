#include "../inc/ush.h"

void mx_increase_sh_lvl(void) {
    int lvl_up = 1;
    char *curr_lvl = mx_get_var_val(SHELL, "SHLVL");
    char sh_lvl[NAME_MAX];
    char *arg[2] = {NULL, NULL};

    if (curr_lvl != NULL) {
        lvl_up += atoi(curr_lvl);
    }
    sprintf(sh_lvl, "SHLVL=%d", lvl_up);
    arg[0] = strdup(sh_lvl);
    mx_export(arg, 1);
    mx_strdel(&arg[0]);
}
