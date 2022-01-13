#include "../inc/libmx.h"

bool mx_is_escape_char(char *cmnd, int i) {
    if (i > 0 && cmnd[i - 1] == '\\' && !mx_is_escape_char(cmnd, i - 1)) {
        return true;
    }
    if (i == 1 && cmnd[i - 1] == '\\') {
        return true;
    }
    return false;
}
