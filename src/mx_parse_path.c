#include "ush.h"

static int count_size_of_path(char **sp_path) {
    int size = 0;
    int i = 0;
    while (sp_path[i]) {
        if (!strcmp(sp_path[i], "")) {
            continue;
        }
        for (int j = 0; sp_path[i][j]; j++ && size++);
        size++;
        i++;
    }
    return size;
}

static char *collect_path(char **sp_path) {
    int size = count_size_of_path(sp_path);
    char *path = mx_strnew(size);
    bool null_path = true;
    int i = 0;
    while (sp_path[i]) {
        if (strcmp(sp_path[i], "") != 0) {
            null_path = false;
        }
        i++;
    }
    if (null_path) {
        mx_strdel(&path);
        return strdup("/");
    }
    path[0] = '/';
    i = 0;
    while (sp_path[i]) {
        if (strcmp(sp_path[i], "") != 0) {
            path = strcat(path, sp_path[i]);
            if (sp_path[i + 1] != NULL) {
                path = strcat(path, "/");
            }
        }
        i++;
    }
    return path;
}

static char *check_path(char *path) {
    char **sp_path = mx_strsplit(path, '/');
    char *res;
    int i = 0;

    while (sp_path[i]) {
        if (!strcmp(sp_path[i], ".")) {
            mx_strdel(&sp_path[i]);
            sp_path[i] = strdup("");
        }
        if (!strcmp(sp_path[i], "..")) {
            mx_make_null_index(sp_path, i);
        }
        i++;
    }
    res = collect_path(sp_path);
    mx_del_strarr(&sp_path);
    return res;
}

static char *make_bad_path(char *path, char *new_dir) {
    int new_dir_slash;
    int path_slash;
    char *temp;

    (new_dir[0] == '/') ? new_dir_slash = 1 : 0;
    int i = 0;
    while (path[i]) {
        path_slash = (path[i] == '/') ? 1 : 0;
        i++;
    }
    if (path_slash == 1 && new_dir_slash == 1) {
        new_dir++;
        return mx_strjoin(path, new_dir);
    }
    if (path_slash != 1 && new_dir_slash != 1) {
        temp = mx_strjoin(path, "/");
        path = mx_strjoin(temp, new_dir);
        mx_strdel(&temp);
        return path;
    }
    return mx_strjoin(path, new_dir);
}

char *mx_parse_path(char *path, char *new_dir, t_map **map) {
    char *temp;

    if (new_dir == NULL) {
        return strdup(getenv("HOME"));
    }
    if (!strcmp(new_dir, "/")) {
        return strdup(new_dir);
    }
    if (!strcmp(new_dir, "~OLDPWD")) {
        return mx_get_map(map, "OLDPWD");
    }
    if (new_dir[0] == '/') {
        temp = check_path(new_dir);
        return temp;
    }
    temp = make_bad_path(path, new_dir);
    path = check_path(temp);
    mx_strdel(&temp);
    temp = mx_strndup(path, mx_strlen(path));
    mx_strdel(&path);
    return temp;
}
