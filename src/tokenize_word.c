/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_word.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:14:48 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/03/26 12:16:38 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
char	*read_word_range(const char *line, int *index, int length, t_shell *shell)
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
			return (handle_dollar_sign(line, index, start, shell));
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
	result = ft_strjoin(temp, temp2);
	if (!result)
		return (free_this(temp, temp2, NULL, "strjoin failed"));
	free_this(temp, temp2, NULL, NULL);
	return (result);
}

char	*handle_dollar_sign(const char *line, int *index, int start, t_shell *shell)
{
	char	*result;

	if (*index > 0 && is_space(line[*index - 1]))
		result = get_env_var_value(line, index, shell);
	else
		result = handle_env_variable_without_space(line, index, start, shell);
	if (!result)
	{
		ft_putstr_fd("Memory error at handling dollar sign ", STDERR_FILENO);
		return (NULL);
	}
	while (line[*index] && is_space(line[*index]))
	{
		result = ft_strjoin(result, " ");
		if (!result)
		{
			ft_putstr_fd("Memory error at handle dollar sign\n", STDERR_FILENO);
			return (NULL);
		}
		(*index)++;
	}
	return (result);
}

/**
 * join_string_with_quoted_if_no_space - Concatenates a word with quoted content
 * if there is no space between them. Eg. echo"hello" -> echohello
 *
 * @line: The input string.
 * @index: Pointer to the current position in the string.
 * @start: The starting position of the word.
 * @quote: The type of quote (single or double).
 * @length: The total length of the input string.
 *
 * Return: A newly allocated string containing the concatenated word.
 */
char	*join_string_with_quoted_if_no_space(const char *line,
	int *index, int start, t_shell *shell)
{
	char	*result;
	char	*temp;
	char	*temp2;

	result = NULL;
	if (*index > 0 && !is_space(line[*index - 1]))
	{
		temp = ft_substr(line, start, (*index) - start);
		if (!temp)
			return (free_this(NULL, NULL, NULL, "substr failed"));
		temp2 = parse_quotes(line, index, shell);
		if (!temp2)
			return (free_this(temp, NULL, NULL, "temp2 failed"));
		result = ft_strjoin(temp, temp2);
		if (!result)
			return (free_this(temp, temp2, NULL, "strjoin failed"));
		free_this(temp, temp2, NULL, NULL);
	}
	return (result);
}

void	*free_this(char *s1, char *s2, char *s3, char *message)
{
	if (s1)
		free(s1);
	if (s2)
		free(s2);
	if (s3)
		free(s3);
	if (message)
		ft_putstr_fd(message, STDERR_FILENO);
	return (NULL);
}
