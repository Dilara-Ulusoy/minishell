/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoce@student.hive.fi>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 10:37:58 by dakcakoc          #+#    #+#             */
/*   Updated: 2024/04/22 11:47:26 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t dstsize)
{
	size_t	i;

	i = 0;
	if (dstsize == 0)
		return (ft_strlen(src));
	while (i < dstsize - 1 && src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (ft_strlen(src));
}
/*
This function copies up to dstsize - 1 characters 
from the string src to dest.
It will then null-terminate the string.
Return the total length of the string it tried to create.
*/
/*
#include <stddef.h>
#include <stdio.h>
#include <string.h>

int main()
{
    char x[25];
    char y[] = "Hello World!";
    printf("Before destination string: %s\n", x);
    int result_len = ft_strlcpy(x, y, 10);
    int result = strlen(x);
    printf("After destination string: %s\n", x);
    printf("Original source string lenght: %d\n", result_len);
    printf("Copied my string: %d\n\n", result);
    
    char a[25];
    char b[] = "Hello World!";
    printf("Before destination string: %s\n", a);
    int result_len1 = ft_strlcpy(a, b, 10);
    int result1 = strlen(x);
    printf("After destination string: %s\n", a);
    printf("Original source string lenght: %d\n", result_len1);
    printf("Copied my string: %d\n\n", result1);
}
 */
