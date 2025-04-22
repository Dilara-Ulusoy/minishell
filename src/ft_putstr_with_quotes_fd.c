/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_with_quotes_fd.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 12:55:30 by htopa             #+#    #+#             */
/*   Updated: 2025/04/22 19:41:13 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"

void	ft_putstr_with_quotes_fd(char *s, int fd)
{
	int	i;
	int	has_equal;

	has_equal = 0;
	i = 0;
	while (s[i] != '\0')
	{
		ft_putchar_fd(s[i], fd);
		if (s[i] == '=')
		{
			has_equal = 1;
			ft_putchar_fd('"', fd);
		}
		i++;
	}
	if (has_equal)
		ft_putchar_fd('"', fd);
}
