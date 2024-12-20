/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoce@student.hive.fi>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 11:30:16 by dakcakoc          #+#    #+#             */
/*   Updated: 2024/05/27 15:26:25 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

int	ft_printf(const char *s, ...)
{
	va_list	args;
	int		count;
	int		i;

	va_start (args, s);
	count = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] == '%')
		{
			count += formats(&args, s[++i]);
			if (count < 0)
				return (-1);
		}
		else
		{
			count += ft_printchar(s[i]);
			if (count < 0)
				return (-1);
		}
		i++;
	}
	va_end(args);
	return (count);
}

int	formats(va_list *args, const char format)
{
	int	count;

	count = 0;
	if (format == 'd' || format == 'i')
		count += ft_printnbr(va_arg(*args, int));
	else if (format == 'c')
		count += ft_printchar((char)va_arg(*args, int));
	else if (format == 's')
		count += ft_printstr(va_arg(*args, char *));
	else if (format == 'x')
		count += ft_printhex(va_arg(*args, unsigned int), 0);
	else if (format == 'X')
		count += ft_printhex(va_arg(*args, unsigned int), 1);
	else if (format == 'u')
		count += ft_printunsigned(va_arg(*args, unsigned int));
	else if (format == 'p')
		count += ft_printptr(va_arg(*args, void *));
	else if (format == '%')
		count += ft_printpercent();
	else
		return (-1);
	if (count < 0)
		return (-1);
	return (count);
}

int	ft_printchar(char c)
{
	int	result;

	result = write(1, &c, 1);
	if (result < 0)
		return (-1);
	return (1);
}

int	ft_printstr(char *str)
{
	int	count;
	int	result;

	count = 0;
	if (!str)
		str = "(null)";
	while (*str)
	{
		result = ft_printchar(*str);
		if (result < 0)
			return (-1);
		count += result;
		str++;
	}
	if (count < 0)
		return (-1);
	return (count);
}

int	ft_printpercent(void)
{
	int	count;

	count = 0;
	count = ft_printchar('%');
	if (count < 0)
		return (-1);
	return (count);
}
