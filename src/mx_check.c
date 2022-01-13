#include "ush.h"

bool mx_check_parentheses(char *c) {
    unsigned int length = strlen(c);
    int stack[length];
    int up = -1;
    unsigned int i = 0;
    while (i < length) {
        mx_skip_quotes(c, &i, MX_GRAVE_ACCENT);
        if ((c[i] == '(' && !mx_isescape_char(c, i)) || (c[i] == '{' && !mx_isescape_char(c, i)) ||
            (c[i] == ')' && !mx_isescape_char(c, i)) || (c[i] == '}' && !mx_isescape_char(c, i))) {
            up++;
            stack[up] = c[i];
        }
        if ((stack[up] == ')' && (up - 1 >= 0 && stack[up - 1] == '('))
            || (stack[up] == '}' && (up - 1 >= 0 && stack[up - 1] == '{'))) {
            up = up - 2;
        }
        i++;
    }
    return up == -1;
}

