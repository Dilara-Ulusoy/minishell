/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoce@student.hive.fi>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 10:23:57 by dakcakoc          #+#    #+#             */
/*   Updated: 2024/04/24 10:33:06 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			diff;
	size_t			i;
	unsigned char	*str1;
	unsigned char	*str2;

	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	i = 0;
	while (i < n)
	{
		diff = str1[i] - str2[i];
		if (diff != 0)
			return (diff);
		i++;
	}
	return (0);
}
/*
This function compares the first n bytes of the 
memory areas pointed to by s1 and s2.
Casting s1 and s2 to unsigned char * ensures type safety. 
It tells the compiler that we're treating the memory as a sequence 
of bytes and allows us to access the memory byte by byte 
without violating const-correctness.
*/
/*
#include <stdio.h>
#include <string.h>

 int main()
 {
     char x[]= "Hello";
     char y[]= "Apple";
     printf("My function: %d\n",ft_memcmp(x,y, 5));
     
    char a[]= "Hello";
     char b[]= "Apple";
     printf("Library function: %d",memcmp(a,b, 5)); 
 }
 */
