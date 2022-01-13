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

bool mx_check_char(char *c) {
    unsigned int i = 0;
    while (i < strlen(c)) {
        mx_skip_quotes(c, &i, MX_D_QUOTES);
        mx_skip_quotes(c, &i, MX_S_QUOTES);
        unsigned int j = 0;
        while (j < strlen(MX_FORBIDDEN_CHARS)) {
            if (c[i] == MX_FORBIDDEN_CHARS[j]
                && !mx_isescape_char(c, i)) {
                return false;
            }
            j++;
        }
        i++;
    }
    return true;
}

