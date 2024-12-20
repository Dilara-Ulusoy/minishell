/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoce@student.hive.fi>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 13:17:17 by dakcakoc          #+#    #+#             */
/*   Updated: 2024/04/17 13:18:00 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_toupper(int c)
{
	if (c >= 'a' && c <= 'z')
	{
		return (c - 32);
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
         printf("%c", toupper(x[i]));
         i++;
     }
     printf("\n");
     
     char y[]= "HEllo";
     i = 0;
     
     while(x[i] != '\0')
     {
         printf("%c", toupper(y[i]));
         i++;
     }
     printf("\n");
     
     char z[]= "HELLO";
     i = 0;
     
     while(x[i] != '\0')
     {
         printf("%c", toupper(z[i]));
         i++;
     }
 }
*/
