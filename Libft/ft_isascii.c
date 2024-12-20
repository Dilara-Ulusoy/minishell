/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoce@student.hive.fi>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 12:35:01 by dakcakoc          #+#    #+#             */
/*   Updated: 2024/04/16 12:35:12 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isascii(int c)
{
	if (c >= 0 && c <= 127)
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
    char x[] = "abctdst";
    int i = 0;
    while (x[i] != '\0')
    {
        printf("%d", ft_isascii(x[i]));
        i++;
    }
}
*/
