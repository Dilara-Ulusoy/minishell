/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_pipe_single_builtin.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 11:11:20 by htopa             #+#    #+#             */
/*   Updated: 2025/04/23 10:06:37 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	arrange_file_single_builtin(t_cmd_parts *cmd_parts, int i)
{
	int	fd;
	int	fileno;

	if (cmd_parts->files_types[i] == 0 || cmd_parts->files_types[i] == 3)
		fileno = STDIN_FILENO;
	if (cmd_parts->files_types[i] == 1 || cmd_parts->files_types[i] == 2)
		fileno = STDOUT_FILENO;
	if (cmd_parts->files_array[i][0] == '\0')
	{
		ft_putstr_fd("minishell: ambiguous redirect\n", 2);
		return (EXIT_FAILURE);
	}
	fd = open_and_check_file(cmd_parts->files_array[i],
			cmd_parts->files_types[i]);
	if (fd == -1)
		return (EXIT_FAILURE);
	if (dup2(fd, fileno) == -1)
	{
		close(fd);
		ft_putstr_fd("dup2() failed!\n", 2);
		return (EXIT_FAILURE);
	}
	close(fd);
	return (EXIT_SUCCESS);
}

static void	check_if_exit(char *str)
{
	if (ft_strncmp(str, "exit\0", 5) == 0)
		ft_putstr_fd("exit\n", 1);
}

int	set_pipe_single_builtin(t_cmd_parts *cmd_parts)
{
	int	i;
	int	ret;

	check_if_exit(cmd_parts->cmd_array[0]);
	if (cmd_parts->n_files > 0)
	{
		i = -1;
		while (++i < cmd_parts->n_files)
		{
			ret = arrange_file_single_builtin(cmd_parts, i);
			if (ret != EXIT_SUCCESS)
				return (ret);
		}
	}
	return (EXIT_SUCCESS);
}
