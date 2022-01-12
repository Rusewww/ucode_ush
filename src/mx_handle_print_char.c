#include "ush.h"

static void backspace_char(t_prompt *prompt, unsigned int d) {
    if (!prompt->cursor_index || !prompt->index) {
        return;
    }
    memmove(prompt->command + d, prompt->command + d + 1, strlen(prompt->command) - d);
    prompt->index--;
    prompt->cursor_index--;
}

static void delete_char(t_prompt *prompt, unsigned int d) {
    if (!prompt->index || d == prompt->index) {
        return;
    }
    memmove(prompt->command + d, prompt->command + d + 1, strlen(prompt->command) - d);
    prompt->index--;
}

static void insert_char(t_prompt *prompt, unsigned int c) {
    memmove(prompt->command + c + 1, prompt->command + c, strlen(prompt->command + c));
    prompt->command[prompt->cursor_index] = prompt->buff[0];
    prompt->index++;
    prompt->cursor_index++;
}

void mx_handle_print_char(t_prompt *prompt) {
    prompt->tmp_history = prompt->history_head;
    if (prompt->buff[0] == 0x15 && strlen(prompt->buff) == 1) {
        memset(prompt->command, '\0', sizeof(prompt->command));
        prompt->index = 0;
        prompt->cursor_index = 0;
        return;
    }
    if (prompt->buff[0] == 0x7f && strlen(prompt->buff) == 1) {
        backspace_char(prompt, prompt->cursor_index - 1);
        return;
    }
    if (!strcmp(prompt->buff, MX_DELETE_KEY)
        || !strcmp(prompt->buff, "\x04")) {
        delete_char(prompt, prompt->cursor_index);
        return;
    }
    if (!isprint(prompt->buff[0]) || strlen(prompt->buff) != 1) {
        return;
    }
    insert_char(prompt, prompt->cursor_index);
}
