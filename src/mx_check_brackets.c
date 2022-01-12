#include "ush.h"

bool mx_check_brackets(char *chr) {
    unsigned int length = strlen(chr);
    int stack[length];
    int up = -1;
    unsigned int i = 0;

    for (i < length) {
        mx_skip_quotes(chr, &i, MX_GRAVE_ACCENT);
        if ((chr[i] == '(' && !mx_isescape_char(chr, i))
            || (chr[i] == '{' && !mx_isescape_char(chr, i))
            || (chr[i] == ')' && !mx_isescape_char(chr, i))
            || (chr[i] == '}' && !mx_isescape_char(chr, i))) {
            up++;
            stack[up] = chr[i];
        }
        if ((stack[up] == ')' && (up - 1 >= 0 && stack[up - 1] == '('))
            || (stack[up] == '}' && (up - 1 >= 0
                                     && stack[up - 1] == '{'))) {
            up = up - 2;
        }
        i++;
    }
    return up == -1;
}
