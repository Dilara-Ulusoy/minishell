/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error_messages.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 12:58:25 by htopa             #+#    #+#             */
/*   Updated: 2025/04/22 20:48:37 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	else if (dup2(file_d, std_type) == -1)
		return (display_error_message(3, arg_struct));
	else
		return (0);
}

void	free_cmd_parts(t_cmd_parts **cmd_parts)
{
	int	i;

	if (cmd_parts == NULL || *cmd_parts == NULL)
		return ;
	free_cmd_array(cmd_parts);
	if ((*cmd_parts)->files_array != NULL)
	{
		i = -1;
		while ((*cmd_parts)->files_array[++i] != NULL)
			free((*cmd_parts)->files_array[i]);
		free((*cmd_parts)->files_array);
		(*cmd_parts)->files_array = NULL;
	}
	if ((*cmd_parts)->files_types != NULL)
	{
		free((*cmd_parts)->files_types);
		(*cmd_parts)->files_types = NULL;
	}
	free(*cmd_parts);
	*cmd_parts = NULL;
}

char	**copy_envp(char **envp)
{
	int		count;
	char	**new_envp;
	int		i;

	count = 0;
	while (envp[count] != NULL)
		count++;
	new_envp = malloc((count + 1) * sizeof(char *));
	if (!new_envp)
	{
		perror("malloc failed");
		return (NULL);
	}
	i = 0;
	while (i < count)
	{
		new_envp[i] = ft_strdup(envp[i]);
		if (!new_envp[i])
			return (free_before_return_3(new_envp, i));
		i++;
	}
	new_envp[count] = NULL;
	return (new_envp);
}

void	free_envp(char **envp)
{
	int	i;

	if (!envp)
		return ;
	i = 0;
	while (envp[i] != NULL)
	{
		free(envp[i]);
		i++;
	}
	free(envp[i]);
	free(envp);
}
