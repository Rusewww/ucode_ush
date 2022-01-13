char *mx_strcpy(char *dst, const char *src) {
    dst[0] = src[0];
    for (int i = 1; src[i - 1] != '\0'; i++) {
        dst[i] = src[i];
    }
    return dst;
}
