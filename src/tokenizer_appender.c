/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_appender.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoc@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 14:04:22 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/03/27 17:14:58 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
   append_token:
   - Adds 'new_token' to the end of the linked list whose head is '*head'.
   - If *head is NULL, then new_token becomes the first and only element.
*/
int	append_token(t_token **head, t_token *new_token)
{
	t_token	*token;

	if (!new_token)
		return (-1);
	if (!*head)
	{
		*head = new_token;
		return (1);
	}
	token = *head;
	while (token->next)
		token = token->next;
	token->next = new_token;
	return (1);
}

/*
   create_new_token_range:
   - Allocates and initializes a new token with a substring from the input line.
   - Copies the specified range
   (`startIndex` to `startIndex + length`) into the token's value.
   - Ensures proper memory allocation and null-terminates the value.
   - Returns a pointer to the newly created token or NULL on failure.
*/
t_token	*create_new_token_range(t_token_type type, const char *line,
		int startIndex, int op_length)
{
	t_token	*new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->type = type;
	new_token->value = (char *)malloc((op_length + 1) * sizeof(char));
	if (!new_token->value)
	{
		free_tokens(&new_token);
		return (NULL);
	}
	if (ft_memcpy(new_token->value, &line[startIndex], op_length + 1) == NULL)
	{
		free_tokens(&new_token);
		return (NULL);
	}
	new_token->value[op_length] = '\0';
	new_token->next = NULL;
	return (new_token);
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
