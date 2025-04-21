/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_pipe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 11:13:19 by htopa             #+#    #+#             */
/*   Updated: 2025/04/21 14:45:39 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"

static void	free_before_exit_set_pipe(t_cmd_parts *cmd_parts,
	t_args *arg_struct, char *path)
{
	free_cmd_parts(&cmd_parts);
	free(arg_struct->pids);
	free_array((void **)arg_struct->fd, 1, arg_struct->argc - 1);
	free_envp(arg_struct->envp);
	free(arg_struct);
	free(path);
}

static int	arrange_file(t_cmd_parts *cmd_parts, t_args *arg_struct,
	t_helper *helper, int i)
{
	int	ret;

	if (cmd_parts->files_array[i][0] == '\0')
	{
		ft_putstr_fd("minishell: ambiguous redirect\n", 2);
		free_before_exit_set_pipe(cmd_parts, arg_struct, helper->path);
		exit (EXIT_FAILURE);
	}
	ret = arrange_in_file(cmd_parts, arg_struct, helper->fileno, i);
	if (ret != EXIT_SUCCESS)
		return (ret);
	ret = arrange_out_file(cmd_parts, arg_struct, helper, i);
	if (ret != EXIT_SUCCESS)
		return (ret);
	return (EXIT_SUCCESS);
}

static int	handle_file(t_cmd_parts *cmd_parts, t_args *arg_struct,
	t_helper *helper, int i)
{
	int	ret;

	ret = arrange_file(cmd_parts, arg_struct, helper, i);
	if (ret != EXIT_SUCCESS)
	{
		free_helper(helper);
		return (ret);
	}
	free_helper(helper);
	return (EXIT_SUCCESS);
}

static int	check_when_no_out_files(t_cmd_parts *cmd_parts, t_args *arg_struct,
	char *path, int is_builtin)
{
	if (cmd_parts->n_out == 0
		&& cmd_parts->command_number != ((cmd_parts->num_commands) - 1)
		&& ((path != NULL) || (is_builtin == 1))
		&& dup2(arg_struct->fd[cmd_parts->command_number][1],
		STDOUT_FILENO) == -1)
		return (1);
	else
		return (0);
}

int	set_pipe(t_cmd_parts *cmd_parts, t_args *arg_struct,
	char *path, int is_builtin)
{
	int			i;
	int			ret;
	t_helper	*helper;

	if (cmd_parts->n_files > 0)
	{
		i = -1;
		while (++i < cmd_parts->n_files)
		{
			helper = initialize_helper(cmd_parts, path, is_builtin, i);
			ret = handle_file(cmd_parts, arg_struct, helper, i);
			if (ret != EXIT_SUCCESS)
				return (ret);
		}
	}
	if (cmd_parts->n_in == 0 && cmd_parts->command_number != 0
		&& (dup2(arg_struct->fd[cmd_parts->command_number - 1][0],
			STDIN_FILENO) == -1))
		return (display_error_message(3, arg_struct));
	if (check_when_no_out_files(cmd_parts, arg_struct, path, is_builtin) == 1)
		return (display_error_message(3, arg_struct));
	return (close_and_free(arg_struct, 0));
}
