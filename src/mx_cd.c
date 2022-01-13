#include "../inc/ush.h"

static bool check_path(t_map **map, char *new_dir) {
    char *path = mx_strnew(mx_strlen(mx_get_map(map, "PWD")) + 1 + mx_strlen(new_dir));
    char *real_path;
    bool result = false;
    if (mx_check_exists(new_dir) == false) {
        mx_strdel(&path);
        return false;
    }
    path = strcpy(path, mx_get_map(map, "PWD"));
    path = strcat(path, "/");
    path = strcat(path, new_dir);
    real_path = realpath(path, NULL);
    if (strcmp(path, real_path) != 0) {
        result = true;
    }
    mx_strdel(&real_path);
    mx_strdel(&path);
    return result;
}

static bool check_link(t_map **map, char *new_dir) {
    char *pwd = mx_strjoin(mx_get_map(map, "PWD"), "/");
    char *real_pwd = realpath(pwd, NULL);
    char *path = mx_strnew(mx_strlen(real_pwd) + 1 + mx_strlen(new_dir));
    char *real_path;
    bool result;

    path = strcpy(path, real_pwd);
    path = strcat(path, "/");
    path = strcat(path, new_dir);
    mx_strdel(&pwd);
    mx_strdel(&real_pwd);
    real_path = realpath(path, NULL);
    path = mx_clear_slash_end(mx_parse_path(NULL, path, map));
    result = (mx_strcmp(path, real_path) == 0) ? false : true;
    mx_strdel(&path);
    mx_strdel(&real_path);
    return result;
}

static int flag_s(t_map **map, char *new_dir) {
    if (new_dir == NULL) {
        mx_change_dir(NULL, map, 1);
    } else if (new_dir[0] == '/') {
        if (mx_check_exists(new_dir) == false) {
            fprintf(stderr, "cd: %s: No such file or directory\n", new_dir);
            return 0;
        }
        mx_check_s_slash(new_dir, map);
    } else {
        if (check_link(map, new_dir)) {
            fprintf(stderr, "cd: %s: is not a directory.\n", new_dir);
            return 0;
        } else
            mx_change_dir(new_dir, map, 1);
    }
    return 1;
}

void mx_flag_p_path(t_map **map, char *new_dir) {
    if (check_path(map, new_dir)) {
        char *pwd = strdup(mx_get_map(map, "PWD"));
        char *path = mx_strnew(mx_strlen(pwd) + 1 + mx_strlen(new_dir));
        char *real_path;
        path = strcpy(path, pwd);
        path = strcat(path, "/");
        path = strcat(path, new_dir);
        real_path = realpath(path, NULL);
        chdir("/");
        chdir(real_path);
        mx_strdel(&pwd);
        mx_change_map(map, real_path);
        mx_put_pwd(real_path, mx_get_map(map, "OLDPWD"));
        mx_strdel(&path);
        mx_strdel(&real_path);
    } else {
        mx_change_dir(new_dir, map, 1);
    }
}

void mx_flag_p_slash(char *nd, t_map **map) {
    if (nd == NULL) {
        mx_change_dir(nd, map, 1);
    } else if (nd[0] == '/') {
        char *str = realpath(nd, NULL);
        if (mx_check_exists(nd) == false) {
            fprintf(stderr, "cd: %s: No such file or directory\n", nd);
            mx_strdel(&str);
        } else if (strcmp(nd, str) == 0) {
            mx_change_dir(nd, map, 1);
        } else {
            mx_change_dir(str, map, 1);
        }
        mx_strdel(&str);
    } else {
        mx_flag_p_path(map, nd);
    }
}

void mx_cd_flags(char *flag, t_map **map, char *new_dir) {
    if (strcmp(flag, "-Ps") == 0 || strcmp(flag, "-sP") == 0) {
        if (flag_s(map, new_dir) == 1) {
            flag = "-P";
        }
        new_dir = ".";
    }
    if (strcmp(flag, "-P") == 0) {
        mx_flag_p_slash(new_dir, map);
    } else if (strcmp(flag, "-s") == 0) {
        flag_s(map, new_dir);
    }
}

int mx_cd(char **split, int dir) {
    t_map **map = mx_get_lenv();

    int check_on_flags = 1;

    if (!strcmp(split[0], "-P") || !strcmp(split[0], "-sP") || !strcmp(split[0], "-Ps")) {
        mx_cd_flags(split[0], map, split[1]);
    } else if (!strcmp(split[0], "-s")) {
        mx_cd_flags("-s", map, split[1]);
    } else if (!strcmp(split[0], "-") && split[1] == NULL) {
        mx_change_dir("~OLDPWD", map, dir);
    } else if ((!strcmp(split[0], "-") && split[1] != NULL)
               || (split[0][0] == '-' && split[1] != NULL)) {
        fprintf(stderr, "cd: string not in pwd: %s\n", split[0]);
    } else if (!strcmp(split[0], "/")) {
        mx_change_dir(split[0], map, 1);
    } else {
        check_on_flags = 0;
    }

    if (!mx_arr_size(split)) {
        mx_change_dir(NULL, map, dir);
    } else if (!check_on_flags) {
        mx_change_dir(split[0], map, dir);
    }
    return 1;
}

void mx_change_dir(char *new_dir, t_map **map, int dir) {
    int res;
    if (new_dir == NULL) {
        mx_home(map);
        return;
    } else if (!strcmp(new_dir, "~OLDPWD")) {
        mx_old_pwd(new_dir, map, dir);
        return;
    }
    res = chdir(new_dir);
    if (res < 0) {
        fprintf(stderr, "cd: %s: ", new_dir);
        perror("");
    } else {
        mx_change_map(map, new_dir);
    }
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
