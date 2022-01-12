#include "ush.h"

static void set_signals(sigset_t *sig, int fd) {
    sigfillset(sig);
    sigdelset(sig, SIGINT);
    sigdelset(sig, SIGQUIT);
    sigdelset(sig, SIGCONT);
    sigdelset(sig, SIGCHLD);
    if (fd == 1) {
        sigdelset(sig, SIGTSTP);
    }
}

t_list *mx_get_last_process(t_list *processes) {
    for (t_list *cur = processes; cur; cur = cur->next) {
        if (!cur->next) {
            return cur;
        }
    }
    return NULL;
}

t_process *mx_create_process(int fd) {
    t_process *process = malloc(sizeof(t_process));
    process->cmd = NULL;
    process->pos = 0;
    process->fd = fd;
    process->gpid = 0;
    set_signals(&process->signals, fd);
    posix_spawnattr_init(&process->attrs);
    posix_spawnattr_setpgroup(&process->attrs, process->gpid);
    posix_spawnattr_setsigmask(&process->attrs, &process->signals);
    posix_spawnattr_setflags(&process->attrs, POSIX_SPAWN_SETSIGMASK
                                              | POSIX_SPAWN_SETPGROUP
                                              | POSIX_SPAWN_START_SUSPENDED);
    posix_spawn_file_actions_init(&process->actions);
    posix_spawn_file_actions_adddup2(&process->actions, fd, 1);
    return process;
}

