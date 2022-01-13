#include "../inc/ush.h"

static void skip_quotes_single(char *cmnd, unsigned int *i) {
    if (cmnd[*i] == MX_S_QUOTES && !mx_is_escape_char(cmnd, *i)) {
        (*i)++;
        while (cmnd[*i]) {
            if (cmnd[*i] == MX_S_QUOTES) {
                break;
            }
            (*i)++;
        }
    }
}

void mx_skip_quotes(char *cmnd, unsigned int *i, char c) {
    if (c == MX_S_QUOTES) {
        skip_quotes_single(cmnd, i);
        return;
    }
    if (cmnd[*i] == c && !mx_is_escape_char(cmnd, *i)) {
        (*i)++;
        while (cmnd[*i]) {
            if (cmnd[*i] == c && !mx_is_escape_char(cmnd, *i)) {
                break;
            }
            (*i)++;
        }
    }
}

void mx_skip_quotes_s(char *cmnd, unsigned int *i) {
    if (cmnd[*i] == MX_S_QUOTES && !mx_is_escape_char(cmnd, *i)) {
        (*i)++;
        for (; cmnd[*i]; (*i)++) {
            if (cmnd[*i] == MX_S_QUOTES
                && !mx_is_escape_char(cmnd, *i)) {
                break;
            }
        }
    }
}

void mx_skip_grave(char *cmnd, unsigned int *i) {
    if (cmnd[*i] == MX_GRAVE_ACCENT && !mx_is_escape_char(cmnd, *i)) {
        (*i)++;
        for (; cmnd[*i]; (*i)++) {
            if (cmnd[*i] == MX_GRAVE_ACCENT
                && !mx_is_escape_char(cmnd, *i)) {
                break;
            }
        }
    }
}

void mx_skip_exp(char *cmnd, unsigned int *i) {
    int brawl_stars = 0;
    if (cmnd[*i] == '$' && !mx_is_escape_char(cmnd, *i)) {
        if (cmnd[*i + 1] == '(' && !mx_is_escape_char(cmnd, *i)) {
            *i += 2;
            brawl_stars++;
            for (; cmnd[*i]; (*i)++) {
                if (cmnd[*i] == '(' && !mx_is_escape_char(cmnd, *i)) {
                    brawl_stars++;
                }
                if (cmnd[*i] == ')' && !mx_is_escape_char(cmnd, *i)) {
                    brawl_stars--;
                }
                if (!brawl_stars) {
                    break;
                }
            }
        }
    }
}

void mx_skip_exps_quotes(char *arg, unsigned int *index, bool *quote) {
    if (arg[*index] == MX_D_QUOTES && !mx_is_escape_char(arg, *index)) {
        *quote = !*quote;
    }
    if (!*quote) {
        mx_skip_quotes(arg, index, MX_S_QUOTES);
    }
    mx_skip_quotes(arg, index, MX_GRAVE_ACCENT);
    mx_skip_exp(arg, index);
}

void mx_skip_quotes_if(bool *quote, char *arg, unsigned int *i) {
    if (arg[*i] == MX_D_QUOTES && !mx_is_escape_char(arg, *i)) {
        *quote = !*quote;
    }
    if (!*quote) {
        mx_skip_quotes(arg, i, MX_S_QUOTES);
    }
}
