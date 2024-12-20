/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoce@student.hive.fi>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 13:51:37 by dakcakoc          #+#    #+#             */
/*   Updated: 2024/04/24 10:32:15 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*str;
	unsigned char		b;
	size_t				i;

	str = (const unsigned char *)s;
	b = (unsigned char)c;
	i = 0;
	while (i < n)
	{
		if (str[i] == b)
		{
			return ((void *)(&str[i]));
		}
		i++;
	}
	return (NULL);
}
/*
#include <stdio.h>
#include <string.h>
#include <stddef.h>
int main()
{
    char str[] = "My name is Dilara-";
    printf("My function: %s\n", ft_memchr(str, 'a',5));

    char str2[] = "My name is Dilara-";
    printf("Library function: %s", memchr(str2, 'a', 5));
    return 0;
}*/
/*
This function locates the first occurrence of c 
in the first n bytes of the string pointed to by s.
If c is found, the function returns memory address of index i.
Unsigned char represents a byte of memory. 
By casting to unsigned char, we can access the 
memory block byte by byte, which is necessary for 
searching for character (c) within the memory block.
By casting int to unsigned char, we ensure that 
we compare the data in the memory block properly 
because the range of unsigned char is 0 to 255, 
and all values are positive. 
This allows us to accurately compare each byte of 
the memory block with the character c
*/
