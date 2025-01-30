/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_appender.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoc@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 14:04:22 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/01/30 14:04:36 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
   append_token:
   - Adds 'new_token' to the end of the linked list whose head is '*head'.
   - If *head is NULL, then new_token becomes the first and only element.
*/
void	append_token(t_token **head, t_token *new_token)
{
	t_token	*token;

	if (!new_token)
		return ;
	if (!*head)
	{
		*head = new_token;
		return ;
	}
	token = *head;
	while (token->next)
		token = token->next;
	token->next = new_token;
}

t_token	*create_new_token_range(t_token_type type, const char *line,
		int startIndex, int length)
{
	t_token	*new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->type = type;
	new_token->value = (char *)malloc((length + 1) * sizeof(char)); // 2. Allocate memory for the token's value string
	if (!new_token->value)
	{
		free_tokens(&new_token);
		return (NULL);
	}
	ft_memcpy(new_token->value, &line[startIndex], length + 1); // 3. Copy the substring from line into new_token->value
	new_token->value[length] = '\0';
	new_token->next = NULL; // 4. Initialize next pointer to NULL
	return (new_token);
}
