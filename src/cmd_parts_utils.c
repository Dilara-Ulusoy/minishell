/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parts_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 14:37:37 by htopa             #+#    #+#             */
/*   Updated: 2025/04/22 20:44:27 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd_parts	*initialize_cmd_parts(void)
{
	t_cmd_parts		*cmd_parts;

	cmd_parts = malloc(sizeof(t_cmd_parts));
	if (cmd_parts == NULL)
		return (NULL);
	cmd_parts->n_cmd = 0;
	cmd_parts->n_in = 0;
	cmd_parts->n_out = 0;
	return (cmd_parts);
}

int	get_file_type(const t_token *curr)
{
	int	file_type;

	if (curr->type == TOKEN_REDIR_IN)
		file_type = 0;
	if (curr->type == TOKEN_REDIR_OUT)
		file_type = 1;
	if (curr->type == TOKEN_REDIR_APPEND)
		file_type = 2;
	if (curr->type == TOKEN_REDIR_HERE)
		file_type = 3;
	return (file_type);
}
