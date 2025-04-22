/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:38:26 by htopa             #+#    #+#             */
/*   Updated: 2025/04/22 20:48:01 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_helper	*initialize_helper(t_cmd_parts *cmd_parts, char *path,
	int is_builtin, int i)
{
	t_helper	*helper;

	helper = malloc(sizeof(t_helper));
	helper->is_builtin = is_builtin;
	helper->path = path;
	if (cmd_parts->files_types[i] == 0 || cmd_parts->files_types[i] == 3)
		helper->fileno = STDIN_FILENO;
	if (cmd_parts->files_types[i] == 1 || cmd_parts->files_types[i] == 2)
		helper->fileno = STDOUT_FILENO;
	return (helper);
}

void	free_helper(t_helper *helper)
{
	free(helper);
	helper = NULL;
}
