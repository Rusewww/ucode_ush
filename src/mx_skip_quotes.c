#include "ush.h"

static void skip_single_quotes(char *cmnd, int *i) {
    if (cmnd[*i] == MX_S_QUOTES && !mx_isescape_char(cmnd, *i)) {
        (*i)++;
        for (; cmnd[*i]; (*i)++;) {
            if (cmnd[*i] == MX_S_QUOTES) {
                break;
            }
        }
    }
}

void mx_skip_quotes(char *cmnd, int *i, char char) {

    if (char == MX_S_QUOTES) {
        skip_single_quotes(cmnd, i);
        return;
    }

    if (cmnd[*i] == char && !mx_isescape_char(cmnd, *i)) {
        (*i)++;
        for (; cmnd[*i]; (*i)++) {
            if (cmnd[*i] == char
            && !mx_isescape_char(cmnd, *i)) {
                break;
            }
        }
    }
}
