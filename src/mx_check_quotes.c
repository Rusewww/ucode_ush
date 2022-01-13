#include "ush.h"

/*static void skip_quotes_expansion(char *cmnd, unsigned int *i, bool s_quotes) {
    if (!s_quotes) {
        mx_skip_quotes(cmnd, i, MX_GRAVE_ACCENT);
        mx_skip_expansion(cmnd, i);
    }
}*/

bool mx_check_quotes(char *cmnd) {
    size_t length = strlen(cmnd);
    bool s_quotes = false;
    bool d_quotes = false;
    unsigned int i = 0;

    while (i < length) {
        if (!s_quotes) {
            mx_skip_quotes(cmnd, i, MX_GRAVE_ACCENT);
            mx_skip_expansion(cmnd, i);
        }
        if (cmnd[i] == MX_D_QUOTES && !mx_isescape_char(cmnd, i) && !s_quotes) {
            d_quotes = !d_quotes;
        }
        if (cmnd[i] == MX_S_QUOTES && !d_quotes) {
            if (!s_quotes) {
                if (!mx_isescape_char(cmnd, i)) {
                    s_quotes = !s_quotes;
                }
            } else if (s_quotes) {
                s_quotes = !s_quotes;
            }
        }
        i++;
    }
    return !s_quotes && !d_quotes;
}
