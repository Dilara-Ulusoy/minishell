/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoce@student.hive.fi>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 12:13:06 by dakcakoc          #+#    #+#             */
/*   Updated: 2024/04/16 12:14:55 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isalpha(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
	{
		return (1);
	}
	else
	{
		return (0);
	}
}
/*
#include <stdio.h>
#include <ctype.h>
int main()
{
    char x[] = "abctdst";
    int i = 0;
    while (x[i] != '\0')
    {
        printf("%d", ft_isalpha(x[i]));
        i++;
    }
    printf("\n");
    char y[] = "abctdst";
    i = 0;
    while (y[i] != '\0')
    {
     printf("%d", isalpha(y[i]));
     i++;
    }
}
 */
