/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoce@student.hive.fi>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 16:03:04 by dakcakoc          #+#    #+#             */
/*   Updated: 2024/04/22 10:11:35 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;
	size_t				i;

	if (!dest && !src)
		return (NULL);
	d = (unsigned char *)dest;
	s = (unsigned char *)src;
	i = 0;
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dest);
}
/*
This function copies n bytes from memory area src 
to memory area dest.
*/
/*
#include <stdio.h>
#include <string.h>
int main()
{
   //Test case for ft_memcpy
    char x[40];
   char y[] = "Hello Wolrd!";
   printf("Destination = %s\n", x);
   ft_memcpy(x, y, 5);
   printf("Destination after ft_memcpy =  %s\n\n",x);
    
    //Test case for memcpy
    char a[40];
    char b[] = "Hello Wolrd!";
    printf("Destination = %s\n", a);
    memcpy(a, b, 5);
    printf("Destination after memcpy =  %s\n",a);
   return 0;
}
*/
