#include "ush.h"

void mx_change_map(t_map **map, char *dir) {
    char *tmp = mx_strnew(mx_strlen(mx_get_map(map, "PWD")));
    char *path;

    if (strcmp(dir, "~OLDPWD") != 0) {
        tmp = mx_strcpy(tmp, mx_get_map(map, "PWD"));
        path = mx_parse_path(tmp, dir, map);
        path = mx_clear_slash_end(path);
        mx_put_map(map, "OLDPWD", strdup(mx_get_map(map, "PWD")));
        mx_put_map(map, "PWD", strdup(path));
        mx_put_pwd(path, mx_get_map(map, "OLDPWD"));
        mx_strdel(&path);
        mx_strdel(&tmp);
    } else {
        mx_strdel(&tmp);
        tmp = strdup(mx_get_map(map, "OLDPWD"));
        mx_put_map(map, "OLDPWD", strdup(mx_get_map(map, "PWD")));
        mx_put_map(map, "PWD", tmp);
        mx_put_pwd(tmp, mx_get_map(map, "OLDPWD"));
    }
}
