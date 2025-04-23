/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_pid.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 16:07:31 by htopa             #+#    #+#             */
/*   Updated: 2025/04/23 15:24:31 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	command_not_found(t_cmd_parts **cmd_parts, t_args *arg_struct)
{
	if ((*cmd_parts)->cmd_array[0] != NULL)
	{
		if ((*cmd_parts)->cmd_array[0][0] == '\0')
			ft_putstr_fd(": command not found\n", 2);
		else
		{
			ft_putstr_fd((*cmd_parts)->cmd_array[0], 2);
			ft_putstr_fd(": command not found\n", 2);
		}
	}
	free_cmd_parts(cmd_parts);
	free(arg_struct->pids);
	free_envp(arg_struct->envp);
	free(arg_struct);
	return (127);
}

int	get_exit_code(void)
{
	if (errno == 13 || errno == 21 || errno == 20)
		return (126);
	if (errno == 2)
		return (127);
	else
		return (errno);
}

static int	execute_command(char *path, t_cmd_parts **cmd_parts,
							t_args *arg_struct)
{
	int	fd;

	fd = open(path, O_DIRECTORY);
	if (fd != -1)
	{
		close(fd);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(21), 2);
		ft_putstr_fd("\n", 2);
		free_before_return_2(arg_struct, cmd_parts, path);
		return (126);
	}
	else
	{
		set_signals(NULL, SIGNAL_CHILD);
		execve(path, (*cmd_parts)->cmd_array, arg_struct->envp);
		perror((*cmd_parts)->cmd_array[0]);
	}
	free_before_return_2(arg_struct, cmd_parts, path);
	return (get_exit_code());
}

int	run_single_builtin(t_cmd_parts **cmd_parts, t_shell *shell,
	char ***envp, int *original_fd)
{
	int		ret;

	set_signals(NULL, SIGNAL_CHILD);
	ret = check_and_run_builtins_single(shell, cmd_parts, envp, original_fd);
	set_signals(NULL, SIGNAL_PARENT);
	shell->exit_code = ret;
	return (ret);
}

int	run_pid(t_args *arg_struct, t_cmd_parts **cmd_parts, t_shell *shell,
	int is_builtin)
{
	char	*path;
	char	**command_array;
	int		ret;

	command_array = (*cmd_parts)->cmd_array;
	path = find_command_path(command_array[0], arg_struct->envp);
	if (set_pipe((*cmd_parts), arg_struct, path, is_builtin) == -1)
	{
		free_cmd_parts(cmd_parts);
		free(path);
		return (EXIT_FAILURE);
	}
	if (is_builtin == 1)
	{
		set_signals(NULL, SIGNAL_CHILD);
		ret = check_and_run_builtins(shell, cmd_parts, arg_struct);
		set_signals(NULL, SIGNAL_PARENT);
		free_before_return_1(arg_struct, path);
		return (ret);
	}
	else if (path != NULL)
		return (execute_command(path, cmd_parts, arg_struct));
	else
		return (command_not_found(cmd_parts, arg_struct));
}
