#include "../inc/ush.h"

bool mx_check_escape(char *cmnd) {
    unsigned int i = 0;
    while (cmnd[i]) {
        bool hlp = false;
        if (cmnd[i] == '\\') {
            if (mx_isescape_char(cmnd, i)) {
                continue;
            }
            unsigned int j = 0;
            while (MX_ESCAPE_CHARS[j]) {
                if (cmnd[i + 1] == MX_ESCAPE_CHARS[j]) {
                    hlp = true;
                    break;
                }
                j++;
            }
            if (!hlp) {
                return hlp;
            }
        }
        i++;
    }
    return true;
}
