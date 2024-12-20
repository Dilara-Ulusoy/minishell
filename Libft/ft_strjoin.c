/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoce@student.hive.fi>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 11:26:30 by dakcakoc          #+#    #+#             */
/*   Updated: 2024/04/22 11:37:09 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strcat(char *dest, const char *src)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (dest[i] != '\0')
	{
		i++;
	}
	while (src[j] != '\0')
	{
		dest[i] = src[j];
		i++;
		j++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		totallen;
	char	*output;

	totallen = ft_strlen(s1) + ft_strlen(s2) + 1;
	output = (char *)malloc(totallen);
	if (output == NULL)
		return (NULL);
	output[0] = '\0';
	ft_strcat(output, s1);
	ft_strcat(output, s2);
	return (output);
}
/*
#include <stdio.h>
int main() {
    char *strs[] = {"Apple", "Orange", "Plum", "Banana"};
    char *sep = "|";
    char *result = ft_strjoin(strs, sep);
    printf("%s\n", result);
    free(result);
    return 0;
}*/
