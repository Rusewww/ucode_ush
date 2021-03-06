#include "../inc/ush.h"

static char *get_flag_arg(char **args, int *flag_idx, int *rval, int *idx) {
    if (args[0][(*flag_idx) + 1] == '\0') {
        if (args[1] == NULL) {
            *rval = mx_print_env_error(args[0][*flag_idx], NULL, 1);
        } else {
            *flag_idx = -1;
            (*idx)++;
            return strdup(args[1]);
        }
    } else {
        int shift = (*flag_idx) + 1;
        *flag_idx = -1;
        return strdup(args[0] + shift);
    }
    return NULL;
}

static void delete_var(char **dname) {
    unsetenv(*dname);
    if (dname != NULL) {
        mx_strdel(dname);
    }
}

static void env_check_del_name(char *dname, int *rval) {
    if (!(*rval)) {
        *rval = mx_match(dname, "=");
        if (*rval) {
            mx_print_env_error(0, dname, 2);
        }
    }
}

int parse_flag(char **args, char **path, int *idx) {
    int retval = 0;
    int i = 1;
    while (args[0][i] != '\0') {
        if (args[0][i] == 'P') {
            if (*path != NULL) {
                mx_strdel(path);
            }
            *path = get_flag_arg(args, &i, &retval, idx);
        } else if (args[0][i] == 'u') {
            char *del_name = get_flag_arg(args, &i, &retval, idx);
            env_check_del_name(del_name, &retval);
            delete_var(&del_name);
        } else if (args[0][i] != 'i' && args[0][i] != '-') {
            return mx_print_env_error(args[0][i], NULL, 0);
        }
        if (retval || i == -1) {
            break;
        }
        i++;
    }
    return retval;
}

int mx_env_flags_parse(char **argv, char **path, int *idx) {
    int rval = 0;
    bool stop = false;
    extern char **environ;
    for (*idx = 0; argv[*idx] && !stop && !rval;) {
        if (argv[*idx][0] == '-' && strcmp(argv[*idx], "--") != 0) {
            if (mx_match(argv[*idx], MX_ENV_FLAG_I)) {
                mx_clear_env();

            }
            rval = parse_flag(&argv[*idx], path, idx);
        } else {
            if (strcmp(argv[*idx], "--") != 0) {
                (*idx)--;
            }
            stop = 1;
        }
        (*idx)++;
    }
    return rval;
}

void mx_env_vars_parse(char **argv, char **path, int *idx) {
    char *name = NULL;
    bool repl = false;
    while (argv[*idx]) {
        if (mx_match(argv[*idx], MX_ENV_VAR)) {
            mx_get_name(argv[*idx], &name);
            mx_put_env(argv[*idx]);
            if (!strcmp(name, "PATH") && (!(*path) || repl)) {
                repl = true;
                if (*path) {
                    mx_strdel(path);
                }
                *path = mx_get_var_info(argv[*idx], 1);
            }
        } else {
            break;
        }
        (*idx)++;
    }
    if (!(*path) && mx_get_var_val(SHELL, "PATH")) {
        *path = strdup(mx_get_var_val(SHELL, "PATH"));
    }
    mx_strdel(&name);
}

