#include "ush.h"

static t_list *get_process_by_cmd(char *arg, t_list *proc) {
    t_list *ret = NULL;
    unsigned int count = 0;
    t_process *tmp = NULL;

    for (; proc; proc = proc->next) {
        tmp = (t_process *) proc->data;
        if (!mx_get_substr_index(tmp->cmd, arg)) {
            count++;
            ret = proc;
        }
    }
    if (count == 1) {
        return ret;
    } else if (count > 1) {
        fprintf(stderr, "fg: %s: ambiguous job spec\n", arg);
    } else if (!count) {
        fprintf(stderr, "fg: %s: no such job\n", arg);
    }
    return NULL;
}

static t_list *get_process_by_id(char *arg, t_list *proc) {
    int cur = atoi(arg);
    t_process *tmp = NULL;

    for (; proc; proc = proc->next) {
        tmp = (t_process *) proc->data;
        if (tmp->pos == cur) {
            return proc;
        }
    }
    fprintf(stderr, "fg: %s: no such job\n", arg);
    return NULL;
}

static t_list *get_process(char *arg) {
    bool is_num = true;
    unsigned int lenght = 0;
    t_list **proc = mx_get_list_procs();

    if (!arg)
        return mx_get_last_process(*proc);
    arg++;
    lenght = strlen(arg);
    for (unsigned int i = 0; i < lenght; i++) {
        if (!isnumber(arg[i])) {
            is_num = false;
            break;
        }
    }
    if (is_num) {
        return get_process_by_id(arg, *proc);
    } else {
        return get_process_by_cmd(arg, *proc);
    }
}

static bool check_args(char **args) {
    if (!mx_arr_size(args)) {
        return true;
    }
    if (mx_arr_size(args) > 1) {
        fprintf(stderr, "fg: too many arguments\n");
        return false;
    }
    if (args[0][0] != '%' || !args[0][1]) {
        fprintf(stderr, "fg: invalid argument: %s\n", args[0]);
        return false;
    }
    return true;
}

int mx_fg(char **args, int fd) {
    t_list *process = NULL;
    t_process *process_F_to_pay_respect = NULL;
    t_list **all = mx_get_list_procs();

    if (!check_args(args)) {
        return 1;
    }
    process = get_process(args[0]);
    if (process) {
        process_F_to_pay_respect = (t_process *) process->data;
        mx_disable_canon();
        tcsetpgrp(STDIN_FILENO, process_F_to_pay_respect->gpid);
        mx_continue_process(process_F_to_pay_respect, all, fd);
        tcsetpgrp(STDIN_FILENO, getpgrp());
        mx_enable_canon();
    } else {
        fprintf(stderr, "%s", *args ? "" : "fg: no current jobs\n");
        return 1;
    }
    return process_F_to_pay_respect->status;
}
