#include "libmx.h"

bool mx_isalpha(const char c) {
    return ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z');
}
