#include "libmx.h"

unsigned long mx_hex_to_nbr(const char *hex) {
    if (hex == NULL) return 0;
    unsigned long res = 0;
    while (*hex != 0) {
        if ('0' <= *hex && *hex <= '9') res += *hex - '0';
        if ('a' <= *hex && *hex <= 'z') res += *hex - 'a' + 10;
        if ('A' <= *hex && *hex <= 'Z') res += *hex - 'A' + 10;
        if (*(hex + 1) != 0) res *= 16;
        hex++;
    }
    return res;
}
