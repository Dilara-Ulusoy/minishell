/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variable.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoc@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:23:12 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/04/23 23:46:10 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
📌 append_and_free
Purpose: ft_strjoin + free işlemini tek adımda yapar.
*/
char	*append_and_free(char *original, const char *addition)
{
	char *joined;
	
	joined = ft_strjoin(original, addition);
	free(original);
	return (joined);
}

/*
📌 extract_alphanum
Purpose: Alfasayısal karakterleri bir substring olarak döner.
*/
char	*extract_alphanum(const char *line, int *index)
{
	int	start;

	start = *index;
	while (line[*index] && (ft_isalnum(line[*index]) || line[*index] == '_'))
		(*index)++;
	return (ft_substr(line, start, *index - start));
}

/*
📌 get_var_name
Extracts a valid environment variable name starting after $
*/
char	*get_var_name(const char *line, int *index)
{
	char	*var_name;

	var_name = extract_alphanum(line, index);
	return (var_name && var_name[0] ? var_name : NULL);
}

/*
📌 append_env_value
Appends env var value to result, or empty string if not found
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
	else
		*result = append_and_free(*result, "");
	free_this(NULL, var_name, NULL, NULL);
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

char	*append_exit_code_and_following(const char *line,
		int *index, char *expanded, int exit_code)
{
	char	*tmp;

	tmp = ft_itoa(exit_code);
	expanded = append_and_free(expanded, tmp);
	free(tmp);
	(*index)++;

	tmp = extract_alphanum(line, index);
	if (tmp)
	{
		expanded = append_and_free(expanded, tmp);
		free(tmp);
	}
	return (expanded);
}

char	*append_vars_from_line(const char *line,
		int *index, char *expanded, t_shell *shell)
{
	char	*env_var;

	while (line[*index] == '$')
	{
		(*index)++;
		if (line[*index] == '?')
			expanded = append_exit_code_and_following(line, index, expanded, shell->exit_code);
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


char	*get_env_var_value(const char *line, int *index, t_shell *shell)
{
	char	*expanded;

	expanded = ft_calloc(1, sizeof(char));
	if (!expanded)
		return (NULL);
	return append_vars_from_line(line, index, expanded, shell);
}
