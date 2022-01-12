#include "ush.h"

void mx_env_fill(char **src) {
    int i = 0;
    while (src && src[i]) {
        mx_putenv(src[i]);
        i++;
    }
}
