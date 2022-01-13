#include "../inc/ush.h"

void mx_skip_s_quotes(char *command, unsigned int *i) {
    if (command[*i] == MX_S_QUOTES && !mx_isescape_char(command, *i)) {
        (*i)++;
        for (; command[*i]; (*i)++) {
            if (command[*i] == MX_S_QUOTES
                && !mx_isescape_char(command, *i)) {
                break;
            }
        }
    }
}
