/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:58:06 by htopa             #+#    #+#             */
/*   Updated: 2025/04/22 17:01:51 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	copy_without_extra_spaces(const char *src, char *dst, char *base)
{
	int		in_space;

	in_space = 0;
	while (*src)
	{
		if (*src != ' ' && *src != '\t')
		{
			if (in_space && dst != base)
				*dst++ = ' ';
			*dst++ = *src;
			in_space = 0;
		}
		else
			in_space = 1;
		src++;
	}
	*dst = '\0';
}

char	*remove_spaces(const char *str)
{
	char	*res;

	if (!str)
		return (NULL);
	res = malloc(ft_strlen(str) + 1);
	if (!res)
		return (NULL);
	copy_without_extra_spaces(str, res, res);
	return (res);
}

int	has_digit_then_alpha(const char *line, int index)
{
	int	i;

	if (!line || !ft_isdigit(line[index + 1]))
		return (0);
	i = index + 1;
	while (ft_isdigit(line[i]))
	{
		i++;
		if (ft_isalpha(line[i]))
			return (1);
	}
	return (1);
}
/*
📌 build_command_string(p)
Purpose: Constructs a single command string by
concatenating consecutive WORD tokens.

Steps:
1) Initializes a dynamic buffer (`cmd_buffer`) with an initial size of 256 bytes.
2) Iterates through `TOKEN_WORD` tokens, appending their values to `cmd_buffer`.
3) Expands the buffer if needed using `append_to_buffer()`.
4) Stops when a non-WORD token is encountered.
5) Returns the built command string or NULL on memory allocation failure.

Example:
- Tokens: [ WORD("echo"), WORD("hello"), WORD("world") ]
- Returns: `"echo hello world"`

Effect:
- Moves `p->current_token` forward past the processed WORD tokens.
- If memory allocation fails,
sets `p->error_status = PARSE_MEMORY_ERROR` and returns NULL.
*/

char	*build_command_string(t_parser *p)
{
	t_buffer	cmd_buffer;

	cmd_buffer.size = 256;
	cmd_buffer.pos = 0;
	cmd_buffer.data = ft_calloc(cmd_buffer.size, 1);
	if (!cmd_buffer.data)
	{
		p->error_status = PARSE_MEMORY_ERROR;
		return (NULL);
	}
	while (p->current_token && p->current_token->type == TOKEN_WORD)
	{
		if (!append_to_buffer(&cmd_buffer, p->current_token->value))
		{
			p->error_status = PARSE_MEMORY_ERROR;
			free(cmd_buffer.data);
			return (NULL);
		}
		get_next_token(p);
	}
	return (cmd_buffer.data);
}
