#include "../inc/ush.h"

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

