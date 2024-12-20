/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoce@student.hive.fi>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 13:18:31 by dakcakoc          #+#    #+#             */
/*   Updated: 2024/04/17 13:19:06 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
	{
		return (c + 32);
	}
	else
	{
		return (c);
	}
}
/*
 #include <stdio.h>
 int main()
 {
     char x[]= "hello";
     int i = 0;
     
     while(x[i] != '\0')
     {
         printf("%c", tolower(x[i]));
         i++;
     }
     printf("\n");
     
     char y[]= "HEllo";
     i = 0;
     
     while(x[i] != '\0')
     {
         printf("%c", tolower(y[i]));
         i++;
     }
     printf("\n");
     
     char z[]= "HELLO";
     i = 0;
     
     while(x[i] != '\0')
     {
         printf("%c", tolower(z[i]));
         i++;
     }
 }
*/
