#include "../inc/ush.h"

void mx_init(void) {
    mx_putenv("MX_PROMPT=u$h> ");
    mx_init_var_lists();
    mx_init_map_vars();
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

bool check_stdin(int *exit_code) {
    char *buff = NULL;
    size_t linecap = 0;
    ssize_t linelen = 0;

    if (isatty(STDIN_FILENO))
        return false;
    buff = mx_strnew(ARG_MAX + 1);
    while ((linelen = getline(&buff, &linecap, stdin)) > 0) {
        buff[linelen] = '\0';
        if (buff[linelen - 1] == '\n') {
            buff[linelen - 1] = '\0';
        }
        mx_handle_cmnd(buff, exit_code);
    }
    mx_strdel(&buff);
    return true;
}

void mx_deinit(void) {
    mx_disable_canon();
    mx_kill_process();
    t_map **map = mx_get_lenv();
    mx_del_map(map);
}
