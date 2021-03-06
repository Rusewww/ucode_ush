#include "../inc/ush.h"

static bool handle(t_prompt *prompt, int fd, int *code) {
    if (prompt->buff[0] == '\x03' && strlen(prompt->buff) == 1) {
        memset(prompt->command, '\0', sizeof(prompt->command));
        memset(prompt->tmp_command, '\0', sizeof(prompt->tmp_command));
        prompt->index = 0;
        prompt->cursor_index = 0;
        prompt->end = false;
        *code = 130;
    }
    if (prompt->buff[0] == '\x04' && !prompt->index && strlen(prompt->buff) == 1) {
        *code = -1;
    }
    if (mx_match(prompt->buff, MX_NEW_LINE_CHARS)) {
        return false;
    }
    if (prompt->buff[0] == '\x04' && !prompt->index) {
        return false;
    }
    mx_backspace(prompt->index + strlen(mx_str_prompt()), fd);
    dprintf(fd, "%s%s", mx_str_prompt(), prompt->command);
    mx_backspace(prompt->index + strlen(mx_str_prompt()), fd);
    if (!mx_handle_his(prompt)) {
        mx_handle_print_char(prompt);
        strcpy(prompt->tmp_command, prompt->command);
    }
    return true;
}

void mx_get_in(t_prompt *prompt, int fd, int *code) {
    memset(prompt->command, '\0', sizeof(prompt->command));
    memset(prompt->tmp_command, '\0', sizeof(prompt->tmp_command));
    prompt->index = 0;
    prompt->cursor_index = 0;
    prompt->end = false;
    dprintf(fd, "%s%s", mx_str_prompt(), prompt->command);
    while (read(STDIN_FILENO, prompt->buff, 4) && prompt->index < ARG_MAX && handle(prompt, fd, code)) {
        dprintf(fd, "%s%s", mx_str_prompt(), prompt->command);
        mx_handle_cursor(prompt);
        memset(prompt->buff, '\0', sizeof(prompt->buff));
    }
    mx_update_history(prompt);
}


