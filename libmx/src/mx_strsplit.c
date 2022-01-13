#include "libmx.h"

char **mx_strsplit(const char *s, char c) {
    if (s == NULL) {
        return NULL;
    }
    while (*s == c) {
        s++;
    }
    int count = 0;
    for (int i = 0; s[i] != '\0'; i++) {
        if (s[i] != c
            && (s[i + 1] == c
                || s[i + 1] == '\0')) {
            count++;
        }
    }
    char **array_string = (char **) malloc((count + 1) * sizeof(char **));
    array_string[count] = NULL;
    int length = 0;
    int j = 0;
    for (int i = 0; s[i] != '\0'; i++) {
        if (s[i] != c) {
            length = mx_get_char_index(&s[i], c);
            if (length == -1) {
                length = mx_strlen(&s[i]);
            }
            array_string[j] = mx_strndup(&s[i], length);
            j++;
            i += length;
            if (s[i] == '\0') {
                break;
            }
        }
    }
    return array_string;
}
