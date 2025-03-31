/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoc@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 12:57:34 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/03/31 16:41:23 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_redirections(t_parser *p, t_io_node **io_list, t_shell *shell)
{
	t_io_type	kind;
	int			index;

	index = 0;
	while (p->current_token && is_redirection(p->current_token->type))
	{
		kind = map_token_to_io_type(p->current_token->type);
		get_next_token(p);
		if (!p->current_token || p->current_token->type != TOKEN_WORD)
		{
			p->error_status = PARSE_SYNTAX_ERROR;
			return (-1);
		}
		if (kind == IO_HEREDOC)
		{
			if (process_heredoc(p, io_list, shell, &index) == -1)
				return (-1);
		}
		else
		{
			if (process_io(p, io_list, kind) == -1)
				return (-1);
		}
	}
	return (0);
}

/*
   map_token_to_io_type:
   - Convert a token type for redirection (<, >, >>, <<)
     into an i/o type (IO_IN, IO_OUT, IO_APPEND, IO_HEREDOC).
*/
t_io_type	map_token_to_io_type(t_token_type type)
{
	if (type == TOKEN_REDIR_IN)
		return (IO_IN);
	else if (type == TOKEN_REDIR_OUT)
		return (IO_OUT);
	else if (type == TOKEN_REDIR_APPEND)
		return (IO_APPEND);
	else if (type == TOKEN_REDIR_HERE)
		return (IO_HEREDOC);
	else
		return (IO_IN);
}

/* Helper function: Attach a new IO node to the IO list */
int	attach_io_node(t_io_node **io_list, t_io_node *new_io)
{
	t_io_node	*tmp;

	if (!new_io)
		return (-1);
	if (!*io_list)
		*io_list = new_io;
	else
	{
		tmp = *io_list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_io;
	}
	return (0);
}

void	get_next_token(t_parser *p)
{
	if (p->current_token)
		p->current_token = p->current_token->next;
}

int	is_redirection(t_token_type type)
{
	if (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_APPEND || type == TOKEN_REDIR_HERE)
		return (1);
	return (0);
}
