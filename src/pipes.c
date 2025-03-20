/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 22:03:24 by htopa             #+#    #+#             */
/*   Updated: 2025/03/20 13:25:36 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"

void	close_pipes(int **fd, int j)
{
	int	i;

	i = -1;
	while (++i < j)
	{
		if (fd[i] != NULL)
		{
			if (fd[i][0] != -1)
				close(fd[i][0]);
			if (fd[i][1] != -1)
				close(fd[i][1]);
		}
	}
}

t_args	*add_fd_to_struct(t_args *arg_struct, int num_commands)
{
	int		j;

	arg_struct->fd = malloc(sizeof(int *) * (num_commands + 1));
	if (arg_struct->fd == NULL)
	{
		free(arg_struct->pids);
		free(arg_struct);
		return (NULL);
	}
	j = -1;
	while (++j < num_commands + 1)
	{
		arg_struct->fd[j] = malloc(2 * sizeof(int));
		if (arg_struct->fd[j] == NULL)
		{
			close_pipes(arg_struct->fd, j);
			free_array((void **)arg_struct->fd, 1, j);
			free(arg_struct->pids);
			free(arg_struct);
			return (NULL);
		}
	}
	return (arg_struct);
}

t_args	*initialize_struct_with_pids_and_fd(int num_commands)
{
	t_args	*arg_struct;

	arg_struct = malloc(sizeof(t_args));
	if (arg_struct == NULL)
		return (NULL);
	arg_struct->pids = malloc(sizeof(pid_t) * num_commands);
	if (arg_struct->pids == NULL)
	{
		free(arg_struct);
		return (NULL);
	}
	return (add_fd_to_struct(arg_struct, num_commands));
}

t_args	*prepare_struct(int num_commands, char **envp)
{
	int		j;
	t_args	*arg_struct;

	arg_struct = initialize_struct_with_pids_and_fd(num_commands);
	if (arg_struct == NULL)
	{
		display_error_message(4, arg_struct);
		return (NULL);
	}
	arg_struct->argc = num_commands;
	arg_struct->envp = envp;
	j = -1;
	while (++j < (num_commands + 1))
	{
		if (pipe(arg_struct->fd[j]) == -1)
		{
			close_pipes(arg_struct->fd, j);
			free_array((void **)arg_struct->fd, 1, num_commands + 1);
			free(arg_struct->pids); // Ensure proper cleanup
			free(arg_struct);
			display_error_message(1, arg_struct);
			return (NULL);
		}
	}
	return (arg_struct);
}

int	set_pipe(t_cmd_parts *cmd_parts, t_args *arg_struct, char *path, int is_builtin)
{
	int i;
	if (cmd_parts->command_number == 0)
		close(arg_struct->fd[cmd_parts->command_number][1]);
	if (cmd_parts->command_number == ((cmd_parts->num_commands) - 1))
		close(arg_struct->fd[(cmd_parts->command_number) + 1][0]);
	if (cmd_parts->n_in > 0)
	{
		i = -1;
		while (++i < cmd_parts->n_in)
		{
			arg_struct->fd[cmd_parts->command_number][0] = open_and_check_file(cmd_parts->infiles_array[i], 0);
			if (check_dup2(arg_struct->fd[cmd_parts->command_number][0], arg_struct, STDIN_FILENO) == -1)
				return (-1);
			close(arg_struct->fd[cmd_parts->command_number][0]);
		}
	}
	else if (cmd_parts->command_number != 0
		&& (dup2(arg_struct->fd[cmd_parts->command_number][0], STDIN_FILENO) == -1))
		return (display_error_message(3, arg_struct));
	if (cmd_parts->n_out > 0)
	{
		i = -1;
		while (++i < (cmd_parts->n_out))
		{
			arg_struct->fd[(cmd_parts->command_number) + 1][1] = open_and_check_file(cmd_parts->outfiles_array[i], cmd_parts->outfiles_types[i]);
			if (arg_struct->fd[(cmd_parts->command_number) + 1][1] == -1)
				return (close_and_free(arg_struct, 1));
			if (((is_builtin == 1) || (path != NULL)) && dup2(arg_struct->fd[(cmd_parts->command_number) + 1][1], STDOUT_FILENO) == -1)
				return (display_error_message(3, arg_struct));
			close(arg_struct->fd[(cmd_parts->command_number) + 1][1]);
		}
	}
	else if (cmd_parts->command_number != ((cmd_parts->num_commands) - 1) && ((path != NULL) || (is_builtin == 1))
		&& dup2(arg_struct->fd[(cmd_parts->command_number) + 1][1], STDOUT_FILENO) == -1)
		return (display_error_message(3, arg_struct));
	return (close_and_free(arg_struct, 0));
}
