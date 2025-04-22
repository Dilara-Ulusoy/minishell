/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_num_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 13:23:14 by htopa             #+#    #+#             */
/*   Updated: 2025/04/20 14:41:19 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"

static void	count_pipes(const t_token *head, int *i)
{
	const t_token	*curr;

	if (!head)
		return ;
	curr = head;
	while (curr)
	{
		if (curr->type == TOKEN_PIPE)
			(*i)++;
		curr = curr->next;
	}
}

int	get_num_commands(const t_token *head)
{
	int	num_pipes;
	int	num_commands;

	num_pipes = 0;
	count_pipes(head, &num_pipes);
	num_commands = 1 + num_pipes;
	return (num_commands);
}
