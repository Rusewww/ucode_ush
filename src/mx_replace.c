#include "../inc/ush.h"

static void clear_data(char **s2, char ***a_s1, char ***a_s2) {
    mx_del_strarr(a_s1);
    mx_del_strarr(a_s2);
    mx_strdel(s2);
}

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

static void get_quoted_sub(int *o_sub, int *c_sub, unsigned int *i, char *arg) {
    for (; arg[*i]; *i += 1) {
        mx_skip_quotes(arg, i, MX_S_QUOTES);
        if (arg[*i] == MX_GRAVE_ACCENT
            && !mx_is_escape_char(arg, *i) && *o_sub == -1) {
            *o_sub = *i;
            *i += 1;
            continue;
        }
        if (arg[*i] == MX_GRAVE_ACCENT && !mx_is_escape_char(arg, *i)) {
            *c_sub = *i;
            *i += 1;
            break;
        }
    }
}

static void inc_subs(int *sub, unsigned int *i, int add) {
    *sub = *i;
    *i += add;
}

static void get_parameter_sub(int *o_sub, int *c_sub, unsigned int *i, char *arg) {
    unsigned int brace = 0;
    for (; arg[*i]; *i += 1) {
        mx_skip_quotes(arg, i, MX_S_QUOTES);
        if (arg[*i] == '$' && arg[*i + 1] == '('
            && !mx_is_escape_char(arg, *i) && *o_sub == -1) {
            brace++;
            inc_subs(o_sub, i, 2);
            continue;
        }
        if (arg[*i] == '(' && !mx_is_escape_char(arg, *i))
            brace++;
        if (arg[*i] == ')' && !mx_is_escape_char(arg, *i)) {
            if (brace-- && !brace) {
                inc_subs(c_sub, i, 1);
                break;
            }
        }
    }
}

static char *get_substitution(int *o_sub, int *c_sub, unsigned int *i, char *arg) {
    if (arg[*i] == MX_GRAVE_ACCENT && !mx_is_escape_char(arg, *i)) {
        get_quoted_sub(o_sub, c_sub, i, arg);
    } else if (arg[*i] == '$' && !mx_is_escape_char(arg, *i)) {
        get_parameter_sub(o_sub, c_sub, i, arg);
    }
    if (*o_sub != -1 && *c_sub != -1) {
        return strndup(arg + *o_sub, *c_sub - *o_sub + 1);
    }
    return NULL;
}

static void append(char **result, char *buf) {
    char *tmp_str = mx_strjoin(*result, buf);

    mx_strdel(result);
    *result = mx_strdup(tmp_str);
    mx_strdel(&tmp_str);
}

char *mx_get_output_fd(int fd) {
    size_t bytes = 0;
    char *output = NULL;
    char buf[128];

    while ((bytes = read(fd, buf, sizeof(buf) - 1)) > 0) {
        buf[bytes] = '\0';
        append(&output, buf);
    }
    return output;
}

static char *get_output(char **arguments, unsigned int len) {
    int fds[2];
    char *output = NULL;
    char *save = NULL;

    pipe(fds);
    mx_exec_fork(arguments, fds[1]);
    close(fds[1]);
    output = mx_get_output_fd(fds[0]);
    close(fds[0]);
    if (output) {
        save = output;
        output = mx_strtrim(output);
        mx_strdel(&save);
        if (strlen(output) + len > ARG_MAX - 1)
            return NULL;
    }
    return output;
}

