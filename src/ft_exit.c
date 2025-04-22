/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 15:31:04 by htopa             #+#    #+#             */
/*   Updated: 2025/04/22 20:45:45 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_number(char *str)
{
	int			i;
	char		*str_check;
	long long	n;

	i = -1;
	while (str[++i] != '\0')
	{
		if (i == 0 && (str[i] == '-' || str[i] == '+'))
			i++;
		if (ft_isdigit(str[i]) == 0)
			return (0);
	}
	n = ft_atoi_longlong(str);
	str_check = ft_itoa_longlong(n);
	if (str[0] == '+')
		str++;
	if (strncmp(str, str_check, ft_strlen(str)) != 0)
	{
		free(str_check);
		str_check = NULL;
		return (0);
	}
	free(str_check);
	str_check = NULL;
	return (1);
}

int	ft_exit(char *exit_code)
{
	int	exit_number;

	if (is_number(exit_code) == 0)
	{
		ft_putstr_fd("exit: ", 2);
		ft_putstr_fd(exit_code, 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		return (2);
	}
	exit_number = ft_atoi(exit_code);
	return (exit_number);
}

	// if (exit_number < 0)
	// 	exit_number = 156;
	// if (exit_number > 255)
	// 	exit_number = 42;