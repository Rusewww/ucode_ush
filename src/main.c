#include "../inc/ush.h"

int main(int argc, char **argv) {
    int exit_code = 0;
    if (argc > 1) {
        fprintf(stderr, "%s: illegal option -- %s\n", MX_SHELL_NAME, argv[1]);
        fprintf(stderr, "usage: %s ./ush\n", MX_SHELL_NAME);
        return 1;
    }
    mx_init();
    if (check_stdin(&exit_code)) {
        mx_de_init();
        exit(exit_code);
    }
    int res = 0;
    t_prompt *prompt = malloc(sizeof(t_prompt));
    extern char **environ;
    int fd = isatty(1) ? 1 : 2;
    if (!isatty(1) && !isatty(2)) {
        fd = open("/dev/null", O_RDONLY);
    }
    while (res != -1) {
        mx_get_in(prompt, fd, &res);
        dprintf(fd, "\r\n");
        mx_handle_cmnd(prompt->command, &res);
    }
    if (fd != 1 && fd != 2) {
        close(fd);
    }
    mx_d_del_list(&prompt->history_head);
    free(prompt);
    mx_de_init();
    return 0;
}
