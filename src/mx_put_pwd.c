#include "ush.h"

void mx_put_pwd(char *pwd, char *old_pwd) {
    char *new_pwd = mx_strjoin("PWD=", pwd);
    char *remake_pwd = mx_strjoin("OLDPWD=", old_pwd);
    chdir("/");
    chdir(pwd);
    mx_var_list_insert(SHELL, new_pwd);
    mx_var_list_insert(EXP, new_pwd);
    mx_putenv(new_pwd);
    mx_var_list_insert(SHELL, remake_pwd);
    mx_var_list_insert(EXP, remake_pwd);
    mx_putenv(remake_pwd);
    mx_strdel(&new_pwd);
    new_pwd = NULL;
    mx_strdel(&remake_pwd);
    remake_pwd = NULL;
}
