#include "../inc/ush.h"

static int get_next_command(char *command) {
    unsigned int i = 0;
    while (i < strlen(command)) {
        mx_skip_quotes(command, &i, MX_GRAVE_ACCENT);
        mx_skip_quotes(command, &i, MX_S_QUOTES);
        mx_skip_quotes(command, &i, MX_D_QUOTES);
        mx_skip_exp(command, &i);
        if (command[i] == ';') {
            return i;
        }
        if (!command[i + 1] && command[i] != ';') {
            return i + 1;
        }
        i++;
    }
    return -1;
}

static t_list *split(char *command) {
    t_list *commands = NULL;
    int length = 0;
    char *tmp_cmd = strdup(command);
    char *save = tmp_cmd;
    unsigned int i = 0;
    while (length != -1) {
        length = get_next_command(tmp_cmd);
        mx_push_back(&commands, strndup(tmp_cmd, length));
        if ((tmp_cmd[length] == ';' && !tmp_cmd[length + 1]) || !tmp_cmd[length]) {
            break;
        }
        tmp_cmd += length + 1;
        i++;
    }
    mx_strdel(&save);
    return commands;
}

char **mx_split_commands(char *command) {
    t_list *commands = split(command);
    size_t size = mx_list_size(commands);
    char **cmds = malloc(sizeof(char *) * (size + 1));
    unsigned int index = 0;
    cmds[size] = NULL;
    t_list *cur = commands;
    while (cur) {
        cmds[index++] = strdup(cur->data);
        cur = cur->next;
    }
    mx_del_list(&commands);
    return cmds;
}
