#include "ush.h"

bool mx_check_chars(char *cmd) {
    int i = 0;
    while (i < (int) strlen(cmd)) {
        mx_skip_quotes(cmd, &i, MX_D_QUOTES);
        mx_skip_quotes(cmd, &i, MX_S_QUOTES);
        int j = 0;
        while (j < (int) strlen(MX_FORBIDDEN_CHARS)) {
            if (cmd[i] == MX_FORBIDDEN_CHARS[j]
                && !mx_isescape_char(cmd, i)) {
                return false;
            }
            j++;
        }
        i++;
    }
    return true;
}
