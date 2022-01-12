#include "ush.h"

static void copy_environ(char **copy, char **environ) {
    int i = 0;
    while (environ[i]) {
        copy[i] = strdup(environ[i]);
        if (environ[i + 1] == NULL) {
            copy[i + 1] = NULL;
        }
        i += 1;
    }
}

char **mx_env_copy(void) {
    extern char **environ;
    char **env_clone = NULL;
    int length;

    for (length = 0; environ[length]; length++);

    if (environ[0]) {
        env_clone = malloc((length + 1) * sizeof(char *));
        copy_environ(env_clone, environ);
    }
    return env_clone;
}
