/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoce@student.hive.fi>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 16:56:54 by dakcakoc          #+#    #+#             */
/*   Updated: 2024/04/25 16:57:49 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	i;

	i = 0;
	while (s[i] != '\0')
	{
		(*f)(i, &s[i]);
		i++;
	}
}
/*
Simply this function applies the function f to 
each character of the string passed as argument, 
and passing its index as first argument.
As a result, it modifies the string in place.
*/
/*
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>

void func(unsigned int i, char *c)
{
    *c = *c - 32;
}

 int main()
{
    char str[10] = "dilara";
    printf("Result before my function applied: %s\n", str);
    ft_striteri(str, &func);
    printf("Result after my function applied: %s\n", str);
    return 0;
}
*/
