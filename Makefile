NAME = ush

SRCD = src
INCD = inc
OBJD = obj
LBMXD = libmx
LIBMX = libmx

LBMX = libmx.a
INC = ush.h
SRC = mx_ush.c \
	main.c \
	mx_exec_fork.c \
	mx_replace_tilde_tools.c \
	mx_replace_funcs.c \
	mx_continue_process.c \
	mx_del_node_list.c \
	mx_color.c \
	mx_replace_substitution.c \
	mx_replace_tilde.c \
	mx_pre_interpret.c \
	mx_get_in.c \
    mx_export.c \
	mx_unset.c \
	mx_pwd.c \
	mx_enable_canon.c \
	mx_disable_canon.c \
	mx_match.c \
	mx_handle_print_char.c \
	mx_handle_history.c \
	mx_update_history.c \
	mx_print_env.c \
	mx_is_flag_stop.c \
	mx_rcmd.c \
	mx_which.c \
	mx_str_prompt.c \
	mx_handle_cursor.c \
	mx_interpretate.c \
	mx_home.c \
	mx_split_cmds.c \
	mx_handle_command.c \
	mx_parse_path.c \
	mx_init_var_lists.c \
	mx_print_var_list.c \
	mx_check_escape.c \
	mx_isescape_char.c \
	mx_replace_escape.c \
	mx_replace_env.c \
	mx_echo.c \
	mx_cd.c \
	mx_issubstitution.c \
	mx_exec.c \
	mx_env_exec.c \
	mx_create_process.c \
	mx_handle_substitution.c \
	mx_remove_subchar.c \
	mx_skip.c \
	mx_parse_command.c \
	mx_make_null_index.c \
	mx_source.c \
	mx_put_pwd.c \
	mx_env_copy.c \
	mx_exec_command.c \
	mx_is_builtin.c \
	mx_find_command.c \
	mx_del_process.c \
	mx_putenv.c \
	mx_clearenv.c \
	mx_env_fill.c \
	mx_print_env_error.c \
	mx_env_parse_vars.c \
	mx_env_parse_flags.c \
	mx_replace_sub.c \
	mx_exit.c \
	mx_env.c \
	mx_fg.c \
	mx_increment_shlvl.c \
	mx_init_map_vars.c \
	mx_init_signals.c \
	mx_exec_copy_argv.c \
	mx_oldpwd.c \
	mx_clear_slash_end.c \
	mx_builtin_which.c \
	mx_unset_var.c \
	mx_change_map.c \

INCLUDE = -I $(LBMXD) \
	-I $(INCD) \

LBMXS = $(addprefix $(LBMXD)/, $(LBMX))
INCS = $(addprefix $(INCD)/, $(INC))
SRCS = $(addprefix $(SRCD)/, $(SRC))
OBJS = $(addprefix $(OBJD)/, $(SRC:%.c=%.o))
	
CFLAGS = -std=c11 -g $(addprefix -W, all extra error pedantic)
CC = clang

all: $(NAME)

install: $(NAME) clean

$(LBMXS):
	@make -sC $(LBMXD)

$(LIBMX): $(LBMXS)
	@make -sC $(LBMXD)
	
$(NAME): $(OBJS) $(LIBMX)
	@$(CC) $(CFLAGS) $(LBMXS) $(OBJS) -o $@
	@printf "\x1b[32;1m$@ created\x1b[0m\n"

$(OBJD)/%.o: $(SRCD)/%.c $(INCS)
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE)
	@printf "\x1b[32mcompiled: \x1b[0m[$(<:$(SRCD)/%.c=%.c)]\n"

$(OBJS): | $(OBJD)

$(OBJD):
	@mkdir -p $@

uninstall: clean
	@make -sC $(LBMXD) $@
	@rm -rf $(NAME)
	@printf "\x1b[34;1mdeleted $(NAME)\x1b[0m\n"

clean:
	@make -sC $(LBMXD) $@
	@rm -rf $(OBJD)
	@printf "\x1b[34;1mdeleted $(OBJD)\x1b[0m\n"

reinstall: uninstall install
