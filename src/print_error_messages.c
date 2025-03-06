/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error_messages.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 12:58:25 by htopa             #+#    #+#             */
/*   Updated: 2025/03/06 21:49:48 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"

int	display_error_message(int i, t_args *arg_struct)
{
	ft_putstr_fd("*************************\n", 2);
	if (i == 1)
	{
		ft_putstr_fd("pipe() failed!\n", 2);
		free(arg_struct->pids);
		free(arg_struct);
	}
	else if (i == 2)
	{
		ft_putstr_fd("fork() failed!\n", 2);
		return (close_and_free(arg_struct, 1));
	}
	else if (i == 3)
	{
		ft_putstr_fd("dup2() failed!\n", 2);
		return (close_and_free(arg_struct, 1));
	}
	else if (i == 4)
		ft_putstr_fd("mallloc() failed!\n", 2);
	ft_putstr_fd("*************************\n", 2);
	return (EXIT_FAILURE);
}

int	check_dup2(int file_d, t_args *arg_struct, int std_type)
{
	if (file_d == -1)
		return (close_and_free(arg_struct, 1));
	if (dup2(file_d, std_type) == -1)
		return (display_error_message(3, arg_struct));
	return (0);
}
