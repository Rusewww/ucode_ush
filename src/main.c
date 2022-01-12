#include "ush.h"

static void main_cycle(void) {
    int res = 0;
    t_prompt *prompt = malloc(sizeof(t_prompt));
    extern char **environ;
    int fd = isatty(1) ? 1 : 2;
    if (!isatty(1) && !isatty(2)) {
        fd = open("/dev/null", O_RDONLY);
    }
    while (res != -1) {
        mx_get_input(prompt, fd, &res);
        dprintf(fd, "\r\n");
        mx_handle_command(prompt->command, &res);
    }
    if (fd != 1 && fd != 2) {
        close(fd);
    }
    mx_d_del_list(&prompt->history_head);
    free(prompt);
}

int main(int argc, char **argv) {
    int exit_code = 0;
    if (argc > 1) {
        fprintf(stderr, "%s: illegal option -- %s\n", MX_SHELL_NAME, argv[1]);
        fprintf(stderr, "usage: %s ./ush\n", MX_SHELL_NAME);
        return 1;
    }
    mx_init();
    if (check_stdin(&exit_code)) {
        mx_deinit();
        exit(exit_code);
    }
    main_cycle();
    mx_deinit();
    return 0;
}