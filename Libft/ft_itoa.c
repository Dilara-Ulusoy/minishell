/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoc@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:12:00 by dakcakoc          #+#    #+#             */
/*   Updated: 2024/08/22 22:23:27 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	digit_count(int n)
{
	int	digit;

	digit = 0;
	if (n == 0)
		return (1);
	if (n < 0)
		digit += 1;
	while (n != 0)
	{
		n = n / 10;
		digit++;
	}
	return (digit);
}

static char	*min_int(char *result)
{
	if (!result)
		return (NULL);
	ft_memcpy(result, "-2147483648", 12);
	return (result);
}

char	*ft_itoa(int n)
{
	char	*result;
	int		d_count;

	d_count = digit_count(n);
	result = (char *)malloc((sizeof(char) * (d_count + 1)));
	if (!result)
		return (NULL);
	result[d_count] = '\0';
	if (n == -2147483648)
		return (min_int(result));
	if (n == 0)
		result[0] = '0';
	if (n < 0)
	{
		n = -n;
		result[0] = '-';
	}
	while (n != 0)
	{
		result[d_count - 1] = n % 10 + '0';
		n = n / 10;
		d_count--;
	}
	return (result);
}
/*
This function takes an integer as a parameter and
returns a string representation of the integer.
*/
/*
int main()
{
    int num = 12345;
    printf("itoa: %s\n", ft_itoa(num));
    return 0;
}
 */
