#include "../inc/ush.h"

static void copy(char **copy, char **environ) {
    for (int i = 0; environ[i]; i++) {
        copy[i] = strdup(environ[i]);
        if (environ[i + 1] == NULL)
            copy[i + 1] = NULL;
    }
}

static char **get_sort(void) {
    char **sorted_environ = NULL;
    extern char **environ;
    int len = 0;

    if (*environ == NULL) {
        mx_putenv("SHLVL=0");
    }
    while (environ[len]) {
        len++;
    }
    sorted_environ = malloc((len + 1) * sizeof(char *));
    copy(sorted_environ, environ);
    mx_bubble_sort(sorted_environ, len);
    return sorted_environ;
}


static bool check_pwd(char *pwd) {
    char *str = NULL;
    char *curr = getwd(NULL);

    if (pwd) {
        str = realpath(pwd, NULL);
        if (str && curr && !strcmp(curr, str)) {
            mx_strdel(&str);
            mx_strdel(&curr);
            return true;
        }
    }
    if (curr) {
        mx_strdel(&curr);
    }

    return false;
}

static void export_pwd(char *name, char *val) {
    char var[NAME_MAX];
    char *arg[2] = {NULL, NULL};
    sprintf(var, "%s=%s", name, val);
    arg[0] = strdup(var);
    mx_export(arg, 1);
    mx_strdel(&arg[0]);
}

static void init_pwd(t_map **map, char *path) {
    mx_put_map(map, strdup("OLDPWD"), strdup(""));
    export_pwd("OLDPWD", "");
    if (check_pwd(getenv("PWD"))) {
        mx_put_map(map, strdup("PWD"), strdup(getenv("PWD")));
    } else {
        mx_put_map(map, strdup("PWD"), strdup(path));
        export_pwd("PWD", path);
    }
}

void mx_init_map_v(void) {
    t_map **map = mx_get_lenv();
    char path[PATH_MAX];
    *map = mx_create_map(40);
    getcwd(path, sizeof(path));
    init_pwd(map, path);
    mx_put_map(map, strdup("?"), strdup("0"));
    mx_put_map(map, strdup("#"), strdup("0"));
    mx_put_map(map, strdup("0"), strdup(MX_SHELL_NAME));
    mx_put_map(map, strdup("_"), strdup(path));
    mx_put_map(map, strdup("$"), mx_itoa(getpid()));
    mx_put_map(map, strdup("*"), strdup(""));
    mx_put_map(map, strdup("@"), strdup(""));
}


void mx_init_signals(void) {
    sigset_t mask;
    sigfillset(&mask);
    sigprocmask(SIG_SETMASK, &mask, NULL);
}

void mx_init_var_ls(void) {
    t_list **shell_list = mx_get_var_list(SHELL);
    t_list **exp_list = mx_get_var_list(EXP);
    char **sorted_environ = get_sort();

    for (int i = 0; sorted_environ[i]; i++) {
        mx_push_back(shell_list, strdup(sorted_environ[i]));
        mx_push_back(exp_list, strdup(sorted_environ[i]));
    }
    mx_del_strarr(&sorted_environ);
}

void mx_init(void) {
    mx_putenv("MX_PROMPT=u$h> ");
    mx_init_var_ls();
    mx_init_map_v();
    mx_increase_sh_lvl();
    if (getenv("HOME") == NULL) {
        struct passwd *pw = getpwuid(getuid());
        char *home = mx_strjoin("HOME=", pw->pw_dir);
        mx_putenv(home);
        mx_var_list_insert(SHELL, home);
        mx_strdel(&home);
    }
    if (getenv("PATH") == NULL) {
        mx_var_list_insert(SHELL, DEFAULT_PATH);
    }
    mx_init_signals();
    tcgetattr(STDIN_FILENO, mx_get_tty());
    setvbuf(stdout, NULL, _IONBF, 0);
    mx_enable_canon();
}

void mx_de_init(void) {
    mx_disable_canon();
    mx_kill_process();
    t_map **map = mx_get_lenv();
    mx_del_map(map);
}
