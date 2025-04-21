/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_pipe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 11:13:19 by htopa             #+#    #+#             */
/*   Updated: 2025/04/21 11:16:18 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"

int	set_pipe(t_cmd_parts *cmd_parts, t_args *arg_struct, char *path, int is_builtin)
{
	int	i;
	int	fd;
	int	fileno;

	if (cmd_parts->n_files > 0)
	{
		i = -1;
		while (++i < cmd_parts->n_files)
		{
			if (cmd_parts->files_types[i] == 0 || cmd_parts->files_types[i] == 3)
				fileno = STDIN_FILENO;
			if (cmd_parts->files_types[i] == 1 || cmd_parts->files_types[i] == 2)
				fileno = STDOUT_FILENO;
			if (cmd_parts->files_array[i][0] == '\0')
			{
				ft_putstr_fd("minishell: ambiguous redirect\n", 2);
				free_cmd_parts(&cmd_parts);
				free(arg_struct->pids);
				free_array((void **)arg_struct->fd, 1, arg_struct->argc - 1);
				free_envp(arg_struct->envp);
				free(arg_struct);
				free(path);
				exit (EXIT_FAILURE);
			}
			if (cmd_parts->command_number == 0 && (cmd_parts->files_types[i] == 0 || cmd_parts->files_types[i] == 3))
			{
				fd = open_and_check_file(cmd_parts->files_array[i], 0);
				if (check_dup2(fd, arg_struct, fileno) == -1)
					return (-1);
				close(fd);
			}
			else if (cmd_parts->command_number != 0 && (cmd_parts->files_types[i] == 0 || cmd_parts->files_types[i] == 3))
			{
				arg_struct->fd[cmd_parts->command_number - 1][0] = open_and_check_file(cmd_parts->files_array[i], 0);
				if (check_dup2(arg_struct->fd[cmd_parts->command_number - 1][0], arg_struct, fileno) == -1)
					return (-1);
				close(arg_struct->fd[cmd_parts->command_number - 1][0]);
			}
			if (cmd_parts->command_number == ((cmd_parts->num_commands) - 1) && (cmd_parts->files_types[i] == 1 || cmd_parts->files_types[i] == 2))
			{
				fd = open_and_check_file(cmd_parts->files_array[i], cmd_parts->files_types[i]);
				if (fd == -1)
					return (close_and_free(arg_struct, 1));
				if (((is_builtin == 1) || (path != NULL)) && dup2(fd, fileno) == -1)
					return (display_error_message(3, arg_struct));
				close(fd);
			}
			else if (cmd_parts->command_number != ((cmd_parts->num_commands) - 1) && (cmd_parts->files_types[i] == 1 || cmd_parts->files_types[i] == 2))
			{
				arg_struct->fd[cmd_parts->command_number][1] = open_and_check_file(cmd_parts->files_array[i], cmd_parts->files_types[i]);
				if (arg_struct->fd[cmd_parts->command_number][1] == -1)
					return (close_and_free(arg_struct, 1));
				if (((is_builtin == 1) || (path != NULL)) && dup2(arg_struct->fd[cmd_parts->command_number][1], fileno) == -1)
					return (display_error_message(3, arg_struct));
				close(arg_struct->fd[cmd_parts->command_number][1]);
			}
		}
	}
	if (cmd_parts->n_in == 0 && cmd_parts->command_number != 0
		&& (dup2(arg_struct->fd[cmd_parts->command_number - 1][0], STDIN_FILENO) == -1))
		return (display_error_message(3, arg_struct));
	if (cmd_parts->n_out == 0 && cmd_parts->command_number != ((cmd_parts->num_commands) - 1) && ((path != NULL) || (is_builtin == 1))
		&& dup2(arg_struct->fd[cmd_parts->command_number][1], STDOUT_FILENO) == -1)
		return (display_error_message(3, arg_struct));
	return (close_and_free(arg_struct, 0));
}
