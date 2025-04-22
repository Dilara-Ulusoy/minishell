/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 17:45:29 by htopa             #+#    #+#             */
/*   Updated: 2025/04/22 20:45:09 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	restore_fd(int *original_fd)
{
	dup2(original_fd[0], STDIN_FILENO);
	dup2(original_fd[1], STDOUT_FILENO);
	close(original_fd[0]);
	close(original_fd[1]);
}

static int	if_single_and_builtin(t_shell *shell, int num_commands,
	char ***envp)
{
	t_cmd_parts	*cmd_parts;
	int			original_fd[2];
	int			exit_code;

	original_fd[0] = dup(STDIN_FILENO);
	original_fd[1] = dup(STDOUT_FILENO);
	cmd_parts = get_command_array(shell->tokens, 1);
	cmd_parts->num_commands = num_commands;
	if (is_builtin(cmd_parts))
	{
		exit_code = set_pipe_single_builtin(cmd_parts);
		if (exit_code != 0)
		{
			restore_fd(original_fd);
			free_cmd_parts(&cmd_parts);
			return (exit_code);
		}
		exit_code = run_single_builtin(&cmd_parts, shell, envp, original_fd);
		restore_fd(original_fd);
		free_cmd_parts(&cmd_parts);
		return (exit_code);
	}
	restore_fd(original_fd);
	free_cmd_parts(&cmd_parts);
	return (42);
}

static int	run_command(t_shell *shell, int num_commands,
	t_args *arg_struct, int j)
{
	t_cmd_parts	*cmd_parts;
	int			exit_code;

	cmd_parts = get_command_array(shell->tokens, j + 1);
	cmd_parts->num_commands = num_commands;
	arg_struct->pids[j] = fork();
	if (arg_struct->pids[j] < 0)
	{
		free_cmd_clean_shell(shell, cmd_parts);
		exit(display_error_message(2, arg_struct));
	}
	if (arg_struct->pids[j] == 0)
	{
		set_signal_in_child();
		exit_code = run_pid(arg_struct, &cmd_parts, shell,
				is_builtin(cmd_parts));
		free_cmd_clean_shell(shell, cmd_parts);
		exit(exit_code);
	}
	set_signal_in_parent();
	free_cmd_parts(&cmd_parts);
	return (0);
}

static int	wait_commands(int num_commands, t_args *arg_struct)
{
	int	printed_newline;
	int	j;
	int	wstatus;

	printed_newline = 0;
	j = -1;
	while (++j < num_commands)
	{
		waitpid(arg_struct->pids[j], &wstatus, 0);
		if (WIFSIGNALED(wstatus) && WTERMSIG(wstatus) == SIGINT
			&& !printed_newline)
		{
			write(1, "\n", 1);
			printed_newline = 1;
		}
	}
	free(arg_struct->pids);
	free(arg_struct);
	if (WIFEXITED(wstatus))
		return (WEXITSTATUS(wstatus));
	else if (WIFSIGNALED(wstatus))
		return (128 + WTERMSIG(wstatus));
	return (EXIT_FAILURE);
}

int	execute_commands(t_shell *shell, int num_commands, char ***envp)
{
	int		exit_code;
	t_args	*arg_struct;
	int		j;

	if (num_commands == 1)
	{
		exit_code = if_single_and_builtin(shell, num_commands, envp);
		if (exit_code != 42)
		{
			cleanup_shell(shell);
			return (exit_code);
		}
	}
	arg_struct = prepare_struct(num_commands, *envp);
	if (arg_struct == NULL)
		exit(EXIT_FAILURE);
	j = -1;
	while (++j < num_commands)
		run_command(shell, num_commands, arg_struct, j);
	close_and_free(arg_struct, 0);
	return (wait_commands(num_commands, arg_struct));
}
