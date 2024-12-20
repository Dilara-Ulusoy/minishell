/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoc@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 10:49:53 by dakcakoc          #+#    #+#             */
/*   Updated: 2024/05/07 10:15:05 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static int	count_words(const char *s, char c)
{
	int	count;
	int	is_word;

	count = 0;
	is_word = 0;
	if (s == NULL)
		return (0);
	while (*s)
	{
		if (*s != c && !is_word)
		{
			is_word = 1;
			count++;
		}
		else if (*s == c)
			is_word = 0;
		s++;
	}
	return (count);
}

static int	find_word_len(char const *s, char c)
{
	if (ft_strchr(s, c))
		return (ft_strchr(s, c) - s);
	else
		return (ft_strlen(s));
}

static void	*free_list(char **list)
{
	int	i;

	i = 0;
	while (list[i])
		free(list[i++]);
	free(list);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	char	**list;
	int		word_len;
	int		i;

	list = (char **)malloc(sizeof(char *) * (count_words(s, c) + 1));
	if (!s || !list)
		return (NULL);
	i = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			word_len = find_word_len(s, c);
			list[i] = ft_substr(s, 0, word_len);
			i++;
			if (!list[i - 1])
				return (free_list(list));
			s += word_len;
		}
	}
	list[i] = NULL;
	return (list);
}
/*
This function splits the string s using the character c as a delimiter.
*/
/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
char *str = "Hello,World,This,Is,Sparta";
char **result = ft_split(str, ',');
int i = 0;
if (result == NULL)
{
printf("Memory allocation failed.\n");
return 1;
}
// Print the result
while (result[i])
{
printf("%s\n", result[i]);
free(result[i]); 
i++;
}
free(result);
return 0;
}*/
