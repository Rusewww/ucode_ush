#include "../inc/ush.h"

static void get_quoted_sub(int *o_sub, int *c_sub, unsigned int *i, char *arg) {
    for (; arg[*i]; *i += 1) {
        mx_skip_quotes(arg, i, MX_S_QUOTES);
        if (arg[*i] == MX_GRAVE_ACCENT
            && !mx_is_escape_char(arg, *i) && *o_sub == -1) {
            *o_sub = *i;
            *i += 1;
            continue;
        }
        if (arg[*i] == MX_GRAVE_ACCENT && !mx_is_escape_char(arg, *i)) {
            *c_sub = *i;
            *i += 1;
            break;
        }
    }
}

static void inc_subs(int *sub, unsigned int *i, int add) {
    *sub = *i;
    *i += add;
}

static void get_parameter_sub(int *o_sub, int *c_sub, unsigned int *i, char *arg) {
    unsigned int brace = 0;
    for (; arg[*i]; *i += 1) {
        mx_skip_quotes(arg, i, MX_S_QUOTES);
        if (arg[*i] == '$' && arg[*i + 1] == '('
            && !mx_is_escape_char(arg, *i) && *o_sub == -1) {
            brace++;
            inc_subs(o_sub, i, 2);
            continue;
        }
        if (arg[*i] == '(' && !mx_is_escape_char(arg, *i))
            brace++;
        if (arg[*i] == ')' && !mx_is_escape_char(arg, *i)) {
            if (brace-- && !brace) {
                inc_subs(c_sub, i, 1);
                break;
            }
        }
    }
}

static char *get_substitution(int *o_sub, int *c_sub, unsigned int *i, char *arg) {
    if (arg[*i] == MX_GRAVE_ACCENT && !mx_is_escape_char(arg, *i)) {
        get_quoted_sub(o_sub, c_sub, i, arg);
    } else if (arg[*i] == '$' && !mx_isescape_char(arg, *i)) {
        get_parameter_sub(o_sub, c_sub, i, arg);
    }
    if (*o_sub != -1 && *c_sub != -1) {
        return strndup(arg + *o_sub, *c_sub - *o_sub + 1);
    }
    return NULL;
}

char *mx_replace_substitution(char *arg, int *code) {
    int i_s[3] = {-1, -1, 0};
    bool quote = false;
    char *sub = NULL;
    char *res = mx_strnew(ARG_MAX);
    unsigned int i = 0;
    while (i < strlen(arg)) {
        mx_skip_quotes_if(&quote, arg, &i);
        if ((sub = get_substitution(&i_s[0], &i_s[1], &i, arg))) {
            strncat(res, arg + i_s[2], i_s[0] - i_s[2]);
            if (!mx_get_sub(res, sub, code))
                return mx_get_invalid_sub(&arg, &res, &sub);
            i = i_s[1];
            i_s[2] = i + 1;
        } else if (i_s[0] != -1 && i_s[1] == -1 && !sub) {
            break;
        }
        i++;
        i_s[0] = -1;
        i_s[1] = -1;
    }
    strcat(res, arg + i_s[2]);
    mx_strdel(&arg);
    return res;
}
