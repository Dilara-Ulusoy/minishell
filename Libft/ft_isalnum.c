/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoce@student.hive.fi>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 12:22:14 by dakcakoc          #+#    #+#             */
/*   Updated: 2024/04/19 10:06:25 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isalnum(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
		|| (c >= '0' && c <= '9'))
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
        printf("%d", ft_isalnum(x[i]));
        i++;
    }
}
*/
