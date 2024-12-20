/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoce@student.hive.fi>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 10:32:41 by dakcakoc          #+#    #+#             */
/*   Updated: 2024/04/22 11:43:58 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t			i;
	size_t			diff;
	unsigned char	*str1;
	unsigned char	*str2;

	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	i = 0;
	while (i < n)
	{
		diff = str1[i] - str2[i];
		if (diff != 0 || str1[i] == '\0')
			return (diff);
		i++;
	}
	return (0);
}
/*
#include <stdio.h>
#include <string.h>

 int main()
 {
     char x[]= "Hello";
     char y[]= "Apple";
     printf("My function: %d\n",ft_strncmp(x,y, 5));
     
    char a[]= "Hello";
     char b[]= "Apple";
     printf("Library function: %d",strncmp(a,b, 5));
     
 }
 */
