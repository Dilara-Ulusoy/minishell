/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 20:00:31 by htopa             #+#    #+#             */
/*   Updated: 2025/04/22 20:45:31 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_before_return_1(t_args *arg_struct, char *path)
{
	free(path);
	free(arg_struct->pids);
	free_envp(arg_struct->envp);
	free(arg_struct);
}

void	free_before_return_2(t_args *arg_struct,
	t_cmd_parts **cmd_parts, char *path)
{
	if (path != (*cmd_parts)->cmd_array[0])
		free(path);
	free_cmd_parts(cmd_parts);
	free(arg_struct->pids);
	free_envp(arg_struct->envp);
	free(arg_struct);
}

char	**free_before_return_3(char **new_envp, int i)
{
	perror("strdup failed");
	while (i > 0)
		free(new_envp[--i]);
	free(new_envp);
	return (NULL);
}

void	free_cmd_array(t_cmd_parts **cmd_parts)
{
	int	i;

	if ((*cmd_parts)->cmd_array != NULL)
	{
		i = -1;
		while ((*cmd_parts)->cmd_array[++i] != NULL)
			free((*cmd_parts)->cmd_array[i]);
		free((*cmd_parts)->cmd_array);
		(*cmd_parts)->cmd_array = NULL;
	}
}
