#include "../inc/ush.h"

char **mx_exec_copy_argv(char **argv) {
    size_t size = mx_arr_size(argv);
    char **clone = malloc(sizeof(char *) * (size + 1));
    clone[size] = NULL;
    unsigned int i = 0;
    while (i < size) {
        clone[i] = strdup(argv[i]);
        i++;
    }
    return clone;
}
