#include "libmx.h"

char *mx_file_to_str(const char *filename) {
    if (filename == NULL) {
        return NULL;
    }
    int fileIn = open(filename, O_RDONLY);
    if (fileIn < 0) {
        close(fileIn);
        return NULL;
    }

    char buffer;
    int length = 0;
    while (read(fileIn, &buffer, 1)) {
        length++;
    }
    if (length == 0) {
        mx_printerr("error: file ");
        mx_printerr(filename);
        mx_printerr(" is empty\n");
        exit(0);
    }

    close(fileIn);

    fileIn = open(filename, O_RDONLY);

    char *arr = mx_strnew(length);

    for (int i = 0; read(fileIn, &buffer, 1); i++) {
        arr[i] = buffer;
    }
    close(fileIn);
    return arr;
}
