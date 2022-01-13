#include "libmx.h"

void mx_swapstr(char **s1, char **s2) {
    char *buf = NULL;
    buf = *s1;
    *s1 = *s2;
    *s2 = buf;
}

int mx_quicksort(char **arr, int left, int right) {
    if (arr == NULL) return -1;
    if (right <= left) return 0;
    int orig_l = left;
    int orig_r = right;
    int count = 0;
    int pivot = mx_strlen(arr[(right + left) / 2]);
    while (left <= right) {
        while (mx_strlen(arr[left]) < pivot) {
            left++;
        }
        while (mx_strlen(arr[right]) > pivot) {
            right--;
        }
        if (left <= right) {
            if (left != right
                && mx_strlen(arr[left])
                   != mx_strlen(arr[right])) {
                mx_swapstr(&arr[left], &arr[right]);
                count++;
            }
            left++;
            right--;
        }
    }
    count += mx_quicksort(arr, left, orig_r);
    count += mx_quicksort(arr, orig_l, left - 1);
    return count;
}
