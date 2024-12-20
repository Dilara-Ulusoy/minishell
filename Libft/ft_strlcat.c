/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoce@student.hive.fi>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 10:37:58 by dakcakoc          #+#    #+#             */
/*   Updated: 2024/04/22 11:41:49 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	destlen;
	size_t	srclen;

	destlen = ft_strlen(dest);
	srclen = ft_strlen(src);
	i = 0;
	if (dstsize <= destlen)
	{
		return (srclen + dstsize);
	}
	while (src[i] && destlen + i + 1 < dstsize)
	{
		dest[destlen + i] = src[i];
		i++;
	}
	dest[destlen + i] = '\0';
	return (destlen + srclen);
}
/*
int main()
{
    char x[25] = "Hello";
    char y[] = "World";
    
    char a[25] = "Hello";
    char b[] = "World";
    size_t result_len1 = ft_strlcat(x, y, 2);
    size_t result_len2 = strlcat(a, b, 2);
    printf("Concatenated My string: %s\n", x); 
    printf("Concatenated Library string: %s\n", a); 
    printf("Total length my function: %zu\n", result_len1);
    printf("Total length library function: %zu\n", result_len2);
    return 0;
}
*/
