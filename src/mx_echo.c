#include "../inc/ush.h"

static unsigned int set_flags(bool *is_nl, bool *is_e, char **argv) {
    unsigned int ind = 0;

    while (argv[ind]) {
        if (mx_match(argv[ind], "^-[nEe]+$")) {
            unsigned int i = 0;
            while (i < strlen(argv[ind])) {
                if (argv[ind][i] == 'E') {
                    *is_e = false;
                }
                if (argv[ind][i] == 'e') {
                    *is_e = true;
                }
                if (argv[ind][i] == 'n') {
                    *is_nl = false;
                }
                i++;
            }
        } else {
            break;
        }
        ind++;
    }
    return ind;
}

static char *replace_octal(char *arg) {
    char *res = mx_strnew(strlen(arg));
    char *octal = NULL;
    char *tmp = arg;
    int ind = 0;
    int num_size = 0;

    while ((ind = mx_get_substr_index(arg, "\\0")) >= 0) {
        strncat(res, arg, ind);
        for (; arg[++ind] >= '0' && arg[ind] <= '7' && arg[ind]; num_size++);
        octal = strndup(arg + ind - num_size, num_size);
        res[strlen(res)] = (char) strtol(octal, NULL, 8);
        mx_strdel(&octal);
        arg += ind;
        num_size = 0;
    }
    strcat(res, arg);
    mx_strdel(&tmp);
    return res;
}

static char *replace_escape(char *arg, bool *null) {
    int ind = 0;
    char *res = mx_strnew(ARG_MAX);

    if ((ind = mx_get_substr_index(arg, "\\c")) >= 0) {
        strncpy(res, arg, ind);
        *null = false;
    } else {
        strcpy(res, arg);
    }
    res = mx_replace_escape(res, "\\a", '\x07', true);
    res = mx_replace_escape(res, "\\b", '\x08', true);
    res = mx_replace_escape(res, "\\f", '\x0c', true);
    res = mx_replace_escape(res, "\\n", '\x0a', true);
    res = mx_replace_escape(res, "\\r", '\x0d', true);
    res = mx_replace_escape(res, "\\t", '\x09', true);
    res = mx_replace_escape(res, "\\v", '\x0b', true);
    res = mx_replace_escape(res, "\\\\", '\\', true);
    res = replace_octal(res);
    return res;
}

int mx_echo(char **args, int dr) {
    bool null = true;
    bool e = false;
    unsigned int ind = 0;
    char *output = NULL;

    ind = set_flags(&null, &e, args);
    while (args[ind]) {
        if (e) {
            output = replace_escape(args[ind], &null);
        } else {
            output = strdup(args[ind]);
        }
        dprintf(dr, "%s", output);
        mx_strdel(&output);
        ind++;
        if (args[ind]) {
            dprintf(dr, " ");
        }
    }

    if (null) {
        dprintf(dr, "\n");
    }
    return 0;
}
