#ifndef USH
#define USH

#include "../libmx/inc/libmx.h"

#define MX_SHELL_PROMPT "u$h> "
#define DEFAULT_PATH "PATH=/bin:/usr/bin:/usr/local/bin:/usr/sbin:/sbin"
#define MX_UP_ARROW "\x1b\x5b\x41"
#define MX_DOWN_ARROW "\x1b\x5b\x42"
#define MX_RIGHT_ARROW "\x1b\x5b\x43"
#define MX_LEFT_ARROW "\x1b\x5b\x44"
#define MX_MOVE_CURSOR_LEFT "\x1b[1C"
#define MX_MOVE_CURSOR_RIGHT "\x1b[1D"
#define MX_DELETE_KEY "\x1b\x5b\x33\x7e"
#define MX_HOME_KEY "\x1b\x5b\x48"
#define MX_END_KEY "\x1b\x5b\x46"
#define MX_NON_PRINTABLE "[\x03\x0a]"
#define MX_NEW_LINE_CHARS "^[\x03\x0a]$"
#define MX_D_QUOTES '\"'
#define MX_S_QUOTES '\''
#define MX_GRAVE_ACCENT '`'
#define MX_ESCAPE_CHARS "\\\"\'ntva$` "
#define MX_FORBIDDEN_CHARS "|&><"
#define MX_HISTORY_SIZE 200
#define MX_EXPORT_ARG "^[A-Za-z_]+[A-Za-z0-9_]*(=.*)?$"
#define MX_ENV_NAME "^[A-Za-z_]+[A-Za-z0-9_]*$"
#define MX_UNSET_ARG "^([0-9]+|[A-Za-z_]+[0-9A-Za-z_]*)$"
#define MX_ENV_FLAG_I "^-(i*|i+.*|-.+)$"
#define MX_ENV_VAR "^.+=.*$"
#define MX_SPEC_ENV "$?#*@_0"

#define MX_W_INT(m) (*(int*) & (m))
#define MX_WSTOPSIG(m) (MX_W_INT(m) >> 8)
#define MX_WSTATUS(m) (MX_W_INT(m) & 0177)
#define MX_WIFSTOPPED(m) (MX_WSTATUS(m) == _WSTOPPED && MX_WSTOPSIG(m) != 0x13)
#define MX_ISREG(m) (((m) & S_IFMT) == S_IFREG)
#define MX_WAIT_TO_INT(m) (*(int *) & (m))
#define MX_WEXITSTATUS(x) ((MX_WAIT_TO_INT(x) >> 8) & 0x000000ff)

typedef struct s_process {
    int fd;
    int pos;
    char *cmd;
    int status;
    posix_spawn_file_actions_t actions;
    posix_spawnattr_t attrs;
    pid_t pid;
    pid_t gpid;
    sigset_t signals;
} t_process;

typedef struct s_prompt {
    unsigned int index;
    unsigned int cursor_index;
    bool end;
    t_d_list *history_head;
    t_d_list *history_back;
    t_d_list *tmp_history;
    char buff[5];
    char command[ARG_MAX + 1];
    char tmp_command[ARG_MAX + 1];
} t_prompt;


bool mx_find_command(char *path, char *command, char **filename);

//mx_pre_interpret

bool mx_check_parentheses(char *c);
bool mx_check_char(char *c);
bool mx_check_substitutions(char *c);
bool mx_check_quotes(char *c);
int mx_pre_interpret(char *c);

//mx_getters

void mx_get_in(t_prompt *prompt, int fd, int *code);

//mx_skip

void mx_skip_quotes(char *cmnd, unsigned int *i, char c);
void mx_skip_quotes_s(char *cmnd, unsigned int *i);
void mx_skip_grave(char *cmnd, unsigned int *i);
void mx_skip_exp(char *cmnd, unsigned int *i);
void mx_skip_exps_quotes(char *arg, unsigned int *index, bool *quote);
void mx_skip_quotes_if(bool *quote, char *arg, unsigned int *i);

//mx_split_cmds

t_list *mx_split_cmd(char *command);
char **mx_split_commands(char *command);

//mx_built_in

bool mx_built_in(char *file, int *mode);
bool mx_is_built_in(char *com);

//mx_cd

