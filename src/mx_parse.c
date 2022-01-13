#include "../inc/ush.h"

static int count_size(char **split_path) {
    int size = 0;

    for (int i = 0; split_path[i]; i++) {
        if (!strcmp(split_path[i], ""))
            continue;
        for (int j = 0; split_path[i][j]; j++)
            size++;
        size++;
    }
    return size;
}

static char *collect(char **split_path) {
    int size = count_size(split_path);
    char *path = mx_strnew(size);
    bool null_path = true;

    for (int i = 0; split_path[i]; i++)
        if (strcmp(split_path[i], "") != 0)
            null_path = false;
    if (null_path) {
        mx_strdel(&path);
        return strdup("/");
    }
    path[0] = '/';
    for (int i = 0; split_path[i]; i++)
        if (strcmp(split_path[i], "") != 0) {
            path = strcat(path, split_path[i]);
            if (split_path[i + 1] != NULL)
                path = strcat(path, "/");
        }
    return path;
}

static char *check(char *path) {
    char **split_path = mx_strsplit(path, '/');
    char *result;

    for (int i = 0; split_path[i]; i++) {
        if (!strcmp(split_path[i], ".")) {
            mx_strdel(&split_path[i]);
            split_path[i] = strdup("");
        }
        if (!strcmp(split_path[i], ".."))
            mx_make_null_index(split_path, i);
    }
    result = collect(split_path);
    mx_del_strarr(&split_path);
    return result;
}

static char *make_bad(char *path, char *new_dir) {
    int new_dir_slash;
    int path_slash;
    char *temp;

    (new_dir[0] == '/') ? new_dir_slash = 1 : 0;
    for (int i = 0; path[i]; i++)
        path_slash = (path[i] == '/') ? 1 : 0;
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
    char *tmp;
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
        tmp = check(new_dir);
        return tmp;
    }
    tmp = make_bad(path, new_dir);
    path = check(tmp);
    mx_strdel(&tmp);
    tmp = mx_strndup(path, mx_strlen(path));
    mx_strdel(&path);
    return tmp;
}

char **mx_parse_cmnd(char *cmnd, int *code) {
    char **cmd = NULL;
    char *cmd_trimmed = mx_strtrim(cmnd);
    if (!strlen(cmd_trimmed)) {
        mx_strdel(&cmd_trimmed);
        return NULL;
    }
    mx_strdel(&cmd_trimmed);
    if ((*code = mx_pre_interpret(cmnd))) {
        return NULL;
    }
    cmd = mx_split_commands(cmnd);
    if (!mx_check_colons(cmd, code)) {
        mx_del_strarr(&cmd);
        return NULL;
    }
    return cmd;
}
