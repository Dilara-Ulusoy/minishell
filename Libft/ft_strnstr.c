/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoce@student.hive.fi>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 11:25:53 by dakcakoc          #+#    #+#             */
/*   Updated: 2024/04/26 13:56:54 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (needle[0] == '\0')
		return ((char *)haystack);
	while (haystack[i] && i < len)
	{
		j = 0;
		while (haystack[i + j] == needle[j] && needle[j] && i + j < len)
			j++;
		if (needle[j] == '\0')
			return ((char *)(haystack + i));
		i++;
	}
	return (NULL);
}
/*
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    char str[] = "This is an apple!";
    char needle[]= "is an";
    printf("%s\n", ft_strnstr(str,needle,10));
     printf("%s\n", strnstr(str,needle,10));
    
    char *s1 = "MZIRIBMZIRIBMZE123";
     char *s2 = "MZIRIBMZEll";
     size_t max = strlen(s2);
     char *i1 = strnstr(s1, s2, max);
     printf("Library %s\n", i1);
    char *i2 = ft_strnstr(s1, s2, max);
     printf("My function %s", i2);
}
*/
