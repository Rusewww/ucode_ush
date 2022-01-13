#include "../inc/ush.h"

char *mx_replace_escape(char *arg, char *esc, char new, bool in_q) {
    bool q = false;
    char *res = mx_strnew(ARG_MAX);
    unsigned int index = 0;
    unsigned int i = 0;
    while (i < strlen(arg)) {
        if (arg[i] == MX_D_QUOTES && !mx_is_escape_char(arg, i)) {
            q = !q;
        }
        if (arg[i] == esc[0] && arg[i + 1] == esc[1] && mx_is_escape_char(arg, i + 1) && (q || in_q)) {
            i++;
            arg[i] = new;
        }
        res[index++] = arg[i];
        i++;
    }
    mx_strdel(&arg);
    return res;
}