static void parse_commands(char **commands, char *sub, char *arg, int *code) {
    char **arguments = NULL;
    char *output = NULL;

    for (unsigned int i = 0; commands[i]; i++) {
        if (!(arguments = mx_interpretate(commands[i], code))) {
            mx_del_strarr(&commands);
            mx_strdel(&sub);
            continue;
        }
        output = get_output(arguments, strlen(arg));
        if (output) {
            strcat(arg, output);
            mx_strdel(&output);
        }
        mx_del_strarr(&arguments);
    }
}


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
        if (arg[index[0]] == '$' && !mx_is_escape_char(arg, index[0])) {
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

char *mx_replace_escape(char *arg, char *esc, char new, bool in_q) {
    bool q = false;
    char *res = mx_strnew(ARG_MAX);
    unsigned int index = 0;
    unsigned int i = 0;
    while (i < strlen(arg)) {
        if (arg[i] == MX_D_QUOTES && !mx_is_escape_char(arg, i)) {
            q = !q;
        }
        if (arg[i] == esc[0] && arg[i + 1] == esc[1] && mx_is_escape_char(arg, i + 1) && (q || in_q)) {
            i++;
            arg[i] = new;
        }
        res[index++] = arg[i];
        i++;
    }
    mx_strdel(&arg);
    return res;
}

bool mx_replace_env_var(char *res, char *env, unsigned int *index, unsigned int length) {
    if (!env) {
        return false;
    }
    strcat(res, env);
    index[0] += length;
    index[1] = index[0] + 1;
    mx_strdel(&env);
    return true;
}

bool mx_check_trimmed_str(char *sub_trim, char *sub) {
    bool rval = strlen(sub_trim) == 0;
    mx_strdel(&sub_trim);
    if (rval) {
        mx_strdel(&sub);
    }
    return rval;
}

void mx_inc_val_var(unsigned int *length, unsigned int add, char *var) {
    *length += add;
    mx_strdel(&var);
}


bool mx_get_sub(char *arg, char *sub, int *code) {
    char **commands = NULL;
    bool sub_type = true;
    char *sub_trimmed = NULL;

    if (sub[0] == '$')
        sub_type = false;
    if (mx_remove_sub_char(sub)) {
        sub_trimmed = mx_strtrim(sub);
        if (mx_check_trimmed_str(sub_trimmed, sub))
            return true;
        commands = mx_parse_cmnd(sub, code);
    }
    if (*code || !commands)
        return false;
    if (sub_type)
        mx_replace_sub_escapes(commands);
    parse_commands(commands, sub, arg, code);
    mx_strdel(&sub);
    mx_del_strarr(&commands);
    return true;
}

char *mx_replace_substitution(char *arg, int *code) {
    int i_s[3] = {-1, -1, 0};
    bool quote = false;
    char *sub = NULL;
    char *res = mx_strnew(ARG_MAX);
    unsigned int i = 0;
    while (i < strlen(arg)) {
        mx_skip_quotes_if(&quote, arg, &i);
        if ((sub = get_substitution(&i_s[0], &i_s[1], &i, arg))) {
            strncat(res, arg + i_s[2], i_s[0] - i_s[2]);
            if (!mx_get_sub(res, sub, code))
                return mx_get_invalid_sub(&arg, &res, &sub);
            i = i_s[1];
            i_s[2] = i + 1;
        } else if (i_s[0] != -1 && i_s[1] == -1 && !sub) {
            break;
        }
        i++;
        i_s[0] = -1;
        i_s[1] = -1;
    }
    strcat(res, arg + i_s[2]);
    mx_strdel(&arg);
    return res;
}

char *mx_replace_tilde(char *arg) {
    char *res = mx_strnew(ARG_MAX);
    unsigned int i_s[3] = {0, 0, 0};
    bool is_quotes = false;
    while (mx_get_char_index(&arg[i_s[0]], '~') >= 0) {
        mx_skip_exps_quotes(arg, &i_s[0], &is_quotes);
        if ((arg[i_s[0]] == '~' && !mx_is_escape_char(arg, i_s[0])) &&
            ((i_s[0] > 0 && isspace(arg[i_s[0] - 1]) && !mx_is_escape_char(arg, i_s[0] - 1)) || (i_s[0] == 0))) {
            add_sub(res, arg, i_s);
        }
        i_s[0]++;
    }
    strcat(res, arg + i_s[1]);
    mx_strdel(&arg);
    return res;
}

void mx_replace_sub_escapes(char **commands) {
    for (unsigned int i = 0; commands[i]; i++) {
        commands[i] = mx_replace_escape(commands[i], "\\`", MX_GRAVE_ACCENT, true);
        commands[i] = mx_replace_escape(commands[i], "\\\\", '\\', true);
    }
}

bool mx_check_user(char *username) {
    DIR *dir = opendir("/Users/");
    struct dirent *entry;
    while ((entry = readdir(dir))) {
        if (!strcmp(entry->d_name, username)) {
            closedir(dir);
            return true;
        }
    }
    closedir(dir);
    return false;
}

char *mx_check_user_file(char *tmp_name) {
    char *pasfile = mx_file_to_str("/etc/passwd");
    char **lines = mx_strsplit(pasfile, '\n');
    char **data = NULL;
    char *res = NULL;
    unsigned int i = 0;
    while (lines[i]) {
        if (lines[i][0] != '#') {
            data = mx_strsplit(lines[i], ':');
            if (!strcmp(data[0], tmp_name)) {
                res = strdup(data[5]);
                clear_data(&pasfile, &data, &lines);
                return res;
            }
            mx_del_strarr(&data);
        }
        i++;
    }
    mx_del_strarr(&lines);
    mx_strdel(&pasfile);
    return NULL;
}

char *mx_get_invalid_sub(char **arg, char **result, char **sub) {
    mx_strdel(arg);
    mx_strdel(result);
    mx_strdel(sub);
    return NULL;
}
