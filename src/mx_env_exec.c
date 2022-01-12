#include "ush.h"

static char *argv_to_str(char **argv) {
    char str[ARG_MAX] = "";
    unsigned int i = 0;
    while (argv[i]) {
        strcat(str, argv[i]);
        if (argv[i + 1]) {
            strcat(str, " ");
        }
        i++;
    }
    return strdup(str);
}

static bool run_process(t_process *pr, char *name, char **argv, char **env) {
    pr->cmd = argv_to_str(argv);
    pr->status = posix_spawn(&pr->pid, name, &pr->actions, &pr->attrs, argv, env);
    pr->gpid = getpgid(pr->pid);
    tcsetpgrp(STDIN_FILENO, pr->gpid);
    kill(-pr->pid, SIGCONT);
    if (pr->status) {
        return false;
    }
    return true;
}

static void add_process(t_process *pr) {
    t_list **list = mx_get_list_procs();
    t_list *tmp = *list;

    tcsetpgrp(STDIN_FILENO, getpgrp());
    mx_enable_canon();
    for (; tmp; tmp = tmp->next) {
        if (!tmp->next) {
            break;
        }
    }
    mx_push_back(list, pr);
    if (tmp) {
        pr->pos = ((t_process *) tmp->data)->pos + 1;
    } else {
        pr->pos = 1;
    }
    printf("[%d]    %d suspended  %s\n", pr->pos, pr->pid, pr->cmd);
}

int mx_env_exec(t_process *process, char *name, char **argv, char **env) {
    int rval = 0;
    mx_disable_canon();

    if (!run_process(process, name, argv, env)) {
        fprintf(stderr, "env: %s: %s\n", name, strerror(process->status));
        rval = 126;
    } else if (waitpid(process->pid, &process->status, WUNTRACED) != -1) {
        if (MX_WIFSTOPPED(process->status)) {
            add_process(process);
        }
    }

    tcsetpgrp(STDOUT_FILENO, getpgrp());
    mx_enable_canon();
    return rval != 126 ? MX_WEXITSTATUS(process->status) : rval;
}
