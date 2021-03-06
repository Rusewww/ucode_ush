#include "../inc/ush.h"

void mx_disable_canon(void) {
    tcsetattr(STDIN_FILENO, TCSADRAIN, mx_get_tty());
}

void mx_enable_canon(void) {
    struct termios tty;
    tcgetattr(STDIN_FILENO, mx_get_tty());
    tty = *mx_get_tty();
    tty.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    tcsetattr(STDIN_FILENO, TCSADRAIN, &tty);
}
