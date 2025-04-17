/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   special_cases.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoc@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 16:23:33 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/04/17 12:46:19 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
📌 handle_special_cases(line, index, start)
Purpose: Handles special cases where the $ sign appears in an unusual way.
Examples:
If the $ is surrounded by spaces (e.g., "echo $"), it returns "$".
If $ is followed by an invalid variable name
(e.g., "echo $123invalid"), it skips invalid characters and
returns an empty string ("").
Effect: Updates index accordingly and returns
a dynamically allocated string if a special case is found.
*/
// Case: If `$` is alone or surrounded by spaces, return "$"
static char	*handle_lonely_dollar(const char *line, int *index)
{
	if (*index > 0 && is_space(line[*index - 1])
		&& (line[*index + 1] == '\0' || is_space(line[*index + 1])))
	{
		(*index)++;
		return (ft_strdup("$"));
	}
	return (NULL);
}

// Case: If the special variable `$?` is used, return the shell's exit code
static char	*handle_exit_code(const char *line, int *index,
		int start, t_shell *shell)
{
	if (line[start] == '?')
	{
		(*index)++;
		(*index)++;
		return (ft_itoa(shell->exit_code));
	}
	return (NULL);
}

// Case: If invalid variable name (non-alpha and not `_`), skip invalid chars
static char	*handle_invalid_variable(const char *line, int *index, int start)
{
	int		size;

	size = ft_strlen(line) - (start + 1);
	if (*index > 0 && is_space(line[*index - 1])
		&& !ft_isalpha(line[start]) && line[start] != '_')
	{
		(*index)++;
		*index = ft_strlen(line);
		return (ft_substr(line, start + 1, size));
	}
	if (!ft_isalpha(line[start]) && line[start] != '_')
	{
		*index = ft_strlen(line);
		return (ft_substr(line, start + 1, size -1));
	}
	return (NULL);
}

char	*handle_special_cases(const char *line, int *index,
		int start, t_shell *shell)
{
	char	*result;

	result = handle_lonely_dollar(line, index);
	if (result)
		return (result);
	result = handle_exit_code(line, index, start, shell);
	if (result)
		return (result);
	result = handle_invalid_variable(line, index, start);
	if (result)
		return (result);
	return (NULL);
}
