/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoc@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:56:13 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/04/23 14:11:40 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static void	handle_empty_quote_after_non_alnum(t_parse_quote *p,
	const char *line, int index)
{
	char	quote;

	if (!line || !line[index])
		return;
	quote = line[index];
	if ((quote != '\'' && quote != '"'))
		return;
	if (((index == 0) || (index > 0 && !ft_isalnum(line[index - 1])))
		&& line[index + 1] == quote
		&& (!line[index + 2] || !ft_isalnum(line[index + 2])))
	{
		free(p->result);
		p->result = ft_strdup(" ");
	}
}

char	*parse_quotes(const char *line, int *index, t_shell *shell)
{
	t_parse_quote	p;

	if (init_parse_quote(&p, line, index) == 1)
		return (NULL);
	if (!p.result)
		return (NULL);
	if (process_quote_loop(&p, shell) == NULL)
		return (NULL);
	if (p.quote != 0)
		return (check_unmatched_quote(&p));
	p.result[p.result_index] = '\0';
	handle_empty_quote_after_non_alnum(&p, line, *index);
	if (p.result[0] == '\0')
	{
		free(p.result);
		if (ft_strchr(line, '$') != NULL && ((*index > 0
					&& !ft_isalnum(line[*index - 1])) || (*index == 0)))
			p.result = ft_strdup(" ");
		else
			p.result = ft_strdup("");
		if (p.result == NULL)
			return (NULL);
	}
	*index = p.index;
	return (p.result);
}
