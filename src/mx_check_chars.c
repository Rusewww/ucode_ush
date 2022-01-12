#include "ush.h"

bool mx_check_chars(char *cmnd) {
    unsigned int i = 0;
    while (i < strlen(cmnd)) {
        mx_skip_quotes(cmnd, &i, MX_D_QUOTES);
        mx_skip_quotes(cmnd, &i, MX_S_QUOTES);
        unsigned int j = 0;
        while (j < strlen(MX_FORBIDDEN_CHARS)) {
            if (cmnd[i] == MX_FORBIDDEN_CHARS[j]
                && !mx_isescape_char(cmnd, i)) {
                return false;
            }
            j++;
        }
        i++;
    }
    return true;
}
