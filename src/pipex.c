/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 16:07:31 by htopa             #+#    #+#             */
/*   Updated: 2025/04/16 02:04:12 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"
#include "signal.h"  // 👈 Sinyaller için ekledik

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
	if (errno == 13 || errno == 21)
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
		if (path != (*cmd_parts)->cmd_array[0])
			free(path);
		free_cmd_parts(cmd_parts);
		free(arg_struct->pids);
		free_envp(arg_struct->envp);
		free(arg_struct);
		return (126);
	}
	else
	{
		// Child process sinyal ayarları yapılır
		set_signals(NULL, SIGNAL_CHILD);
		execve(path, (*cmd_parts)->cmd_array, arg_struct->envp);
		perror((*cmd_parts)->cmd_array[0]);
	}
	if (path != (*cmd_parts)->cmd_array[0])
		free(path);
	free_cmd_parts(cmd_parts);
	free(arg_struct->pids);
	free_envp(arg_struct->envp);
	free(arg_struct);
	return (get_exit_code());
}

int	run_single_builtin(t_cmd_parts **cmd_parts, t_shell *shell, char ***envp, int *original_fd)
{
	char	*path;
	char	**command_array;
	int		ret;

	command_array = (*cmd_parts)->cmd_array;
	path = find_command_path(command_array[0], *envp);

	// 👇 Sinyalleri çalıştırmadan önce geçici olarak CHILD olarak ayarla
	set_signals(NULL, SIGNAL_CHILD);
	ret = check_and_run_builtins_single(shell, cmd_parts, envp, original_fd);
	set_signals(NULL, SIGNAL_PARENT); // Tekrar eski haline getir
	shell->exit_code = ret; // Eğer bu alan shell struct'ında varsa
	free(path);
	return (ret);
}

int	run_pid(t_args *arg_struct, t_cmd_parts **cmd_parts, t_shell *shell,
	int is_builtin)
{
	char	*path;
	char	**command_array;
	int		check_status;
	int		ret;

	command_array = (*cmd_parts)->cmd_array;
	path = find_command_path(command_array[0], arg_struct->envp);
	check_status = set_pipe((*cmd_parts), arg_struct, path, is_builtin);
	if (check_status == -1)
	{
		free_cmd_parts(cmd_parts);
		free(path);
		return (EXIT_FAILURE);
	}
	if (is_builtin == 1)
	{
		// 👇 Built-in de olsa sinyaller child gibi ayarlanır
		set_signals(NULL, SIGNAL_CHILD);
		ret = check_and_run_builtins(shell, cmd_parts, arg_struct);
		set_signals(NULL, SIGNAL_PARENT); // restore
		free(path);
		free(arg_struct->pids);
		free_envp(arg_struct->envp);
		free(arg_struct);
		return (ret);
	}
	else if (path != NULL)
		return (execute_command(path, cmd_parts, arg_struct));
	else
		return (command_not_found(cmd_parts, arg_struct));
}
