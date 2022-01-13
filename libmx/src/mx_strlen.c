int mx_strlen(const char *s) {
    const char *begin = s;
    while (*s != '\0') s++;
    return s - begin;
}
