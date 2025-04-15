/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoc@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:34:54 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/04/15 15:34:57 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_heredoc(t_parser *p, t_io_node **io_list,
			t_shell *shell, int *index)
{
	t_io_node	*new_io;

	if (handle_heredoc(&(p->current_token->value), shell, *index) == -1)
		return (-1);
	(*index)++;
	new_io = create_io_node(IO_HEREDOC, p->current_token->value, p);
	if (!new_io)
	{
		free_io_list(*io_list);
		return (-1);
	}
	if (attach_io_node(io_list, new_io) == -1)
		return (-1);
	return (0);
}
