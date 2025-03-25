/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variable.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:23:12 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/03/25 13:38:52 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"

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
static char	*handle_special_cases(const char *line, int *index, int start, t_shell *shell)
{
	char	*result;

	if (is_space(line[*index - 1]) && (line[*index + 1] == '\0'
			|| is_space(line[*index + 1])))
	{
		(*index)++;
		result = ft_strdup("$");
		if (!result)
			return (NULL);
		return (result);
	}
	if (line[start] == '?')
	{
		(*index)++;
		result = ft_itoa(shell->exit_code);
		(*index)++;
		if (!result)
			return (NULL);
		return (result);
	}
	if (is_space(line[*index - 1]) && !ft_isalpha(line[start])
		&& line[start] != '_')
	{
		(*index)++;
		while (!ft_isalpha(line[*index]) && line[*index] != '_'
			&& line[*index] != '\0')
			(*index)++;
		result = ft_strdup("");
		if (!result)
			return (NULL);
		return (result);
	}
	return (NULL);
}

/*
📌 get_var_name(line, index)
Purpose: Extracts a valid environment variable name starting after $.
Example:
If line = "$HOME" → get_var_name() returns "HOME", updating index
If line = "$123var", it returns NULL
Effect: Moves index forward to the character after the variable name
*/
static char	*get_var_name(const char *line, int *index)
{
	int		start;
	int		len;
	char	*var_name;

	start = *index;
	len = 0;
	while (ft_isalpha(line[start + len]) || line[start + len] == '_'
		|| ft_isdigit(line[start + len]))
		len++;
	if (len == 0)
		return (NULL);
	*index += len;
	var_name = ft_substr(line, start, len);
	if (!var_name)
		return (NULL);
	return (var_name);
}

/*
📌 append_env_value(&result, var_name)

Purpose: Retrieves the actual value of the environment variable
and appends it to result.
Example:
If var_name = "HOME" and $HOME="/home/user", then result becomes "/home/user".

Effect: Expands environment variables in the parsed string.
*/
static void	append_env_value(char **result, char *var_name, t_shell *shell)
{
	char	*var_value;
	char	*temp;

	var_value = ft_getenv(var_name, shell);
	temp = *result;
	if (var_value != NULL)
		*result = ft_strjoin(*result, var_value);
	else
		*result = ft_strjoin(*result, "");
	free_this(temp, var_name, NULL, NULL);
}

/*
📌 append_dollar_if_no_var(&result)

Purpose: If no valid variable name is found, appends "$" to result.
Example:
Input: "echo $123invalid"

If get_var_name() fails (returns NULL), result is updated to " $".
Effect: Ensures $ signs remain in the output
when they are not part of valid variable names.
*/
static void	append_dollar_if_no_var(char **result)
{
	char	*temp;

	temp = *result;
	*result = ft_strjoin(*result, "$");
	if (!*result)
		*result = ft_strdup("");
	free(temp);
}

/*
   get_env_var_value:

- Extracts and expands an environment variable from the input string.
- Handles special cases (e.g., isolated `$` or invalid variable names).
- Allocates memory for the resulting expanded value using `ft_calloc()`.
- Iterates through consecutive `$` signs, retrives and appends variable values.
- Returns a dynamically allocated string containing the processed variable value,
or NULL if memory allocation fails.
*/
char	*get_env_var_value(const char *line, int *index, t_shell *shell)
{
	char	*result;
	char	*special;
	char	*var_name;
	int		start;

	start = *index + 1;
	special = handle_special_cases(line, index, start, shell);
	if (special)
		return (special);
	result = ft_calloc(1, sizeof(char));
	if (!result)
		return (NULL);
	while (line[*index] == '$')
	{
		(*index)++;
		var_name = get_var_name(line, index);
		if (!var_name)
			append_dollar_if_no_var(&result);
		else
			append_env_value(&result, var_name, shell);
	}
	return (result);
}
