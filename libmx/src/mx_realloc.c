#include "libmx.h"

void *mx_realloc(void *ptr, size_t size) {
    if (size == 0) {
        free(ptr);
        return NULL;
    }
    if (ptr == NULL) {
        return malloc(size);
    }
    if (size <= malloc_size(ptr)) {
        return ptr;
    }
    unsigned char *res = malloc(size);
    mx_memcpy(res, ptr, malloc_size(ptr));
    free(ptr);
    return res;
}
