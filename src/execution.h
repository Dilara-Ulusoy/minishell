/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 22:03:15 by htopa             #+#    #+#             */
/*   Updated: 2025/03/07 10:35:57 by htopa            ###   ########.fr       */
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

typedef struct	s_cmd_parts
{
	int command_number;
	int num_commands;
	int n_cmd;
	int n_in;
	int n_out;
	int *outfiles_types;
	char **cmd_array;
	char **infiles_array;
	char **outfiles_array;
} t_cmd_parts;

typedef struct s_args
{
	int		argc;
	char	**envp;
	int		**fd;
	int		*pids;
}	t_args;

void find_ast_pipes(t_ast_node *node);
void count_pipes(t_ast_node *node, int *i);
t_cmd_parts *count_tokens(const t_token *head, int command_number);
t_cmd_parts *get_command_array(const t_token *head, int command_number);
int ft_pwd();
int is_number(char *str);
int ft_exit(char *exit_code);
int ft_env(char **envp);
int ft_cd(char *new_path);
int get_num_commands(t_shell *shell);
int check_and_run_builtins(t_shell *shell, t_cmd_parts *cmd_parts, char **envp);
int execute_commands(t_shell *shell, int num_commands, char **envp);
void	close_pipes(int **fd, int j);
t_args	*add_fd_to_struct(t_args *arg_struct, int argc);
t_args	*initialize_struct_with_pids_and_fd(int argc);
t_args	*prepare_struct(int num_commands, char **envp);
int	set_pipe(t_cmd_parts *cmd_parts, t_args *arg_struct, char *path, int is_builtin);
void	free_array(void **arr, int is_int, int j);
int	close_and_free(t_args *arg_struct, int free_pid);
int	open_and_check_file(char *filename, int type);
char	*find_command_path(char *cmd, char **envp);
int	display_error_message(int i, t_args *arg_struct);
int	check_dup2(int file_d, t_args *arg_struct, int std_type);
int is_builtin(t_cmd_parts *cmd_parts);
#endif