#include "ush.h"

static bool check_dir(char *path, char *file) {
    DIR *dir = opendir(path);
    struct dirent *entry;
    bool rval = 0;

    if (dir) {
        while ((entry = readdir(dir)) != NULL)
            if (!strcmp(entry->d_name, file) && strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
                rval = 1;
            }
        closedir(dir);
    }
    return rval;
}

static void check_in_abspath(char *cmnd, char **name, bool *rval) {
    char *d_name = dirname(cmnd);
    char *b_name = basename(cmnd);

    if (check_dir(d_name, b_name)) {
        *rval = 1;
        *name = mx_strdup(cmnd);
    }
}

bool mx_find_command(char *path, char *cmnd, char **filen) {
    char **path = mx_strsplit(path, ':');
    bool rval = 0;
    char fname[PATH_MAX];

    if (path) {
        int i = 0;
        while (path[i]) {
            if (check_dir(path[i], cmnd)) {
                rval = 1;
                sprintf(fname, "%s/%s", path[i], cmnd);
                if (*filen) {
                    mx_strdel(filen);
                }
                *filen = strdup(fname);
            }
            mx_del_strarr(&path);
            i++;
        }
    }
    if (!rval) {
        check_in_abspath(cmnd, filen, &rval);
    }
    return rval;
}
