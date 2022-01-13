#include "../inc/ush.h"

static char *get_env(char *var, unsigned int *length, int add, char *arg) {
    char *res = mx_get_var_val(SHELL, var);
    if (res) {
        *length += add;
        return strdup(res);
    }
    return strndup(arg, 0);
}

static char *get_dir(char *arg, unsigned int *length, unsigned int index, unsigned int save) {
    char *d_name = NULL;
    char *t_name = NULL;
    char *user_in_file = NULL;

    t_name = strndup(arg + save, index - save);
    d_name = mx_strjoin("/Users/", t_name);
    user_in_file = mx_check_user_file(t_name);
    if ((!mx_check_user(t_name) && !user_in_file) || !strcmp(t_name, ".") || !strcmp(t_name, "..") ||
        !strcmp(t_name, "Shared")) {
        mx_strdel(&t_name);
        mx_strdel(&d_name);
        return strndup(arg + save - 1, 1);
    }
    *length += index - save;
    mx_strdel(&t_name);
    if (user_in_file) {
        mx_strdel(&d_name);
        return user_in_file;
    }
    return d_name;
}

static char *get_tilde_sub(char *arg, unsigned int index, unsigned int *length) {
    bool is_last = false;
    unsigned int save = index + 1;

    *length = 0;
    index++;
    is_last = isspace(arg[index + 1]) || !arg[index + 1];
    if (arg[index] == '/' || isspace(arg[index]) || !arg[index]) {
        return get_env("HOME", length, 0, arg);
    }
    if (arg[index] == '-' && (arg[index + 1] == '/' || is_last)) {
        return get_env("OLDPWD", length, 1, arg);
    }
    if (arg[index] == '+' && (arg[index + 1] == '/' || is_last)) {
        return get_env("PWD", length, 1, arg);
    } else if (arg[index]) {
        for (; !isspace(arg[index]) && arg[index] != '/' && arg[index]; index++);
        return get_dir(arg, length, index, save);
    }
    return strndup(arg, 1);
}

static void add_sub(char *res, char *arg, unsigned int *i_s) {
    char *sub = NULL;
    strncat(res, arg + i_s[1], i_s[0] - i_s[1]);
    sub = get_tilde_sub(arg, i_s[0], &i_s[2]);
    strcat(res, sub);
    i_s[0] += i_s[2];
    i_s[1] = i_s[0] + 1;
    mx_strdel(&sub);
}

char *mx_replace_tilde(char *arg) {
    char *res = mx_strnew(ARG_MAX);
    unsigned int i_s[3] = {0, 0, 0};
    bool is_quotes = false;
    while (mx_get_char_index(&arg[i_s[0]], '~') >= 0) {
        mx_skip_exps_quotes(arg, &i_s[0], &is_quotes);
        if ((arg[i_s[0]] == '~' && !mx_is_escape_char(arg, i_s[0])) &&
            ((i_s[0] > 0 && isspace(arg[i_s[0] - 1]) && !mx_isescape_char(arg, i_s[0] - 1)) || (i_s[0] == 0))) {
            add_sub(res, arg, i_s);
        }
        i_s[0]++;
    }
    strcat(res, arg + i_s[1]);
    mx_strdel(&arg);
    return res;
}
