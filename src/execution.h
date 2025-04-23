/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 22:03:15 by htopa             #+#    #+#             */
/*   Updated: 2025/04/23 10:06:40 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "minishell.h"
# include <unistd.h>
# include <errno.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <fcntl.h>
# include <stdio.h>
# include <limits.h>

typedef struct s_cmd_parts
{
	int		num_commands;
	int		command_number;
	int		n_cmd;
	int		n_in;
	int		n_out;
	int		n_files;
	int		*files_types;
	char	**cmd_array;
	char	**files_array;
}	t_cmd_parts;

typedef struct s_args
{
	int		argc;
	char	**envp;
	int		**fd;
	pid_t	*pids;
}	t_args;

typedef struct s_helper
{
	int		fileno;
	int		is_builtin;
	char	*path;
}	t_helper;

t_cmd_parts	*get_command_array(const t_token *head, int command_number);
int			ft_pwd(t_shell *shell);
int			is_number(char *str);
int			ft_exit(char *exit_code);
int			ft_env(char **envp);
int			ft_cd(char *new_path, char ***envp);
int			ft_unset(char *value, char ***envp);
int			check_and_run_builtins(t_shell *shell, t_cmd_parts **cmd_parts,
				t_args *arg_struct);
int			execute_commands(t_shell *shell, int num_commands, char ***envp);
void		close_pipes(int **fd, int j);
t_args		*add_fd_to_struct(t_args *arg_struct, int argc);
t_args		*initialize_struct_with_pids_and_fd(int argc);
t_args		*prepare_struct(int num_commands, char **envp);
int			set_pipe(t_cmd_parts *cmd_parts, t_args *arg_struct, char *path,
				int is_builtin);
int			set_pipe_single_builtin(t_cmd_parts *cmd_parts);
void		free_array(void **arr, int is_int, int j);
int			close_and_free(t_args *arg_struct, int free_pid);
int			open_and_check_file(char *filename, int type);
char		*find_command_path(char *cmd, char **envp);
int			display_error_message(int i, t_args *arg_struct);
int			check_dup2(int file_d, t_args *arg_struct, int std_type);
int			is_builtin(t_cmd_parts *cmd_parts);
void		free_cmd_parts(t_cmd_parts **cmd_parts);
char		**copy_envp(char **envp);
void		free_envp(char **envp);
void		ft_add_env_var(char *var_eq_value, char ***envp);
int			ft_set(char *var_eq_value, char ***envp);
int			check_and_run_builtins_single(t_shell *shell,
				t_cmd_parts **cmd_parts, char ***env, int *original_fd);
char		*ft_getenv(char *var_name, t_shell *shell);
int			get_exit_code(void);
int			run_pid(t_args *arg_struct, t_cmd_parts **cmd_parts, t_shell *shell,
				int is_builtin);
int			run_single_builtin(t_cmd_parts **cmd_parts, t_shell *shell,
				char ***envp, int *original_fd);
int			get_num_commands(const t_token *head);
void		free_cmd_clean_shell(t_shell *shell, t_cmd_parts *cmd_parts);
void		set_signal_in_child(void);
void		set_signal_in_parent(void);
t_cmd_parts	*initialize_cmd_parts(void);
int			get_file_type(const t_token *curr);
int			contains_equal(char *s);
char		*copy_until_equal(char *src);
int			ft_strcmp_wo_equal(char *s1, char *s2);
int			check_echo_option(char *str);
int			run_unset(t_cmd_parts **cmd_parts, char ***envp);
int			run_export(t_cmd_parts **cmd_parts, char ***envp);
int			run_echo(t_cmd_parts **cmd_parts);
int			run_env(t_cmd_parts **cmd_parts, char ***envp);
int			run_cd(t_cmd_parts **cmd_parts, char ***envp);
void		restore_fd(int *original_fd);
int			arrange_in_file(t_cmd_parts *cmd_parts, t_args *arg_struct,
				int fileno, int i);
int			arrange_out_file(t_cmd_parts *cmd_parts, t_args *arg_struct,
				t_helper *helper, int i);
t_helper	*initialize_helper(t_cmd_parts *cmd_parts, char *path,
				int is_builtin, int i);
void		free_helper(t_helper *helper);
void		free_before_return_1(t_args *arg_struct, char *path);
void		free_before_return_2(t_args *arg_struct,
				t_cmd_parts **cmd_parts, char *path);
char		**free_before_return_3(char **new_envp, int i);
void		free_cmd_array(t_cmd_parts **cmd_parts);
void		ft_putstr_with_quotes_fd(char *s, int fd);

#endif
