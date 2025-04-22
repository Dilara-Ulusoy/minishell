/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variable.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:23:12 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/04/22 15:37:03 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"

/*
📌 get_var_name(line, index)
Purpose: Extracts a valid environment variable name starting after $.
Example:
If line = "$HOME" → get_var_name() returns "HOME", updating index
If line = "$123var", it returns NULL
Effect: Moves index forward to the character after the variable name
*/
char	*get_var_name(const char *line, int *index)
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

static void	copy_without_extra_spaces(const char *src, char *dst, char *base)
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

static char	*remove_spaces(const char *str)
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
	char	*normalized;
	char	*temp;

	var_value = ft_getenv(var_name, shell);
	temp = *result;
	if (var_value != NULL)
	{
		normalized = remove_spaces(var_value);
		*result = ft_strjoin(*result, normalized);
		free(normalized);
	}
	else
		*result = ft_strjoin(*result, "");
	free_this(temp, var_name, NULL, NULL);
	free(var_value);
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

static char	*append_remaining_text(const char *line, int *index, char *prefix)
{
	char	*result;
	char	*new_result;
	char	temp[2];

	result = prefix;
	while (line[*index] && (ft_isalnum(line[*index]) || line[*index] == '_'))
	{
		temp[0] = line[*index];
		temp[1] = '\0';
		new_result = ft_strjoin(result, temp);
		free(result);
		if (!new_result)
			return (NULL);
		result = new_result;
		(*index)++;
	}
	return (result);
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
		return (append_remaining_text(line, index, special));
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
