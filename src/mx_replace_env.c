#include "../inc/ush.h"

static unsigned int get_len_spec(char *spec) {
    unsigned int i = 1;
    if (isalpha(spec[i]) || spec[i] == '_') {
        i++;
        while (isnumber(spec[i]) || isalpha(spec[i]) || spec[i] == '_')
            i++;
    } else if (isnumber(spec[i])) {
        i++;
        for (; isnumber(spec[i]); i++);
    }
    return i;
}

static char *check_spec_char(char *arg, unsigned int *len, unsigned int var_len) {
    t_map **map = mx_get_lenv();
    char key[2];
    unsigned int l = var_len ? var_len : get_len_spec(arg);
    unsigned int i = 0;
    while (i < strlen(MX_SPEC_ENV)) {
        if (*arg == MX_SPEC_ENV[i]) {
            strncpy(key, arg, 1);
            *len += 1;
            return strdup(mx_get_map(map, key));
        }
        i++;
    }
    if (l > 1) {
        return NULL;
    }
    return NULL;
}

static char *get_brace_sub(char *arg, unsigned int index, unsigned int *len) {
    int close_index = mx_get_char_index(arg + index + 1, '}');
    char *var = strndup(arg + index + 1, close_index);
    char *env = NULL;
    if ((env = check_spec_char(arg + index + 1, len, strlen(var)))) {
        mx_inc_val_var(len, 2, var);
        return env;
    }
    if (mx_match(var, "^[0-9]*$")) {
        mx_inc_val_var(len, strlen(var) + 2, var);
        return strdup("");
    } else if (!mx_match(var, MX_ENV_NAME) && strlen(var)) {
        mx_strdel(&var);
        return NULL;
    }
    env = mx_get_var_val(SHELL, var);
    mx_inc_val_var(len, strlen(var) + 2, var);
    return env ? strdup(env) : strdup("");
}

static char *get_spec_sub(char *arg, unsigned int index, unsigned int *length) {
    char *var = NULL;
    char *env = NULL;

    if ((env = check_spec_char(arg + index, length, 0))) {
        return env;
    }
    if (arg[index] == '{') {
        return get_brace_sub(arg, index, length);
    }
    if (isalpha(arg[index]) || arg[index] == '_') {
        *length = get_len_spec(arg + index);
        var = strndup(arg + index, *length);
        env = mx_get_var_val(SHELL, var);
        *length = strlen(var);
        mx_strdel(&var);
        return env ? strdup(env) : strdup("");
    } else if (isnumber(arg[index])) {
        *length = 1;
        return strdup("");
    }
    return strndup(arg + index - 1, 1);
}

char *mx_replace_env(char *arg, int *code) {
    char *res = mx_strnew(ARG_MAX);
    unsigned int length = 0;
    unsigned int index[2] = {0, 0};
    bool is_quotes = false;

    while (mx_get_char_index(&arg[index[0]], '$') >= 0) {
        mx_skip_exps_quotes(arg, &index[0], &is_quotes);
        if (arg[index[0]] == '$' && !mx_isescape_char(arg, index[0])) {
            strncat(res, arg + index[1], index[0] - index[1]);
            length = 0;
            if (!mx_replace_env_var(res, get_spec_sub(arg, index[0] + 1, &length), index, length)) {
                *code = 1;
                return NULL;
            }
        }
        index[0]++;
    }
    strcat(res, arg + index[1]);
    return res;
}
