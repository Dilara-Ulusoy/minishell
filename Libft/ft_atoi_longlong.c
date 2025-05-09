/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_longlong.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 10:37:58 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/04/15 20:53:20 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_isspace(const char c)
{
	if (c == ' ' || (c >= '\t' && c <= '\r'))
		return (1);
	else
		return (0);
}

static long long	ft_atoi_main_longlong(char *str)
{
	long long	result;
	int			sign;

	result = 0;
	sign = 1;
	while (ft_isspace(*str))
		str++;
	if (*str == '-')
	{
		sign = sign * (-1);
		str++;
	}
	else if (*str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		if (sign == 1 && result < 0)
			return (-1);
		if (sign == -1 && result < 0)
			return (0);
		str++;
	}
	return ((long long)result * sign);
}

long long	ft_atoi_longlong(char *str)
{
	char	*itoa_str;

	itoa_str = ft_itoa_longlong(LLONG_MIN);
	if (ft_strncmp(str, itoa_str, ft_strlen(str)) == 0)
	{
		free (itoa_str);
		itoa_str = NULL;
		return (LLONG_MIN);
	}
	else
	{
		free (itoa_str);
		itoa_str = NULL;
		return (ft_atoi_main_longlong(str));
	}
}

/*
This function converts the initial portion of the string
pointed to by str to int representation.
If the result exceeds the range of the int type,
the function returns -1 or 0 depending on the sign.
Example number for out of range: 9223372036854775807
(max int value + 1), in this case the function returns -1.
Example number for out of range: -9223372036854775808
(min int value - 1), in this case the function returns 0.
int main() {
    char str[] = "-9223372036854775810";
    printf("%d\n", ft_atoi(str));
    printf("%d\n", atoi(str));
    return 0;
}*/
