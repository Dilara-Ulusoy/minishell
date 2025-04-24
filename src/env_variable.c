/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variable.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoc@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:23:12 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/04/24 14:16:12 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Join two strings and free the original string.
 * Used to extend a dynamically allocated string with new content.
 *
 * @param original The original string to append to (will be freed)
 * @param addition The string to append
 * @return         A newly allocated string combining both inputs
 */
char	*append_and_free(char *original, const char *addition)
{
	char	*joined;

	joined = ft_strjoin(original, addition);
	free(original);
	return (joined);
}

char	*extract_alphanum(const char *line, int *index)
{
	int	start;

	start = *index;
	while (line[*index] && (ft_isalnum(line[*index]) || line[*index] == '_'))
		(*index)++;
	return (ft_substr(line, start, *index - start));
}

char	*get_var_name(const char *line, int *index)
{
	char	*var_name;

	var_name = extract_alphanum(line, index);
	if (var_name && var_name[0])
		return (var_name);
	else
	{
		free(var_name);
		return (NULL);
	}
}

/*
 * Appends the value of an environment variable to the result string.
 * If the variable is not found, appends an empty string or a modified version.
 *
 * - Removes spaces from value before appending.
 * - If variable starts with digit (e.g., $1), appends part of name directly.
 *
 * Example:
 *   var_name = "HOME", value = "/home/user"
 *   result becomes ".../home/user"
 */
void	append_env_value(char **result, char *var_name, t_shell *shell)
{
	char	*var_value;
	char	*normalized;

	var_value = ft_getenv(var_name, shell);
	if (var_value)
	{
		normalized = remove_spaces(var_value);
		*result = append_and_free(*result, normalized);
		free(normalized);
	}
	else if (ft_isdigit(var_name[0]))
	{
		free(*result);
		*result = ft_strdup(var_name + 1);
	}
	else
		*result = append_and_free(*result, "");
	free(var_name);
	free(var_value);
}

/*
📌 append_dollar_if_no_var
Appends "$" when no valid variable follows
*/
void	append_dollar_if_no_var(char **result)
{
	*result = append_and_free(*result, "$");
}