int mx_cd(char **split, int dir);
void mx_change_dir(char *new_dir, t_map **map, int dir);
void mx_cd_flags(char *flag, t_map **map, char *new_dir);
void mx_flag_p_slash(char *nd, t_map **map);
void mx_flag_p_path(t_map **map, char *new_dir);
void mx_check_s_slash(char *dir, t_map **map);

//mx_change_map

void mx_change_map(t_map **map, char *dir);

//mx_check_escape

bool mx_check_escape(char *cmnd);

//mx_env

void mx_clear_env(void);
int mx_env(char **argv, int fd);
char **mx_env_copy(void);
int mx_env_exec(t_process *process, char *name, char **argv, char **env);
void mx_env_fill(char **src);

//mx_env_parse

int mx_env_flags_parse(char **argv, char **path, int *idx);
void mx_env_vars_parse(char **argv, char **path, int *idx);

//mx_color

void mx_print_color(int color);
int mx_color(char **args);

//mx_process

t_list *mx_get_last_proc(t_list *processes);
t_process *mx_create_proc(int fd);
void mx_continue_proc(t_process *process, t_list **processes, int fd);
void mx_del_process(t_process **process);
void mx_kill_process(void);

//mx_del_node_list

void mx_del_node_list(t_list **list, t_process **d_p);

//mx_canon

void mx_disable_canon(void);
void mx_enable_canon(void);

//mx_echo

int mx_echo(char **args, int dr);

//mx_exec

int mx_exec(t_process *process, char *name, char **argv, char **env);
int mx_exec_command(char **argv, int fd);
char **mx_exec_copy_argv(char **argv);
char **mx_exec_copy_argv(char **argv);
void mx_exec_fork(char **arguments, int fd);

//mx_exit

int mx_exit(char **args);

//mx_export

int mx_export(char **args, int fd);

//mx_fg

int mx_fg(char **args, int fd);

//mx_handle

void mx_handle_cmnd(char *cmnd, int *code);
void mx_handle_cursor(t_prompt *prompt);
bool mx_handle_his(t_prompt *prompt);
void mx_handle_print_char(t_prompt *prompt);
t_list *mx_handle_subs(t_list *arg);

//mx_home

void mx_home(t_map **map);

//mx_increase_sh_lvl

void mx_increase_sh_lvl(void);

//mx_init

void mx_init_var_ls(void);
void mx_init_signals(void);
void mx_init_map_v(void);
void mx_init(void);
void mx_de_init(void);

//mx_interpretate

char **mx_interpretate(char *cmnd, int *code);

//mx_make_null_index

char **mx_make_null_index(char **split, int index);

//mx_match

bool mx_match(char *src, char *regex);

//mx_old_pwd

void mx_old_pwd(char *new_dir, t_map **map, int fd);

//mx_parse

char **mx_parse_cmnd(char *cmnd, int *code);
char *mx_parse_path(char *path, char *new_dir, t_map **map);

//mx_print

void mx_print_env(int fd);
int mx_print_env_error(char option, char *err_arg, int error);
void mx_print_var_list(t_var_list key, int fd);

//mx_pwd

int mx_pwd(char **flags, int fd);
void mx_put_pwd(char *pwd, char *old_pwd);

//mx_put_env

void mx_put_env(char *var);

//mx_remove_sub_char

bool mx_remove_sub_char(char *subs);

//mx_replace

char *mx_get_invalid_sub(char **arg, char **res, char **sub);
char *mx_check_user_file(char *tmp_name);
bool mx_check_user(char *username);
void mx_replace_sub_escapes(char **cmnd);
char *mx_replace_tilde(char *arg);
char *mx_replace_substitution(char *arg, int *code);
bool mx_get_sub(char *arg, char *sub, int *code);
void mx_inc_val_var(unsigned int *length, unsigned int add, char *var);
bool mx_check_trimmed_str(char *sub_trim, char *sub);
bool mx_replace_env_var(char *res, char *env, unsigned int *index, unsigned int length);
char *mx_replace_escape(char *arg, char *esc, char new, bool in_q);
char *mx_replace_env(char *arg, int *code);
char *mx_get_output_fd(int fd);

//mx_source

char **mx_source(char *str);

//mx_str_prompt

char *mx_str_prompt(void);

//mx_unset

int mx_unset_var(char *var);
int mx_unset(char **args);

//mx_update_history

void mx_update_history(t_prompt *prompt);

//check_stdin

bool check_stdin(int *exit_code);

//mx_witch

int mx_which(char **args, int fd);

#endif
