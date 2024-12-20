/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoc@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 18:00:19 by dakcakoc          #+#    #+#             */
/*   Updated: 2024/08/13 22:11:47 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_strcpy(char *dest, const char *src)
{
	int	i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strdup(const char *s)
{
	int		i;
	char	*copy;

	i = 0;
	while (s[i])
		i++;
	copy = (char *)malloc(sizeof(char) * (i + 1));
	if (!copy)
		return (0);
	ft_strcpy(copy, s);
	return (copy);
}
/*
#include <string.h>
#include <stdio.h>
int main (void)
{
    char *str;
    char *str2;
    char *str3;
    str = "hi there";
    str2 = ft_strdup(str);
    str3 = strdup(str);
    printf("My function; %s\n", str2);
    printf("Library function; %s", str3);
}
*/
