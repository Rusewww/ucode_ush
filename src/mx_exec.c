#include "../inc/ush.h"

static int built_in(char *command, char **argv, int fd) {
    if (!strcmp(command, "export")) {
        return mx_export(&argv[1], fd);
    }
    if (!strcmp(command, "which")) {
        return mx_which(&argv[1], fd);
    }
    if (!strcmp(command, "unset")) {
        return mx_unset(&argv[1]);
    }
    if (!strcmp(command, "color")) {
        return mx_color(&argv[1]);
    }
    if (!strcmp(command, "exit")) {
        return mx_exit(&argv[1]);
    }
    if (!strcmp(command, "pwd")) {
        return mx_pwd(&argv[1], fd);
    }
    if (!strcmp(command, "cd")) {
        return mx_cd(&argv[1], fd);
    }
    if (!strcmp(command, "echo")) {
        return mx_echo(&argv[1], fd);
    }
    if (!strcmp(command, "env")) {
        return mx_env(&argv[1], fd);
    }
    if (!strcmp(command, "fg")) {
        return mx_fg(&argv[1], fd);
    }
    return 1;
}

static int print_exec_error(char *error_name) {
    if (mx_match(error_name, "/")) {
        fprintf(stderr, "%s: %s: No such file or directory\n", MX_SHELL_NAME, error_name);
    } else {
        fprintf(stderr, "%s: %s: Command not found\n", MX_SHELL_NAME, error_name);
    }
    return 127;
}

static char *argv_to_str(char **argv) {
    char str[ARG_MAX] = "";

    for (unsigned int i = 0; argv[i]; i++) {
        strcat(str, argv[i]);
        if (argv[i + 1]) {
            strcat(str, " ");
        }
    }
    return strdup(str);
}

static bool run(t_process *process, char *filename, char **argv, char **env) {
    process->cmd = argv_to_str(argv);
    process->status = posix_spawn(&process->pid, filename, &process->actions, &process->attrs, argv, env);
    process->gpid = getpgid(process->pid);
    tcsetpgrp(STDIN_FILENO, process->gpid);
    kill(-process->pid, SIGCONT);
    if (process->status) {
        return false;
    }
    return true;
}

static void add(t_process *process) {
    t_list **list = mx_get_list_procs();
    t_list *tmp = *list;

    tcsetpgrp(STDIN_FILENO, getpgrp());
    mx_enable_canon();
    for (; tmp; tmp = tmp->next) {
        if (!tmp->next) {
            break;
        }
    }
    mx_push_back(list, process);
    if (tmp) {
        process->pos = ((t_process *) tmp->data)->pos + 1;
    } else {
        process->pos = 1;
    }
    printf("[%d]    %d suspended  %s\n", process->pos, process->pid, process->cmd);
}

int mx_exec(t_process *process, char *name, char **argv, char **env) {
    int rval = 0;
    mx_disable_canon();
    if (!run(process, name, argv, env)) {
        fprintf(stderr, "%s: %s: %s\n", MX_SHELL_NAME, name, strerror(process->status));
        rval = 126;
    } else if (waitpid(-process->pid, &process->status, WUNTRACED) != -1) {
        if (MX_WIFSTOPPED(process->status)) {
            add(process);
        }
    }
    if (!MX_WIFSTOPPED(process->status)) {
        tcsetpgrp(STDIN_FILENO, getpgrp());
        mx_enable_canon();
    }
    return rval != 126 ? MX_WEXITSTATUS(process->status) : rval;
}

int mx_exec_command(char **argv, int fd) {
    char *filename = NULL;
    int retval = 0;

    if (mx_is_built_in(argv[0])) {
        return built_in(argv[0], argv, fd);
    } else if (mx_find_command(mx_get_var_val(SHELL, "PATH"), argv[0], &filename)) {
        extern char **environ;
        t_process *process = mx_create_proc(fd);

        retval = mx_exec(process, filename, argv, environ);
        mx_del_process(&process);
        mx_strdel(&filename);
    } else {
        retval = print_exec_error(argv[0]);

    }
    return retval;
}

char **mx_exec_copy_argv(char **argv) {
    size_t size = mx_arr_size(argv);
    char **clone = malloc(sizeof(char *) * (size + 1));
    clone[size] = NULL;
    unsigned int i = 0;
    while (i < size) {
        clone[i] = strdup(argv[i]);
        i++;
    }
    return clone;
}

void mx_exec_fork(char **arguments, int fd) {
    pid_t child = fork();
    if (!child) {
        mx_exec_command(arguments, fd);
        exit(0);
    }
    waitpid(child, NULL, 0);
}

