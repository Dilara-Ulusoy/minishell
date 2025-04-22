/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_word.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:14:48 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/04/22 15:09:50 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*handle_dollar_and_quote(const char *line, int *index,
		int start, t_shell *shell)
{
	char	*dollar;
	char	*temp;
	char	*joined;

	if (line[*index] == '$' && ft_strlen(line) == 1)
	{
		(*index)++;
		return (ft_strdup("$"));
	}
	dollar = handle_dollar_sign(line, index, start, shell);
	if (!dollar)
		return (NULL);
	if ((line[*index] == '"' || line[*index] == '\'' )
		&& (line[*index + 1] == '$' || ft_isalpha(line[*index + 1])))
	{
		temp = parse_quotes(line, index, shell);
		if (!temp)
			free_this(dollar, NULL, NULL, "parse_quotes failed");
		joined = ft_strjoin(dollar, temp);
		free(temp);
		free(dollar);
		if (!joined)
			return (NULL);
		return (joined);
	}
	return (dollar);
}

/**
 * read_word_range - Extracts a word from a given position in the input line.
 * It handles quoted words, environment variables, and operators.
 *
 * @line: The input string from which to extract the word.
 * @index: Pointer to the current position in the string
 * (updated during parsing).
 * @length: The total length of the input line.
 *
 * Return: A newly allocated string containing the extracted word.
 */
char	*read_word_range(const char *line, int *index,
		int length, t_shell *shell)
{
	int		start;
	int		wordlength;
	char	c;

	start = *index;
	while (*index < length)
	{
		c = line[*index];
		if ((c == '"' || c == '\''))
		{
			if (*index > 0 && !is_space(line[*index - 1]))
				return (join_string_with_quoted_if_no_space(line,
						index, start, shell));
			return (parse_quotes(line, index, shell));
		}
		if (c == '$')
			return (handle_dollar_and_quote(line, index, start, shell));
		if (is_space(c) || is_two_char_operator(c) || c == '(' || c == ')')
			break ;
		(*index)++;
	}
	wordlength = (*index) - start;
	if (wordlength == 0)
		return (NULL);
	return (ft_substr(line, start, wordlength));
}

char	*handle_env_variable_without_space(const char
	*line, int *index, int start, t_shell *shell)
{
	char	*temp;
	char	*temp2;
	char	*result;

	temp = ft_substr(line, start, (*index) - start);
	if (!temp)
		return (free_this(NULL, NULL, NULL, "substr failed"));
	temp2 = get_env_var_value(line, index, shell);
	if (!temp2)
		return (free_this(temp, NULL, NULL, "get_env_var_value failed"));
	if (temp2[0] == '\0')
	{
		while (line[*index] && is_space(line[*index]))
			(*index)++;
	}
	result = ft_strjoin(temp, temp2);
	if (!result)
		return (free_this(temp, temp2, NULL, "strjoin failed"));
	free_this(temp, temp2, NULL, NULL);
	return (result);
}

/*
Determines how to handle the dollar sign based on whether
there's a space before it.
If there is a space, we treat it as a standalone variable.
Otherwise, it's part of a more complex word (e.g., echo$VAR).
*/
static char	*fetch_env_result_based_on_space(const char *line, int *index,
		int start, t_shell *shell)
{
	if (*index > 0 && is_space(line[*index - 1]))
		return (get_env_var_value(line, index, shell));
	else
		return (handle_env_variable_without_space(line, index, start, shell));
}

/*
If the result of the env variable expansion is not empty,
and there are spaces following the variable, we want to preserve those spaces.
This function appends spaces one by one and moves the index forward.
*/
static char	*append_spaces_if_needed(const char *line, int *index, char *result)
{
	char	*temp;

	while (line[*index] && is_space(line[*index]))
	{
		temp = result;
		result = ft_strjoin(result, " ");
		free(temp);
		if (!result)
		{
			ft_putstr_fd("Memory error at handle dollar sign\n", STDERR_FILENO);
			return (NULL);
		}
		(*index)++;
	}
	return (result);
}

// Main function to handle `$` in a shell command line.
char	*handle_dollar_sign(const char *line, int *index,
		int start, t_shell *shell)
{
	char	*result;

	result = fetch_env_result_based_on_space(line, index, start, shell);
	if (!result)
	{
		ft_putstr_fd("Memory error at handling dollar sign\n", STDERR_FILENO);
		return (NULL);
	}
	if (result[0] != '\0')
	{
		result = append_spaces_if_needed(line, index, result);
		if (!result)
			return (NULL);
	}
	else
	{
		while (line[*index] && is_space(line[*index]))
			(*index)++;
	}
	return (result);
}
