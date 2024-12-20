/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoce@student.hive.fi>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 12:17:30 by dakcakoc          #+#    #+#             */
/*   Updated: 2024/04/16 12:17:40 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
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
    char x[] = "12345a";
    int i = 0;
    while (x[i] != '\0')
    {
        printf("%d", ft_isdigit(x[i]));
        i++;
    }
}
*/
