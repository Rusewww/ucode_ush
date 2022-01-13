#include "../inc/ush.h"

bool mx_remove_subchar(char *subs) {
    unsigned int length = strlen(subs);
    if (subs[0] == '`' && subs[length - 1] == '`') {
        subs[length - 1] = '\0';
        memmove(subs, subs + 1, length - 1);
        return true;
    }
    if (subs[0] == '$' && subs[1] == '('
        && subs[length - 1] == ')') {
        subs[length - 1] = '\0';
        memmove(subs, subs + 2, length - 2);
        return true;
    }
    return false;
}
