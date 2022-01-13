#include "../inc/ush.h"

void mx_skip_exps_quotes(char *arg, unsigned int *index, bool *quote) {
    if (arg[*index] == MX_D_QUOTES && !mx_isescape_char(arg, *index)) {
        *quote = !*quote;
    }
    if (!*quote) {
        mx_skip_quotes(arg, index, MX_S_QUOTES);
    }
    mx_skip_quotes(arg, index, MX_GRAVE_ACCENT);
    mx_skip_exp(arg, index);
}

bool mx_replace_env_var(char *res, char *env, unsigned int *index, unsigned int length) {
    if (!env) {
        return false;
    }
    strcat(res, env);
    index[0] += length;
    index[1] = index[0] + 1;
    mx_strdel(&env);
    return true;
}

bool mx_check_trimmed_str(char *sub_trim, char *sub) {
    bool rval = strlen(sub_trim) == 0;
    mx_strdel(&sub_trim);
    if (rval) {
        mx_strdel(&sub);
    }
    return rval;
}

void mx_inc_val_var(unsigned int *length, unsigned int add, char *var) {
    *length += add;
    mx_strdel(&var);
}

void mx_skip_quotes_if(bool *quote, char *arg, unsigned int *i) {
    if (arg[*i] == MX_D_QUOTES && !mx_isescape_char(arg, *i)) {
        *quote = !*quote;
    }
    if (!*quote) {
        mx_skip_quotes(arg, i, MX_S_QUOTES);
    }
}
