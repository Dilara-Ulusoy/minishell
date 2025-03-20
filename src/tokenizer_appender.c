/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_appender.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoc@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 14:04:22 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/03/17 15:45:37 by dakcakoc         ###   ########.fr       */
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
		return NULL;
	}
	new_token->value[op_length] = '\0';
	new_token->next = NULL;
	return (new_token);
}
