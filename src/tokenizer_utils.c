/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoc@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 14:04:08 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/02/10 12:26:10 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int skip_whitespace(const char *line, int i)
{
	while (line[i] && is_space(line[i]))
		i++;
	return (i);
}

int is_space(char c)
{
	if (c == ' ' || c == '\t' || c == '\r' || c == '\v' || c == '\f')
		return (1);
	return (0);
}

int is_two_char_operator(char c)
{
	if ((c == '&' || c == '|' || c == '<' || c == '>'))
		return (1);
	return (0);
}

void *ft_realloc(void *ptr, size_t size)
{
	void *new_ptr;

	new_ptr = malloc(size);
	if (!new_ptr)
		return (NULL);
	if (ptr)
	{
		ft_memcpy(new_ptr, ptr, size);
		free(ptr);
	}
	return (new_ptr);
}