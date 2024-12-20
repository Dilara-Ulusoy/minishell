/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoce@student.hive.fi>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 11:50:13 by dakcakoc          #+#    #+#             */
/*   Updated: 2024/05/27 12:42:19 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "ft_printf.h"

int	ft_printnbr_recursive(int nb)
{
	int	count;

	count = 0;
	if (nb > 9)
	{
		count += ft_printnbr_recursive(nb / 10);
		if (count < 0)
			return (-1);
		count += ft_printnbr_recursive(nb % 10);
	}
	else
	{
		count += ft_printchar(nb + '0');
		if (count < 0)
			return (-1);
	}
	return (count);
}

int	ft_printnbr(int nb)
{
	int	count;

	count = 0;
	if (nb == -2147483648)
	{
		count = ft_printstr("-2147483648");
		if (count < 0)
			return (-1);
	}
	else if (nb < 0)
	{
		count = ft_printchar('-');
		if (count < 0)
			return (-1);
		nb = -nb;
		count += ft_printnbr_recursive(nb);
	}
	else
	{
		count = ft_printnbr_recursive(nb);
		if (count < 0)
			return (-1);
	}
	return (count);
}

int	ft_printhex(unsigned long long n, int uppercase)
{
	int				count;
	unsigned int	digit;

	count = 0;
	if (n >= 16)
	{
		count += ft_printhex(n / 16, uppercase);
		if (count < 0)
			return (-1);
	}
	digit = n % 16;
	if (digit < 10)
		count += ft_printchar(digit + '0');
	else
	{
		if (uppercase)
			count += ft_printchar(digit - 10 + 'A');
		else
			count += ft_printchar(digit - 10 + 'a');
	}
	if (count < 0)
		return (-1);
	return (count);
}

int	ft_printptr(void *p)
{
	int	count;

	count = 0;
	count += ft_printstr("0x");
	if (count < 0)
		return (-1);
	count += ft_printhex((unsigned long long)p, 0);
	if (count < 0)
		return (-1);
	return (count);
}

int	ft_printunsigned(unsigned int n)
{
	int	count;

	count = 0;
	if (n < 0)
		return (-1);
	if (n >= 10)
	{
		count += ft_printunsigned(n / 10);
		if (count < 0)
			return (-1);
	}
	count += ft_printchar(n % 10 + '0');
	if (count < 0)
		return (-1);
	return (count);
}
