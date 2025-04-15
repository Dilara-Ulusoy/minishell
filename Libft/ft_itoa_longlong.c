/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_longlong.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 10:12:00 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/04/15 15:17:30 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	digit_count(long long n)
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
	ft_memcpy(result, "-9223372036854775808", 20);
	return (result);
}

char	*ft_itoa_longlong(long long n)
{
	char	*result;
	int		d_count;

	d_count = digit_count(n);
	result = (char *)malloc((sizeof(char) * (d_count + 1)));
	if (!result)
		return (NULL);
	result[d_count] = '\0';
	if (n == (LLONG_MIN))
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
