/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoc@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 23:57:37 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/04/23 23:57:49 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			expanded = append_exit_code_and_following(line, index,
					expanded, shell->exit_code);
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
	return (append_vars_from_line(line, index, expanded, shell));
}
