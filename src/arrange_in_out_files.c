/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arrange_in_out_files.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:38:49 by htopa             #+#    #+#             */
/*   Updated: 2025/04/24 15:08:34 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	arrange_in_file(t_cmd_parts *cmd_parts, t_args *arg_struct,
	int fileno, int i)
{
	int	fd;

	if (cmd_parts->command_number == 0
		&& (cmd_parts->files_types[i] == 0 || cmd_parts->files_types[i] == 3))
	{
		fd = open_and_check_file(cmd_parts->files_array[i], 0);
		if (check_dup2(fd, arg_struct, fileno) == -1)
			return (-1);
		close(fd);
	}
	else if (cmd_parts->command_number != 0
		&& (cmd_parts->files_types[i] == 0 || cmd_parts->files_types[i] == 3))
	{
		fd = open_and_check_file(cmd_parts->files_array[i], 0);
		if (check_dup2(fd, arg_struct, fileno) == -1)
			return (-1);
		close(fd);
		close(arg_struct->fd[cmd_parts->command_number - 1][0]);
	}
	return (EXIT_SUCCESS);
}

static int	run_for_last_command(t_cmd_parts *cmd_parts, t_args *arg_struct,
	t_helper *helper, int i)
{
	int	fd;

	fd = open_and_check_file(cmd_parts->files_array[i],
			cmd_parts->files_types[i]);
	if (fd == -1)
		return (close_and_free(arg_struct, 1));
	if (((helper->is_builtin == 1) || (helper->path != NULL))
		&& dup2(fd, helper->fileno) == -1)
	{
		close (fd);
		return (display_error_message(3, arg_struct));
	}
	close(fd);
	return (EXIT_SUCCESS);
}

static int	run_for_not_last_command(t_cmd_parts *cmd_parts, t_args *arg_struct,
	t_helper *helper, int i)
{
	int	fd;

	fd = open_and_check_file(cmd_parts->files_array[i],
			cmd_parts->files_types[i]);
	if (fd == -1)
		return (close_and_free(arg_struct, 1));
	if (((helper->is_builtin == 1) || (helper->path != NULL))
		&& dup2(fd, helper->fileno) == -1)
	{
		close(fd);
		return (display_error_message(3, arg_struct));
	}
	close(fd);
	close(arg_struct->fd[cmd_parts->command_number][1]);
	return (EXIT_SUCCESS);
}

int	arrange_out_file(t_cmd_parts *cmd_parts, t_args *arg_struct,
	t_helper *helper, int i)
{
	if (cmd_parts->command_number == ((cmd_parts->num_commands) - 1)
		&& (cmd_parts->files_types[i] == 1 || cmd_parts->files_types[i] == 2))
		return (run_for_last_command(cmd_parts, arg_struct, helper, i));
	else if (cmd_parts->command_number != ((cmd_parts->num_commands) - 1)
		&& (cmd_parts->files_types[i] == 1 || cmd_parts->files_types[i] == 2))
		return (run_for_not_last_command(cmd_parts, arg_struct, helper, i));
	return (EXIT_SUCCESS);
}
