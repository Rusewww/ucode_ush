#include "libmx.h"

char *mx_itoa(int number) {
    int length = 0;

    int number_clone = number;

    while (number_clone) {
        number_clone /= 10;
        length++;
    }
    char *out = NULL;
    out = mx_strnew(length);

    if (number == -2147483648) {
        return mx_strcpy(out, "-2147483648");
    }
    if (number == 0) {
        return mx_strcpy(out, "0");
    }
    int length_second = 0;
    int minus = 0;
    if (number < 0) {
        minus = 1;
        number *= -1;
    }
    while (number) {
        out[length_second] = number % 10 + 48;
        number = number / 10;
        length_second++;
    }
    if (minus != 0) {
        out[length_second] = ('-');
        length_second++;
    }
    mx_str_reverse(out);
    return out;
}
