#include "ush.h"

t_list *mx_split_command(char *command) {
    t_list *res = NULL;
    unsigned int save = 0;
    unsigned int i = 0;
    while (i < strlen(command)) {
        mx_skip_quotes(command, &i, MX_D_QUOTES);
        mx_skip_quotes(command, &i, MX_S_QUOTES);
        if (isspace(command[i]) && !mx_isescape_char(command, i)) {
            save++;
            continue;
        }
        if (!command[i + 1] || (isspace(command[i + 1]) && !mx_isescape_char(command, i + 1))) {
            mx_push_back(&res, strndup(command + save, i - save + 1));
            save = i + 1;
        }
        i++;
    }
    return res;
}
