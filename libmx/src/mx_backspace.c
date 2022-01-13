#include "../inc/libmx.h"

void mx_backspace(unsigned int times, int dir) {
    unsigned int i = 0;
    while (i < times) {
        dprintf(dir, "\b\x1b[0K");
        i++;
    }
}
