/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoc@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 14:04:08 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/01/30 14:04:10 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

int	skip_whitespace(const char *line, int i)
{
	while (line[i] && is_space(line[i]))
		i++;
	return (i);
}

int	is_space(char c)
{
	if(c == ' ' || c == '\t' || c == '\r' || c == '\v' || c == '\f')
		return (1);
	return (0);
}

int	is_two_char_operator(char c)
{
	if((c == '&' || c == '|' || c == '<' || c == '>'))
		return (1);
	return (0);
}

char	*allocate_word(const char *line, int start, int length)
{
	char	*word;

	word = (char *)malloc(length + 1);
	if (!word)
		return (NULL);
	ft_memcpy(word, &line[start], length);
	word[length] = '\0';
	return (word);
}
