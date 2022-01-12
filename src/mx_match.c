#include "ush.h"

bool mx_match(char *src, char *regex) {
    regex_t reg;
    int res;
    regcomp(&reg, regex, REG_EXTENDED);
    res = regexec(&reg, src, 0, NULL, 0);
    regfree(&reg);
    return res == 0;
}
