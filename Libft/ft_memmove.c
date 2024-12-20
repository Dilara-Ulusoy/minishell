/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoce@student.hive.fi>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 10:36:18 by dakcakoc          #+#    #+#             */
/*   Updated: 2024/04/19 10:59:31 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char		*str1;
	const unsigned char	*str2;
	size_t				i;

	str1 = (unsigned char *)dst;
	str2 = (const unsigned char *)src;
	if (!dst && !src)
		return (NULL);
	if (str1 < str2)
	{
		i = 0;
		while (i < len)
		{
			str1[i] = str2[i];
			i++;
		}
	}
	else
	{
		while (len-- > 0)
		{
			str1[len] = str2[len];
		}
	}
	return (dst);
}
/*
This function copies len bytes from memory area src to memory area dest. 
*/
/*
#include <stdio.h>
#include <string.h>
int main() {
    char dest[] = "Hello World!";

    printf("Before ft_memmove string: %s\n", dest);
    ft_memmove(dest, dest+2 , 9);
    printf("After ft_memmove string: %s\n\n", dest);;
    
    char dest1[] = "Hello World!";
    
    printf("Before library memmove string: %s\n", dest1);
    memmove(dest1, dest1+2 , 9);
    printf("After library memmove string: %s\n\n", dest1);;

    return 0;
}
*/
