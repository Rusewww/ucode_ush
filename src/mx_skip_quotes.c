#include "ush.h"

void mx_skip_quotes(char *command, unsigned int *i, char c) {
    if (c == MX_S_QUOTES) {
        if (command[*i] == MX_S_QUOTES && !mx_isescape_char(command, *i)) {
            (*i)++;
            for (; command[*i]; (*i)++) {
                if (command[*i] == MX_S_QUOTES) {
                    break;
                }
            }
        }
        return;
    }
    if (command[*i] == c && !mx_isescape_char(command, *i)) {
        (*i)++;
        for (; command[*i]; (*i)++) {
            if (command[*i] == c
                && !mx_isescape_char(command, *i)) {
                break;
            }\
        }
    }
}
