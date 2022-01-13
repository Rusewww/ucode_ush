#include "../inc/ush.h"

void mx_skip_grave(char *command, unsigned int *i) {
    if (command[*i] == MX_GRAVE_ACCENT && !mx_isescape_char(command, *i)) {
        (*i)++;
        for (; command[*i]; (*i)++;) {
            if (command[*i] == MX_GRAVE_ACCENT
                && !mx_isescape_char(command, *i)) {
                break;
            }
        }
    }
}
