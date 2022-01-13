#include "libmx.h"

void mx_printint(int n) {
    char *res = mx_itoa(n);
    mx_printstr(res);
    mx_strdel(&res);
}
