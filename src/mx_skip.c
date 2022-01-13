#include "../inc/ush.h"

static void skip_quotes_single(char *cmnd, unsigned int *i) {
    if (cmnd[*i] == MX_S_QUOTES && !mx_isescape_char(cmnd, *i)) {
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
    if (cmnd[*i] == c && !mx_isescape_char(cmnd, *i)) {
        (*i)++;
        while (cmnd[*i]) {
            if (cmnd[*i] == c && !mx_isescape_char(cmnd, *i)) {
                break;
            }
            (*i)++;
        }
    }
}

void mx_skip_quotes_s(char *cmnd, unsigned int *i) {
    if (cmnd[*i] == MX_S_QUOTES && !mx_isescape_char(cmnd, *i)) {
        (*i)++;
        for (; cmnd[*i]; (*i)++) {
            if (cmnd[*i] == MX_S_QUOTES
                && !mx_isescape_char(cmnd, *i)) {
                break;
            }
        }
    }
}

void mx_skip_grave(char *cmnd, unsigned int *i) {
    if (cmnd[*i] == MX_GRAVE_ACCENT && !mx_isescape_char(cmnd, *i)) {
        (*i)++;
        for (; cmnd[*i]; (*i)++;) {
            if (cmnd[*i] == MX_GRAVE_ACCENT
                && !mx_isescape_char(cmnd, *i)) {
                break;
            }
        }
    }
}

void mx_skip_exp(char *cmnd, unsigned int *i) {
    int brawl_stars = 0;
    if (cmnd[*i] == '$' && !mx_isescape_char(cmnd, *i)) {
        if (cmnd[*i + 1] == '(' && !mx_isescape_char(cmnd, *i)) {
            *i += 2;
            brawl_stars++;
            for (; cmnd[*i]; (*i)++) {
                if (cmnd[*i] == '(' && !mx_isescape_char(cmnd, *i)) {
                    brawl_stars++;
                }
                if (cmnd[*i] == ')' && !mx_isescape_char(cmnd, *i)) {
                    brawl_stars--;
                }
                if (!brawl_stars) {
                    break;
                }
            }
        }
    }
}

