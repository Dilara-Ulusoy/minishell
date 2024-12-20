/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoce@student.hive.fi>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:38:55 by dakcakoc          #+#    #+#             */
/*   Updated: 2024/04/22 11:35:17 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strchr(const char *s, int c)
{
	const unsigned char	*str;
	unsigned char		b;

	b = (unsigned char)c;
	str = (const unsigned char *)s;
	while (*str != '\0')
	{
		if (*str == b)
		{
			return ((char *)str);
		}
		str++;
	}
	if (b == '\0')
	{
		return ((char *)str);
	}
	return (0);
}
/*
#include <stdio.h>
#include <string.h>
int main()
{
    char str[] = "My name is Dilara-";
    printf("My function: %s\n", ft_strchr(str, 'a'));

    char str2[] = "My name is Dilara-";
    printf("Library function: %s", strchr(str2, 'a'));
    return 0;
}
*/
