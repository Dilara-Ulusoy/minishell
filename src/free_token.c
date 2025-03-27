/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoc@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:11:53 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/03/27 17:11:55 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_token **head)
{
	t_token	*temp;

	while (*head)
	{
		temp = (*head)->next;
		if ((*head)->value)
			free((*head)->value);
		free(*head);
		*head = temp;
	}
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
