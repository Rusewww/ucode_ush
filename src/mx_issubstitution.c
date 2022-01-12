#include "ush.h"

bool mx_issubstitution(char *arg) {
    unsigned int length = strlen(arg);
    bool res = false;

    if (arg[0] == '`' && arg[length - 1] == '`') {
        res = true;
    }
    if (arg[0] == '$' && arg[1] == '(' && arg[length - 1] == ')') {
        res = true;

        return res;
    }
