#include "ush.h"

static void clear_data(char **s2, char ***a_s1, char ***a_s2) {
    mx_del_strarr(a_s1);
    mx_del_strarr(a_s2);
    mx_strdel(s2);
}

void mx_replace_sub_escapes(char **commands) {
    for (unsigned int i = 0; commands[i]; i++) {
        commands[i] = mx_replace_escape(commands[i], "\\`", MX_GRAVE_ACCENT, true);
        commands[i] = mx_replace_escape(commands[i], "\\\\", '\\', true);
    }
}

bool mx_check_user(char *username) {
    DIR *dir = opendir("/Users/");
    struct dirent *entry;
    while ((entry = readdir(dir))) {
        if (!strcmp(entry->d_name, username)) {
            closedir(dir);
            return true;
        }
    }
    closedir(dir);
    return false;
}

char *mx_check_user_file(char *tmp_name) {
    char *pasfile = mx_file_to_str("/etc/passwd");
    char **lines = mx_strsplit(pasfile, '\n');
    char **data = NULL;
    char *res = NULL;
    unsigned int i = 0;
    while (lines[i]) {
        if (lines[i][0] != '#') {
            data = mx_strsplit(lines[i], ':');
            if (!strcmp(data[0], tmp_name)) {
                res = strdup(data[5]);
                clear_data(&pasfile, &data, &lines);
                return res;
            }
            mx_del_strarr(&data);
        }
        i++;
    }
    mx_del_strarr(&lines);
    mx_strdel(&pasfile);
    return NULL;
}

char *mx_get_invalid_sub(char **arg, char **result, char **sub) {
    mx_strdel(arg);
    mx_strdel(result);
    mx_strdel(sub);
    return NULL;
}
