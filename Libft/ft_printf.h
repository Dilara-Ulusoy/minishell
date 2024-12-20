/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoce@student.hive.fi>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 13:46:00 by dakcakoc          #+#    #+#             */
/*   Updated: 2024/05/27 13:48:28 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>

int	ft_printnbr(int nb);
int	ft_printchar(char c);
int	ft_printstr(char *str);
int	ft_printhex(unsigned long long n, int uppercase);
int	ft_printptr(void *p);
int	ft_printpercent(void);
int	ft_printunsigned(unsigned int n);
int	formats(va_list *args, const char format);
int	ft_printf(const char *s, ...);
int	ft_printnbr_recursive(int nb);

#endif
