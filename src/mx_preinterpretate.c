#include "ush.h"

int mx_preinterpretate(char *command) {
    int res = 0;

    if (!(res = mx_check_quotes(command))) {
        fprintf(stderr, "%s: missing terminating character\n",
                MX_SHELL_NAME);
    } else if (!(res = mx_check_brackets(command))) {
        fprintf(stderr, "%s: missing brace character\n",
                MX_SHELL_NAME);
    } else if (!(res = mx_check_substitutions(command))) {
        fprintf(stderr, "%s: missing subsitution character\n",
                MX_SHELL_NAME);
    } else if (!(res = mx_check_chars(command))) {
        fprintf(stderr, "%s: command contain forbidden character\n",
                MX_SHELL_NAME);
    }
    return !res;
}
