/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoce@student.hive.fi>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:32:10 by dakcakoc          #+#    #+#             */
/*   Updated: 2024/04/26 11:37:27 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*subs;
	unsigned int	i;
	unsigned int	slen;

	slen = ft_strlen(s);
	if (start >= slen)
		return (ft_strdup(""));
	if (len > slen - start)
		len = slen - start;
	subs = (char *)malloc(sizeof(char) * (len + 1));
	if (!subs)
		return (NULL);
	i = 0;
	while (i < len)
	{
		subs[i] = s[start + i];
		i++;
	}
	subs[i] = '\0';
	return (subs);
}
/*
This function finds the substring of a string by 
taking the starting index and the length of the substring.
Then allocates memory for the substring and returns it.
*/
/*
#include <stdlib.h>
#include <stdio.h>
int main()
{
  const char string[] = "This is Dilara";
  printf("%s", ft_substr(string,8, 4));
}*/
