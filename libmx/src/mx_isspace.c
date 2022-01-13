#include "libmx.h"

bool mx_isspace(const char c) {
    return c == ' '
           || c == '\t'
           || c == '\r'
           || c == '\n'
           || c == '\v'
           || c == '\f';
}
