/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoc@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 18:18:52 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/02/18 16:21:05 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*check_unmatched_quote(t_parse_quote *p)
{
	if (p->quote != 0)
	{
		ft_putstr_fd("Error: Unmatched quote detected\n", STDERR_FILENO);
		free(p->result);
		return (NULL);
	}
	return (p->result);
}

static int	update_quote_status(t_parse_quote *p)
{
	if (p->quote == 0)
	{
		p->quote = p->line[p->index];
		if (p->quote == '"')
			p->quote_is_double = 1;
		p->index++;
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

static int	expand_env_variable(t_parse_quote *p)
{
	char	*env;
	size_t	result_size;
	char	*new_result;

	result_size = ft_strlen(p->line) + 1;
	env = handle_dollar_sign(p->line, &p->index, p->index);
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
				return (1);
			}
			p->result = new_result;
		}
		ft_strlcpy(p->result + p->result_index, env, ft_strlen(env) + 1);
		p->result_index += ft_strlen(env);
		free(env);
	}
	return (0);
}

static int	process_character(t_parse_quote *p)
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
		if (expand_env_variable(p))
			return (-1);
		return (0);
	}
	p->result[p->result_index++] = p->line[p->index++];
	return (0);
}

/**
 * parse_quotes - Parses a string while handling quoted substrings.
 *
 * This function processes a given input string, handling both single ('') and
 * double ("") quotes. It ensures that quotes are properly matched, expands
 * environment variables if necessary, and constructs a processed output string.
 *
 * @line: The input string to be parsed.
 * @index: A pointer to an integer that keeps track of the current
 * position in the input string.
 *
 * Return: A newly allocated string with processed content, or NULL
 */
char	*parse_quotes(const char *line, int *index)
{
	t_parse_quote	p;
	int				status;

	if (init_parse_quote(&p, line, index) == 1)
		return (NULL);
	if (!p.result)
		return (NULL);
	while (p.line[p.index])
	{
		status = process_character(&p);
		// If a quote is closed and followed by a space, exit the loop
		if (status == 1)
			break ;
		// If an error occurred clean up and return NULL
		if (status == -1)
		{
			free(p.result);
			return (NULL);
		}
	}
	if (p.quote != 0)
		return (check_unmatched_quote(&p));
	p.result[p.result_index] = '\0';
	*index = p.index;
	return (p.result);
}
