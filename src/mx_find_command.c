#include "ush.h"

static bool check_dir(char *path, char *file) {
    DIR *dir = opendir(path);
    struct dirent *entry;
    bool rval = 0;
    if (dir) {
        while ((entry = readdir(dir)) != NULL) {
            if (!strcmp(entry->d_name, file) && strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
                rval = 1;
            }
        }
        closedir(dir);
    }
    return rval;
}

static void check_in_abspath(char *command, char **name, bool *rval) {
    char *d_name = dirname(command);
    char *b_name = basename(command);

    if (check_dir(d_name, b_name)) {
        *rval = 1;
        *name = mx_strdup(command);
    }
}

bool mx_find_command(char *path, char *command, char **filename) {
    char **paths = mx_strsplit(path, ':');
    bool retval = 0;
    char fname[PATH_MAX];

    if (paths) {
        for (int i = 0; paths[i]; i++)
            if (check_dir(paths[i], command)) {
                retval = 1;
                sprintf(fname, "%s/%s", paths[i], command);
                if (*filename)
                    mx_strdel(filename);
                *filename = strdup(fname);
            }
        mx_del_strarr(&paths);
    }
    if (!retval)
        check_in_abspath(command, filename, &retval);
    return retval;
}
