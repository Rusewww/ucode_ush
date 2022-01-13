#include "libmx.h"

int mx_atoi(const char *str) {
    long out = 0;
    while (*str == ' ') {
        str++;
    }
    int minus = str[0] == '-' ? -1 : 1;
    if (str[0] == '+') str++;
    for (int i = minus < 0; mx_isdigit(str[i]); i++) {
        if (minus > 0) {
            if (out + (str[i] - '0') < 0) {
                return -1;
            }
            out += str[i] - '0';
        } else {
            if (out - (str[i] - '0') > 0) {
                return 0;
            }
            out -= str[i] - '0';
        }
        if (mx_isdigit(str[i + 1])) {
            out *= 10;
        }
    }
    return (int) out;
}
