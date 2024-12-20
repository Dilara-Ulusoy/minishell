/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoc@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 13:20:30 by dakcakoc          #+#    #+#             */
/*   Updated: 2024/08/22 22:24:10 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*str;
	size_t			i;

	str = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		str[i] = '\0';
		i++;
	}
}
/*
This function writes first n bytes of the memory
area pointed to by s with constant byte 0.
Simply, we can say that this function erases
the data in the memory area pointed to by s with the size of n bytes.
I casted to pointer s to unsigned char pointer
because the data in the memory area is in byte format.
But void pointer does not have a size.
*/
/*
#include <strings.h>
#include <stdio.h>
int main()
{
   char x[] = "Hello";
   printf("Before ft_bzero: %s\n", x);
   ft_bzero(x, 5);
   bzero(x, 5);
   printf("After ft_bzero: %s\n", x); // Print x after erasing first 5 bytes
   return 0; // Return from main function
}
*/
