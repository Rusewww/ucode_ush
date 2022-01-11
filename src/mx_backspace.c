#include "ush.h"

void mx_backspace(unsigned int times, int fd) {
    unsigned int i = 0;
    while (i < times) {
        dprintf(fd, "\b\x1b[0K");
        i++;
    }
}
