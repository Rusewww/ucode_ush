#ifndef LIBMX
#define LIBMX

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <time.h>
#include <fcntl.h>
#include <grp.h>
#include <stdbool.h>
#include <malloc/malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <pwd.h>
#include <string.h>
#include <limits.h>
#include <termios.h>
#include <fcntl.h>
#include <ctype.h>
#include <regex.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <spawn.h>
#include <libgen.h>
#include <wordexp.h>

#define MX_SHELL_NAME "ush"

typedef struct s_list
{
    void *data;
    struct s_list *next;
}              t_list;

typedef struct s_d_list
{
    void *data;
    struct s_d_list *next;
    struct s_d_list *prev;
}              t_d_list;

typedef struct s_node
{
    void *data;
    char *key;
    struct s_node *next;
}              t_node;

typedef struct s_map
{
    t_node **nodes;
    size_t size;
}              t_map;

typedef enum e_var_list {
    SHELL,
    EXP
} t_var_list;

t_list *mx_sort_list(t_list *lst, bool (*cmp)(void *, void *));
int mx_list_size(t_list *list);
t_list  *mx_create_node(void *data);
void mx_push_front(t_list **list, void *data);
void mx_push_back(t_list **list, void *data);
void mx_pop_front(t_list **head);
void mx_pop_back(t_list **head);
void mx_del_list(t_list **head);

int mx_d_list_size(t_d_list *list);
t_d_list *mx_d_create_node(void *data);
void mx_d_push_front(t_d_list **list, void *data);
void mx_d_push_back(t_d_list **list, void *data);
void mx_d_pop_front(t_d_list **head);
void mx_d_pop_back(t_d_list **head);
void mx_d_del_list(t_d_list **list);

t_map *mx_create_map(size_t size);
void mx_put_map(t_map **map, char *key, void *value);
void *mx_get_map(t_map **map, char *key);
void mx_del_map(t_map **map);

char *mx_arr_to_str(char **arr);
int mx_arr_size(char **arr);
char *mx_pr_num_to_hex(unsigned long num);
void mx_del_strarr(char ***strs);
void mx_strdel(char **str);
int mx_count_substr(const char *str, const char *sub);
char *mx_file_to_str(const char *file);
char *mx_strtrim(const char *str);
char *mx_del_extra_spaces(const char *str);
int mx_strncmp(const char *s1, const char *s2, size_t n);
char *mx_strndup(const char *s1, size_t n);
char *mx_strncat(char *restrict s1, const char *restrict s2, size_t n);
char *mx_replace_substr(const char *str, const char *sub, const char *replace);
char *mx_strstr(const char *haystack, const char *needle);
void mx_printspaces(int spaces);
int mx_strcmp(const char *s1, const char *s2);
char *mx_strcat(char *restrict s1, const char *restrict s2);
char *mx_strjoin(const char *s1, const char *s2);
char *mx_itoa(int number);
int mx_atoi(const char *str);
int mx_sqrt(int n);
bool mx_isspace(char c);
bool mx_isdigit(char c);
void mx_print_unicode(wchar_t c);
void mx_printstr(const char *str);
int mx_numlen(int num);
char *mx_strnew(int s);
int mx_strlen(const char *s);
char *mx_strncpy(char *dst, const char *src, int len);
void mx_str_reverse(char *s);
char **mx_strsplit(const char *s, char c);
int mx_count_words(const char *str, char c);
char *mx_strdup(const char *str);
int mx_get_char_index(const char *str, char c);
int mx_get_substr_index(const char *str, const char *sub);
unsigned long mx_hex_to_nbr(const char *hex);
char *mx_nbr_to_hex(unsigned long nbr);
char *mx_strcpy(char *dst, const char *src);
void mx_swap_char(char *s1, char *s2);
int mx_bubble_sort(char **arr, int size);
void mx_print_strarr(char **arr, const char *delim);
void *mx_memmove(void *dst, const void *src, size_t len);
void *mx_memcpy(void *restrict dst, const void *restrict src, size_t n);
void *mx_realloc(void *ptr, size_t size);
bool mx_isescape(char c);
unsigned int mx_hash(void *data);
void mx_printerr(const char *s);
void mx_printchar(char c);
void mx_print_strarr(char **arr, const char *delim);
bool mx_isalpha(const char c);

void mx_get_name(char *var, char **name);
char *mx_get_var_info(char *arg, bool type);
char *mx_get_var_val(t_var_list key, char *var);
t_list **mx_get_var_list(t_var_list key);
struct termios *mx_get_tty(void);
t_map **mx_get_lenv(void);
t_list **mx_get_list_procs(void);

bool mx_check_colons(char **cmnd, int *code);
bool mx_check_exists(char *dir);

void mx_backspace(unsigned int times, int dir);

void mx_delete_names(char **n_one, char **n_two, t_list *node);

void mx_var_list_push_front(t_list **node, t_list ***list, char **name_one, char **name_two);
void mx_var_list_replace_var(t_list **list, t_list *node);
void mx_var_list_push_mid(t_list **list, t_list **node, char *arg_name, char **var_name);

void mx_var_list_insert(t_var_list key, char *arg);

void mx_var_list_delete(t_var_list key, char *del_n);

void mx_rcmd(char *dst, char *src, size_t size, unsigned int *index);

#endif
