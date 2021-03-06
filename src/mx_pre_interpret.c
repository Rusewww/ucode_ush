#include "../inc/ush.h"

bool mx_check_quotes(char *c) {
    size_t length = strlen(c);
    bool s_quotes = false;
    bool d_quotes = false;
    unsigned int i = 0;
    while (i < length) {
        if (!s_quotes) {
            mx_skip_quotes(c, &i, MX_GRAVE_ACCENT);
            mx_skip_exp(c, &i);
        }
        if (c[i] == MX_D_QUOTES && !mx_is_escape_char(c, i) && !s_quotes) {
            d_quotes = !d_quotes;
        }
        if (c[i] == MX_S_QUOTES && !d_quotes) {
            if (!s_quotes) {
                if (!mx_is_escape_char(c, i)) {
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

bool mx_check_parentheses(char *c) {
    unsigned int length = strlen(c);
    int stack[length];
    int up = -1;
    unsigned int i = 0;
    while (i < length) {
        mx_skip_quotes(c, &i, MX_GRAVE_ACCENT);
        if ((c[i] == '(' && !mx_is_escape_char(c, i)) || (c[i] == '{' && !mx_is_escape_char(c, i)) ||
            (c[i] == ')' && !mx_is_escape_char(c, i)) || (c[i] == '}' && !mx_is_escape_char(c, i))) {
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
                && !mx_is_escape_char(c, i)) {
                return false;
            }
            j++;
        }
        i++;
    }
    return true;
}

bool mx_check_substitutions(char *c) {
    bool g_quotes = false;
    size_t length = strlen(c);
    unsigned int i = 0;
    while (i < length) {
        mx_skip_quotes(c, &i, MX_S_QUOTES);
        mx_skip_exp(c, &i);
        if (c[i] == MX_GRAVE_ACCENT && !mx_is_escape_char(c, i)) {
            g_quotes = !g_quotes;
        }
        i++;
    }
    return !g_quotes;
}

int mx_pre_interpret(char *c) {
    int res = 0;
    if (!(res = mx_check_quotes(c))) {
        fprintf(stderr, "%s: missing terminating character\n", MX_SHELL_NAME);
    } else if (!(res = mx_check_parentheses(c))) {
        fprintf(stderr, "%s: missing brace character\n", MX_SHELL_NAME);
    } else if (!(res = mx_check_substitutions(c))) {
        fprintf(stderr, "%s: missing subsitution character\n", MX_SHELL_NAME);
    } else if (!(res = mx_check_char(c))) {
        fprintf(stderr, "%s: command contain forbidden character\n", MX_SHELL_NAME);
    }
    return !res;
}
