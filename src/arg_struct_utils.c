/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_struct_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 22:03:24 by htopa             #+#    #+#             */
/*   Updated: 2025/04/21 11:18:38 by htopa            ###   ########.fr       */
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

	arg_struct->fd = malloc(sizeof(int *) * (num_commands - 1));
	if (arg_struct->fd == NULL)
	{
		free(arg_struct->pids);
		free(arg_struct);
		return (NULL);
	}
	j = -1;
	while (++j < num_commands - 1)
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
	while (++j < (num_commands - 1))
	{
		if (pipe(arg_struct->fd[j]) == -1)
		{
			close_pipes(arg_struct->fd, j);
			free_array((void **)arg_struct->fd, 1, num_commands - 1);
			free(arg_struct->pids);
			free(arg_struct);
			display_error_message(1, arg_struct);
			return (NULL);
		}
	}
	return (arg_struct);
}
