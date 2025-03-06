/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 13:33:50 by htopa             #+#    #+#             */
/*   Updated: 2025/03/06 21:49:41 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"

static int	command_not_found(char **command_array, t_args *arg_struct)
{
	if (command_array[0] == NULL)
	{
		//ft_putstr_fd(arg_struct->argv[i], 2);
		ft_putstr_fd(": command not found\n", 2);
	}
	else
	{
		ft_putstr_fd(command_array[0], 2);
		ft_putstr_fd(": command not found\n", 2);
	}
	free_array((void **)command_array, 0, -1);
	free(arg_struct->pids);
	free(arg_struct);
	return (127);
}

static int	get_exit_code(void)
{
	if (errno == 13 || errno == 21)
		return (126);
	if (errno == 2)
		return (127);
	else
		return (errno);
}

static int	execute_command(char *path, char **command_array,
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
		free(arg_struct->pids);
		free(arg_struct);
		free_array((void **)command_array, 0, -1);
		return (126);
	}
	else
	{
		execve(path, command_array, arg_struct->envp);
		perror(command_array[0]);
	}
	free(arg_struct->pids);
	free(arg_struct);
	free_array((void **)command_array, 0, -1);
	return (get_exit_code());
}

static int	run_pid(t_args *arg_struct, t_cmd_parts *cmd_parts, t_shell *shell, int is_builtin)
{
	char	*path;
	char	**command_array;
	int		check_status;

	command_array = cmd_parts->cmd_array;
	path = find_command_path(command_array[0], arg_struct->envp);
	check_status = set_pipe(cmd_parts, arg_struct, path, is_builtin);
	if (check_status == -1)
	{
		free_array((void **)command_array, 0, -1);
		free(path);
		return (EXIT_FAILURE); // exit
	}
	if (is_builtin == 1)
		return (check_and_run_builtins(shell, cmd_parts, arg_struct->envp));
	else if (path != NULL)
		return (execute_command(path, command_array, arg_struct)); // exit
	else
		return (command_not_found(command_array, arg_struct)); // exit
}

int	execute_commands(t_shell *shell, int num_commands, char **envp)
{
	int		wstatus;
	int		j;
	t_args	*arg_struct;
	t_cmd_parts *cmd_parts;

	//printf("num_commands: %d\n", num_commands);
	arg_struct = prepare_struct(num_commands, envp);
	if (arg_struct == NULL)
		exit(EXIT_FAILURE);
	j = -1;
	while (++j < num_commands)
	{
		cmd_parts = get_command_array(shell->tokens, j + 1);
		cmd_parts->num_commands = num_commands;
		arg_struct->pids[j] = fork();
		if (arg_struct->pids[j] < 0)
			return (display_error_message(2, arg_struct)); // exit
		if (arg_struct->pids[j] == 0)
			run_pid(arg_struct, cmd_parts, shell, is_builtin(cmd_parts));
		free(cmd_parts);
	}
	close_and_free(arg_struct, 0);
	j = -1;
	while (++j < num_commands)
		waitpid(arg_struct->pids[j], &wstatus, 0);
	free(arg_struct->pids);
	free(arg_struct);
	if (WIFEXITED(wstatus))
		return (WEXITSTATUS(wstatus)); //exit
	return (EXIT_FAILURE); //exit
}


