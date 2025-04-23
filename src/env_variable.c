/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variable.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoc@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:23:12 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/04/23 22:24:53 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

/*
📌 append_env_value(&result, var_name)

Purpose: Retrieves the actual value of the environment variable
and appends it to result.
Example:
If var_name = "HOME" and $HOME="/home/user", then result becomes "/home/user".

Effect: Expands environment variables in the parsed string.
*/
void	append_env_value(char **result, char *var_name, t_shell *shell)
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
void	append_dollar_if_no_var(char **result)
{
	char	*temp;

	temp = *result;
	*result = ft_strjoin(*result, "$");
	if (!*result)
		*result = ft_strdup("");
	free(temp);
}

char	*append_vars_from_line(const char *line,
		int *index, char *expanded, t_shell *shell)
{
	char	*env_var;
	char	*tmp;

	while (line[*index] == '$')
	{
		(*index)++;
		if (line[*index] == '?')
		{
			tmp = ft_strjoin(expanded, ft_itoa(shell->exit_code));
			free(expanded);
			expanded = tmp;
			(*index)++;
		}
		else
		{
			env_var = get_var_name(line, index);
			if (!env_var)
				append_dollar_if_no_var(&expanded);
			else
				append_env_value(&expanded, env_var, shell);
		}
	}
	return (expanded);
}

char	*expand_special_and_following_vars(const char *line,
	int *index, char *special_value, t_shell *shell)
{
	char	*expanded;

	expanded = ft_strdup(special_value);
	if (!expanded)
		return (NULL);
	return (append_vars_from_line(line, index, expanded, shell));
}

char	*get_env_var_value(const char *line, int *index, t_shell *shell)
{
	char	*special_value;
	char	*expanded;

	special_value = handle_special_cases(line, index, *index + 1, shell);
	if (special_value)
		return (expand_special_and_following_vars(line,
				index, special_value, shell));
	expanded = ft_calloc(1, sizeof(char));
	if (!expanded)
		return (NULL);
	return (append_vars_from_line(line, index, expanded, shell));
}
