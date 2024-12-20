/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoce@student.hive.fi>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 12:42:03 by dakcakoc          #+#    #+#             */
/*   Updated: 2024/04/16 12:42:12 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isprint(int c)
{
	if (c >= 32 && c <= 126)
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
int main()
{
    char x[] = "abctdst ";
    int i = 0;
    while (x[i] != '\0')
    {
        printf("%d", ft_isprint(x[i]));
        i++;
    }
}
*/
