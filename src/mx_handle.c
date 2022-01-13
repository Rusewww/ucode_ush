#include "../inc/ush.h"

static void c_backspace(t_prompt *prompt, unsigned int d) {
    if (!prompt->cursor_index || !prompt->index) {
        return;
    }
    memmove(prompt->command + d, prompt->command + d + 1, strlen(prompt->command) - d);
    prompt->index--;
    prompt->cursor_index--;
}

static void c_delete(t_prompt *prompt, unsigned int d) {
    if (!prompt->index || d == prompt->index) {
        return;
    }
    memmove(prompt->command + d, prompt->command + d + 1, strlen(prompt->command) - d);
    prompt->index--;
}

static void c_insert(t_prompt *prompt, unsigned int c) {
    memmove(prompt->command + c + 1, prompt->command + c, strlen(prompt->command + c));
    prompt->command[prompt->cursor_index] = prompt->buff[0];
    prompt->index++;
    prompt->cursor_index++;
}

static bool is_valid(t_prompt *prompt) {
    if (!mx_get_substr_index(prompt->tmp_history->data, prompt->tmp_command)
        && strcmp(prompt->tmp_history->data, prompt->command)
        && strcmp(prompt->tmp_history->data, prompt->tmp_command)) {
        return true;
    }
    return false;
}

static void get_down(t_prompt *prompt) {
    if (prompt->tmp_history) {
        if (prompt->tmp_history->prev) {
            prompt->tmp_history = prompt->tmp_history->prev;
        }
    }
    while (prompt->tmp_history) {
        if (is_valid(prompt)) {
            mx_rcmd(prompt->command, prompt->tmp_history->data, sizeof(prompt->command), &prompt->index);
            break;
        }
        if (!prompt->tmp_history->prev) {
            prompt->end = true;
            break;
        }
        prompt->tmp_history = prompt->tmp_history->prev;
    }
    if (prompt->end) {
        mx_rcmd(prompt->command, prompt->tmp_command, sizeof(prompt->command), &prompt->index);
    }
}

static void get_up(t_prompt *prompt) {
    if (prompt->tmp_history
        && !strcmp(prompt->command, prompt->tmp_history->data)) {
        if (prompt->tmp_history->next)
            prompt->tmp_history = prompt->tmp_history->next;
    }
    while (prompt->tmp_history) {
        if (is_valid(prompt)) {
            mx_rcmd(prompt->command, prompt->tmp_history->data, sizeof(prompt->command), &prompt->index);
            prompt->end = false;
            break;
        }
        if (!prompt->tmp_history->next) {
            break;
        }
        prompt->tmp_history = prompt->tmp_history->next;
    }
}

static void set_cursor(t_prompt *prompt) {
    prompt->index = strlen(prompt->command);
    prompt->cursor_index = strlen(prompt->command);
}

void mx_handle_cmnd(char *cmnd, int *code) {
    char **cmnds = mx_parse_cmnd(cmnd, code);
    char **argum = NULL;
    t_map **map = mx_get_lenv();

    if (*code || !cmnds) {
        mx_put_map(map, "?", mx_itoa(*code));
        return;
    }
    unsigned int i = 0;
    while (cmnds[i]) {
        if (!(argum = mx_interpretate(cmnds[i], code))) {
            mx_put_map(map, "?", mx_itoa(*code));
            continue;
        }
        *code = mx_exec_command(argum, 1);
        mx_put_map(map, "?", mx_itoa(*code));
        mx_del_strarr(&argum);
        i++;
    }
    mx_del_strarr(&cmnds);
}

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

bool mx_handle_his(t_prompt *prompt) {
    if (!strcmp(prompt->buff, MX_DOWN_ARROW)) {
        get_down(prompt);
        set_cursor(prompt);
        return true;
    }
    if (!strcmp(prompt->buff, MX_UP_ARROW)) {
        get_up(prompt);
        set_cursor(prompt);
        return true;
    }
    return false;
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
        c_backspace(prompt, prompt->cursor_index - 1);
        return;
    }
    if (!strcmp(prompt->buff, MX_DELETE_KEY)
        || !strcmp(prompt->buff, "\x04")) {
        c_delete(prompt, prompt->cursor_index);
        return;
    }
    if (!isprint(prompt->buff[0]) || strlen(prompt->buff) != 1) {
        return;
    }
    c_insert(prompt, prompt->cursor_index);
}

t_list *mx_handle_subs(t_list *arg) {
    t_list *res = NULL;
    for (; arg; arg = arg->next) {
        mx_push_back(&res, strdup(arg->data));
    }
    return res;
}

