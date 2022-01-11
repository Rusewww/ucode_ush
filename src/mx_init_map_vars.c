#include "ush.h"

static bool check_pwd(char *pwd) {
    char *str = NULL;
    char *curr_dir = getwd(NULL);

    if (pwd) {
        str = realpath(pwd, NULL);
        if (str && curr_dir && !strcmp(curr_dir, str)) {
            mx_strdel(&str);
            mx_strdel(&curr_dir);
            return true;
        }
    }
    if (curr_dir)
        mx_strdel(&curr_dir);
    return false;
}

static void export_pwd_var(char *name, char *val) {
    char var[NAME_MAX];
    char *arg[2] = {NULL, NULL};

    sprintf(var, "%s=%s", name, val);
    arg[0] = strdup(var);
    mx_export(arg, 1);
    mx_strdel(&arg[0]);
}

static void init_pwd_vars(t_map **map, char *path) {
    mx_put_map(map, strdup("OLDPWD"), strdup(""));
    export_pwd_var("OLDPWD", "");
    if (check_pwd(getenv("PWD")))
        mx_put_map(map, strdup("PWD"), strdup(getenv("PWD")));
    else {
        mx_put_map(map, strdup("PWD"), strdup(path));
        export_pwd_var("PWD", path);
    }
}

void mx_init_map_vars(void) {
    t_map **map = mx_get_lenv();
    char path[PATH_MAX];

    *map = mx_create_map(40);
    getcwd(path, sizeof(path));
    init_pwd_vars(map, path);
    mx_put_map(map, strdup("?"), strdup("0"));
    mx_put_map(map, strdup("#"), strdup("0"));
    mx_put_map(map, strdup("0"), strdup(MX_SHELL_NAME));
    mx_put_map(map, strdup("_"), strdup(path));
    mx_put_map(map, strdup("$"), mx_itoa(getpid()));
    mx_put_map(map, strdup("*"), strdup(""));
    mx_put_map(map, strdup("@"), strdup(""));
}
