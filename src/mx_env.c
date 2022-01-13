#include "../inc/ush.h"

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

static bool run(t_process *pr, char *name, char **argv, char **env) {
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

static void add(t_process *pr) {
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


static void copy_environ(char **copy, char **environ) {
    int i = 0;
    while (environ[i]) {
        copy[i] = strdup(environ[i]);
        if (environ[i + 1] == NULL) {
            copy[i + 1] = NULL;
        }
        i += 1;
    }
}

static void de_init(char ***env, char **path, char **name) {
    mx_clear_env();
    mx_env_fill(*env);
    if (*env && *env[0]) {
        mx_del_strarr(env);
    }
    if (*path) {
        mx_strdel(path);
    }
    if (*name) {
        mx_strdel(name);
    }
}

static int exec_process(char *filename, char **argv, int fd) {
    extern char **environ;
    t_process *process = mx_create_proc(fd);
    int rval = mx_env_exec(process, filename, argv, environ);
    mx_del_process(&process);
    return rval;
}

int mx_env(char **argv, int fd) {
    char **env = mx_env_copy();
    char *name = NULL;
    char *path = NULL;
    int rval = 0;
    int i = 0;

    if (!(rval = mx_env_flags_parse(argv, &path, &i))) {
        mx_env_vars_parse(argv, &path, &i);
        if (argv[i] == NULL) {
            mx_print_env(fd);
        } else if (mx_find_command(path, argv[i], &name)) {
            rval = exec_process(name, &argv[i], fd);
        } else {
            fprintf(stderr, "env: %s: No such file or directory\n", argv[i]);
            rval = 127;
        }
    }
    de_init(&env, &path, &name);
    return rval;
}

char **mx_env_copy(void) {
    extern char **environ;
    char **env_clone = NULL;
    int length;

    for (length = 0; environ[length]; length++);

    if (environ[0]) {
        env_clone = malloc((length + 1) * sizeof(char *));
        copy_environ(env_clone, environ);
    }
    return env_clone;
}

int mx_env_exec(t_process *process, char *name, char **argv, char **env) {
    int rval = 0;
    mx_disable_canon();

    if (!run(process, name, argv, env)) {
        fprintf(stderr, "env: %s: %s\n", name, strerror(process->status));
        rval = 126;
    } else if (waitpid(process->pid, &process->status, WUNTRACED) != -1) {
        if (MX_WIFSTOPPED(process->status)) {
            add(process);
        }
    }

    tcsetpgrp(STDOUT_FILENO, getpgrp());
    mx_enable_canon();
    return rval != 126 ? MX_WEXITSTATUS(process->status) : rval;
}

void mx_env_fill(char **src) {
    int i = 0;
    while (src && src[i]) {
        mx_putenv(src[i]);
        i++;
    }
}

void mx_clear_env(void) {
    extern char **environ;
    char **del_var = NULL;
    char *del_n = NULL;

    if (environ[0] != NULL) {
        del_var = mx_env_copy();
        int i = 0;
        while (del_var[i]) {
            mx_get_name(del_var[i], &del_n);
            unsetenv(del_n);
            i++;
        }
        if (del_var[0]) {
            mx_del_strarr(&del_var);
        }
        if (del_n != NULL) {
            mx_strdel(&del_n);
        }
    }
}

