#include "ush.h"

char **mx_parse_command(char *command, int *code) {
    char **cmd = NULL;
    char *cmd_trimmed = mx_strtrim(command);
    if (!strlen(cmd_trimmed)) {
        mx_strdel(&cmd_trimmed);
        return NULL;
    }
    mx_strdel(&cmd_trimmed);
    if ((*code = mx_preinterpretate(command))) {
        return NULL;
    }
    cmd = mx_split_commands(command);
    if (!mx_check_semicolons(cmd, code)) {
        mx_del_strarr(&cmd);
        return NULL;
    }
    return cmd;
}
