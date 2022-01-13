int mx_strcmp(const char *s1, const char *s2);

int mx_binary_search(char **arr, int size, const char *s, int *count) {
    int left = 0;
    int right = size - 1;
    int middle = 0;
    *count = 0;
    while (right >= left) {
        (*count)++;
        middle = (right + left) / 2;
        if (mx_strcmp(s, arr[middle]) == 0) return middle;
        if (mx_strcmp(s, arr[middle]) > 0) left = middle + 1;
        else right = middle - 1;
    }
    *count = 0;
    return -1;
}
