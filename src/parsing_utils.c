/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoc@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:56:35 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/02/19 14:57:32 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_operator(t_token_type type)
{
	if (type == TOKEN_AND || type == TOKEN_OR || type == TOKEN_PIPE
		|| type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_APPEND || type == TOKEN_REDIR_HERE)
		return (1);
	return (0);
}

/*
   is_binary_operator:
   - returns 1 if it's &&, ||, or |,
   - 0 otherwise
*/
int	is_binary_operator(t_token_type ttype)
{
	if (ttype == TOKEN_AND || ttype == TOKEN_OR || ttype == TOKEN_PIPE)
		return (1);
	return (0);
}
/*
   get_precedence:
   - a simple numeric precedence system
	 OR (||) -> 10
	 AND (&&) -> 20
	 PIPE (|) -> 30
   - higher => parse first
*/

int	get_precedence(t_token_type type)
{
	if (type == TOKEN_OR)
		return (10);
	else if (type == TOKEN_AND)
		return (20);
	else if (type == TOKEN_PIPE)
		return (30);
	else
		return (0);
}

char	*resize_buffer(char *buffer, size_t *buffer_size)
{
	size_t	new_size;
	char	*new_buffer;

	new_size = *buffer_size * 2;
	new_buffer = ft_calloc(new_size, 1);
	if (!new_buffer)
	{
		free(buffer);
		return (NULL);
	}
	ft_memcpy(new_buffer, buffer, *buffer_size);
	free(buffer);
	*buffer_size = new_size;
	return (new_buffer);
}
