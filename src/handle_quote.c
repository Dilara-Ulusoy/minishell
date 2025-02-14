/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoc@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 18:18:52 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/02/14 18:37:56 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*check_unmatched_quote(int quote, char *result)
{
	if (quote != 0)
	{
		ft_putstr_fd("Unmatched quote\n", STDERR_FILENO);
		free(result);
		return (NULL);
	}
	return (result);
}

static int	handle_quotes(const char *line, int *index, char *quote)
{
	if (*quote == 0)
	{
		*quote = line[*index];
		(*index)++;
		return (1);
	}
	else if (*quote == line[*index])
	{
		*quote = 0;
		(*index)++;
		return (1);
	}
	return (0);
}

static int	deal_evn(const char *line, int *index,
		int *result_index, char **result)
{
	char	*evn;
	int		env_len;
	int		result_size;

	result_size = ft_strlen(line) + 1;
	evn = handle_dollar_sign(line, (index), (*index));
	if (evn)
	{
		env_len = ft_strlen(evn);
		while (*result_index + env_len >= result_size)
		{
			result_size *= 2;
			*result = ft_realloc(*result, result_size);
			if (!result)
			{
				free(evn);
				return (1);
			}
		}
		ft_strlcpy(*result + (*result_index), evn, ft_strlen(evn) + 1);
		(*result_index) += ft_strlen(evn);
		free(evn);
	}
	return (0);
}

char	*parse_quotes(const char *line, int *index, char quote)
{
	int		result_index;
	char	*result;

	quote = 0;
	result_index = 0;
	result = ft_calloc((ft_strlen(line) + 1), sizeof(char));
	if (!result)
		return (NULL);
	while (line[(*index)] && line[(*index)] != ' ')
	{
		if ((line[(*index)] == '"' || line[(*index)] == '\'')
			&& handle_quotes(line, index, &quote) == 1)
			continue ;
		if (quote == '"' && line[(*index)] == '$'
			&& deal_evn(line, index, &result_index, &result) == 1)
		{
			free(result);
			return (NULL);
		}
		continue ;
		result[result_index++] = line[(*index++)];
	}
	return (check_unmatched_quote(quote, result));
	result[result_index] = '\0';
	return (result);
}