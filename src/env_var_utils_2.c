/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoc@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 23:57:37 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/04/24 14:17:38 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Append the numeric exit code and any following
 * alphanumeric characters from the input line
 * into the expanded string buffer.
 * Advances the index past the exit code and following text.
 *
 * Example:
 *   minishell$ echo $?dilara
 *   Output: 0dilara    (assuming exit code is 0)
 */

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

/*
 * Process and append one or more
 shell variables from the input line into expanded.
 * For each leading '$', advances index, then:
 *   - if '?' follows, calls append_exit_code_and_following
 *   - otherwise, extracts the variable name and
 * appends its value or a literal '$' if not found
 *
 * Example:
 *   minishell$ echo $HOME $?USER
 *   Output: /home/user 0USER   (assuming $HOME=/home/user and exit code is 0)
 */
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

/*
 * Initialize an empty string and delegate to append_vars_from_line to expand any
 * shell variables starting at the current index in line.
 *
 * Example:
 *   minishell$ echo $USER$?
 *   Output: alice0   (assuming USER=alice and exit code is 0)
 */
char	*get_env_var_value(const char *line, int *index, t_shell *shell)
{
	char	*expanded;
	char	*temp;

	expanded = ft_calloc(1, sizeof(char));
	temp = append_vars_from_line(line, index, expanded, shell);
	return (temp);
}
