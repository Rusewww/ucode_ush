#include "ush.h"

bool mx_check_substitutions(char *cmnd) {
    bool g_quotes = false;
    size_t length = strlen(cmnd);

    unsigned int i = 0;

    while (i < length) {
        mx_skip_quotes(cmnd, &i, MX_S_QUOTES);
        mx_skip_expansion(cmnd, &i);
        if (cmnd[i] == MX_GRAVE_ACCENT && !mx_isescape_char(cmnd, i)) {
            g_quotes = !g_quotes;
        }
        i++;
    }
    return !g_quotes;
}
