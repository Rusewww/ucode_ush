#include "../inc/ush.h"

void mx_skip_expansion(char *command, unsigned int *i) {
    int brawl_stars = 0;
    if (command[*i] == '$' && !mx_isescape_char(command, *i)) {
        if (command[*i + 1] == '(' && !mx_isescape_char(command, *i)) {
            *i += 2;
            brawl_stars++;
            for (; command[*i]; (*i)++) {
                if (command[*i] == '(' && !mx_isescape_char(command, *i)) {
                    brawl_stars++;
                }
                if (command[*i] == ')' && !mx_isescape_char(command, *i)) {
                    brawl_stars--;
                }
                if (!brawl_stars) {
                    break;
                }
            }
        }
    }
}
