#include "ush.h"

bool mx_check_brackets(char *char) {
    unsigned int len = strlen(char);
    int stack[len];
    int top = -1;

    for (unsigned int i = 0; i < len; i++) {
        mx_skip_quotes(char, &i, MX_GRAVE_ACCENT);
        if ((char[i] == '(' && !mx_isescape_char(char, i))
            || (char[i] == '{' && !mx_isescape_char(char, i))
            || (char[i] == ')' && !mx_isescape_char(char, i))
            || (char[i] == '}' && !mx_isescape_char(char, i))) {
            top++;
            stack[top] = char[i];
        }
        if ((stack[top] == ')' && (top - 1 >= 0 && stack[top - 1] == '('))
            || (stack[top] == '}' && (top - 1 >= 0
                                      && stack[top - 1] == '{'))) {
            top = top - 2;
        }
    }
    return top == -1;
}
