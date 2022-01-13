#include "libmx.h"

char *mx_nbr_to_hex(unsigned long nbr) {
    if (nbr == 0) {
        return mx_strdup("0");
    }
    unsigned long copy = nbr;
    int count = 0;
    char *hex = "0123456789abcdef";
    while (copy != 0) {
        copy /= 16;
        count++;
    }
    char *res = mx_strnew(count);
    while (nbr != 0) {
        res[count - 1] = hex[nbr % 16];
        nbr /= 16;
        count--;
    }
    return res;
}
