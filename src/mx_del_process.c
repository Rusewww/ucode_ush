#include "../inc/ush.h"

void mx_del_process(t_process **process) {
    if (!MX_WIFSTOPPED((*process)->status)) {
        posix_spawnattr_destroy(&(*process)->attrs);
        posix_spawn_file_actions_destroy(&(*process)->actions);
        mx_strdel(&(*process)->cmd);
        free(*process);
        process = NULL;
    }
}

void mx_kill_process(void) {
    t_list **processes = mx_get_list_procs();
    t_process *tmp = NULL;
    t_list *cur = *processes;
    while (cur) {
        tmp = (t_process *) cur->data;
        kill(-tmp->pid, SIGKILL);
        mx_del_node_list(processes, &tmp);
        cur = cur->next;
    }
}
