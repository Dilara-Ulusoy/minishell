/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoce@student.hive.fi>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 15:48:28 by dakcakoc          #+#    #+#             */
/*   Updated: 2024/04/24 15:50:38 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*result;
	unsigned int	i;
	unsigned int	len;

	len = ft_strlen(s);
	result = (char *)malloc(sizeof(char) * (len +1));
	if (!result)
		return (NULL);
	i = 0;
	while (i < len)
	{
		result[i] = (*f)(i, s[i]);
		i++;
	}
	result[i] = '\0';
	return (result);
}
/*
This function applies the function f to each character of 
the string and creates a new string with 
malloc resulting from the successive applications of f.
*/
/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

 char func(unsigned int i, char c)
 {
     return c - 32;
}

int main()
 {
     char str[10] = "dilara";
     printf("Result before my function applied: %s\n", str);
     char *result = ft_strmapi(str, func);
     printf("Result after my function applied: %s\n", result);
     free(result);
     return 0;
 }
*/
