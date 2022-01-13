#include "../inc/ush.h"

void mx_handle_cursor(t_prompt *prompt) {
    unsigned int left = prompt->index - prompt->cursor_index;
    if (!strcmp(prompt->buff, MX_RIGHT_ARROW)) {
        if (prompt->cursor_index < prompt->index) {
            prompt->cursor_index++;
        }
    }
    if (!strcmp(prompt->buff, MX_LEFT_ARROW)) {
        if (prompt->cursor_index) {
            prompt->cursor_index--;
        }
    }
    if (!strcmp(prompt->buff, MX_END_KEY)) {
        prompt->cursor_index = prompt->index;
    }
    if (!strcmp(prompt->buff, MX_HOME_KEY)) {
        prompt->cursor_index = 0;
    }
    unsigned int i = 0;
    while (i < left) {
        printf("\b");
        i++;
    }
}
