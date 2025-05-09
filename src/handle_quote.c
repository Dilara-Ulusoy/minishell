/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoc@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 18:18:52 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/04/23 14:39:16 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*check_unmatched_quote(t_parse_quote *p)
{
	if (p->quote != 0)
	{
		ft_putstr_fd("Error: Unmatched quote detected\n", STDERR_FILENO);
		free(p->result);
		return (NULL);
	}
	return (p->result);
}

/*
** This function checks the current character in the line and determines if a
** quote has started or ended. It helps track whether the parser is inside
** single or double quotes.
* Returns:
** - 1 if the quote status is updated (either opened or closed).
** - 0 if no change in quote status occurs.
*/
static int	update_quote_status(t_parse_quote *p)
{
	if (p->quote == 0)
	{
		p->quote = p->line[p->index];
		if (p->quote == '"')
			p->quote_is_double = 1;
		p->index++;
		if ((p->line[p->index]) == '"')
			p->quote_is_double = 1;
		return (1);
	}
	else if (p->quote == p->line[p->index])
	{
		p->quote = 0;
		p->index++;
		return (1);
	}
	return (0);
}

/*
** expand_env_variable - Expands an environment variable in the parsed line.
**
** This function handles the expansion of environment variables when a `$` is
** encountered within double quotes. It retrieves the corresponding variable
** value and appends it to the result buffer.
**
** Parameters:
** - t_parse_quote *p: A pointer to the parsing structure containing the line,
**   result buffer, and tracking indices.
**
** Returns:
** - -1 if memory allocation fails.
** - 0 on successful expansion or if no environment variable is found.
**
** Functionality:
** - Uses handle_dollar_sign() to extract the environment variable name and
**   retrieve its value.
** - Ensures the result buffer has enough space, reallocating if necessary.
** - Copies the expanded value into the result buffer and updates result_index.
** - Frees allocated memory for the environment variable string after copying.
*/
static int	expand_env_variable(t_parse_quote *p, t_shell *shell)
{
	char	*env;
	size_t	result_size;
	char	*new_result;

	result_size = ft_strlen(p->line) + 1;
	env = handle_dollar_sign(p->line, &p->index, p->index, shell);
	if (env)
	{
		while (p->result_index + ft_strlen(env) >= result_size)
		{
			result_size *= 2;
			new_result = ft_realloc(p->result, result_size);
			if (!new_result)
			{
				free(env);
				free(p->result);
				return (-1);
			}
			p->result = new_result;
		}
		ft_strlcpy(p->result + p->result_index, env, ft_strlen(env) + 1);
		p->result_index += ft_strlen(env);
		free(env);
	}
	return (0);
}
/*
** process_character - Handles the parsing of individual characters in a line.
**
** This function processes each character in the input line while tracking
** quotes and handling special cases like environment variable expansion.
**
** Parameters:
** - t_parse_quote *p: A pointer to the parsing structure containing quote
**   status, line data, and result buffer.
**
** Returns:
** - 1 if a space is found right after closing a quote
** (to manage token separation).
** - -1 if an error occurs during environment variable expansion.
** - 0 otherwise, indicating normal character processing.
**
** Functionality:
** - If the current character is a quote (single or double), it updates the
**   quote status using update_quote_status().
** - If a quote is closed and the next character is a space, it signals token
**   separation by returning 1.
** - If inside double quotes and a `$` is found, it attempts to expand an
**   environment variable.
** - Otherwise, it copies the character to
** the result buffer and moves to the next index.
*/

static int	process_character(t_parse_quote *p, t_shell *shell)
{
	char	current;

	current = p->line[p->index];
	if ((current == '"' || current == '\'') && update_quote_status(p))
	{
		if (p->quote == 0 && p->line[p->index] == ' ')
			return (1);
		return (0);
	}
	if (p->quote_is_double && current == '$')
	{
		if (has_digit_then_alpha(p->line, p->index))
			p->quote = 0;
		if (expand_env_variable(p, shell) == -1)
			return (-1);
		return (0);
	}
	if (current == ' ' && p->quote == 0)
		return (1);
	p->result[p->result_index++] = p->line[p->index++];
	return (0);
}

char	*process_quote_loop(t_parse_quote *p, t_shell *shell)
{
	int	status;

	while (p->line[p->index])
	{
		status = process_character(p, shell);
		if (status == 1)
			break ;
		if (status == -1)
		{
			free(p->result);
			return (NULL);
		}
	}
	return (p->result);
}
