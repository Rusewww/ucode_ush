#include "ush.h"

bool mx_check_escape(char *cmnd) {
    unsigned int i = 0;
    while (cmnd[i]) {
        bool c = false;

        if (cmnd[i] == '\\') {
            if (mx_isescape_char(cmnd, i)) {
                continue;
            }
            unsigned int j = 0;
            while (MX_ESCAPE_CHARS[j]) {
                if (cmnd[i + 1] == MX_ESCAPE_CHARS[j]) {
                    c = true;
                    break;
                }
                j++;
            }
            if (!c) {
                return c;
            }
        }
        i++;
    }
    return true;
}
