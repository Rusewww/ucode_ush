#include "ush.h"

static char *format_quotes(char *str) {
    char *res = mx_strnew(ARG_MAX);
    unsigned int length = strlen(str);
    unsigned int index = 0;
    bool is_quotes[2] = {false, false};
    unsigned int i = 0;

    while (i < length) {
        if ((str[i] == '\'') && !mx_isescape_char(str, i) && !is_quotes[1])
            is_quotes[0] = !is_quotes[0];
        if ((str[i] == '\"') && !mx_isescape_char(str, i) && !is_quotes[0])
            is_quotes[1] = !is_quotes[1];
        if (is_quotes[1] && !is_quotes[0] && mx_isescape_char(str, i + 1)
            && (str[i + 1] == '$' || str[i + 1] == '\\')) {
            i++;
        }
        res[index++] = str[i];
        i++;
    }
    mx_strdel(&str);
    return res;
}

static char *get_formated_arg(char *str) {
    char *res = mx_strnew(ARG_MAX);
    unsigned int length = strlen(str);
    unsigned int index = 0;
    bool is_quotes[2] = {false, false};
    unsigned int i = 0;
    while (i < length) {
        if ((str[i] == '\'') && !is_quotes[1]) {
            is_quotes[0] = !is_quotes[0];
            continue;
        }
        if ((str[i] == '\"') && !mx_isescape_char(str, i) && !is_quotes[0]) {
            is_quotes[1] = !is_quotes[1];
            continue;
        }
        if (mx_isescape_char(str, i + 1) && !is_quotes[0] && !is_quotes[1])
            i++;
        res[index++] = str[i];
        i++;
    }
    return res;
}

static char **get_result(char *command) {
    char **res = NULL;
    t_list *arg = NULL;
    int length = 0;

    arg = mx_split_command(command);
    length = mx_list_size(arg);
    res = malloc(sizeof(char *) * (length + 1));
    res[length] = NULL;
    t_list *cur = arg;
    for (unsigned int i = 0; cur; cur = cur->next) {
        cur->data = format_quotes(cur->data);
        res[i++] = get_formated_arg(cur->data);
    }
    mx_del_list(&arg);
    mx_strdel(&command);
    return res;
}

char **mx_interpretate(char *command, int *code) {
    if (!(command = mx_replace_env(command, code))) {
        fprintf(stderr, "%s: bad substitution\n", MX_SHELL_NAME);
        return NULL;
    }
    command = mx_replace_tilde(command);
    if (!(command = mx_replace_substitution(command, code))) {
        fprintf(stderr, "%s: unmatched quote\n", MX_SHELL_NAME);
        return NULL;
    }
    if (!strlen(command)) {
        mx_strdel(&command);
        return NULL;
    }
    return get_result(command);
}
