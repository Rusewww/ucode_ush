#include "ush.h"


char *mx_get_output_fd(int fd) {
    size_t bytes = 0;
    char *out = NULL;
    char buff[128];

    while ((bytes = read(fd, buff, sizeof(buff) - 1)) > 0) {
        buff[bytes] = '\0';
        char *tmp = mx_strjoin(*out, buff);
        mx_strdel(out);
        *out = mx_strdup(tmp);
        mx_strdel(&tmp);
    }
    return out;
}

static char *get_output(char **arg, unsigned int len) {
    int fds[2];
    char *out = NULL;
    char *save = NULL;
    pipe(fds);
    mx_exec_fork(arg, fds[1]);
    close(fds[1]);
    out = mx_get_output_fd(fds[0]);
    close(fds[0]);
    if (out) {
        save = out;
        out = mx_strtrim(out);
        mx_strdel(&save);
        if (strlen(out) + len > ARG_MAX - 1) {
            return NULL;
        }
    }
    return out;
}

static void parse_commands(char **commands, char *sub, char *arg, int *code) {
    char **arguments = NULL;
    char *out = NULL;
    unsigned int i = 0;
    while (commands[i]) {
        if (!(arguments = mx_interpretate(commands[i], code))) {
            mx_del_strarr(&commands);
            mx_strdel(&sub);
            continue;
        }
        out = get_output(arguments, strlen(arg));
        if (out) {
            strcat(arg, out);
            mx_strdel(&out);
        }
        mx_del_strarr(&arguments);
        i++;
    }
}

bool mx_get_sub(char *arg, char *sub, int *code) {
    char **commands = NULL;
    bool sub_t = true;
    char *sub_tr = NULL;
    if (sub[0] == '$') {
        sub_t = false;
    }
    if (mx_remove_subchar(sub)) {
        sub_tr = mx_strtrim(sub);
        if (mx_check_trimmed_str(sub_tr, sub))
            return true;
        commands = mx_parse_command(sub, code);
    }
    if (*code || !commands) {
        return false;
    }
    if (sub_t) {
        mx_replace_sub_escapes(commands);
    }
    parse_commands(commands, sub, arg, code);
    mx_strdel(&sub);
    mx_del_strarr(&commands);
    return true;
}
