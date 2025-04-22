/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:16:09 by htopa             #+#    #+#             */
/*   Updated: 2025/04/22 20:45:41 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i] != NULL)
	{
		if (contains_equal(envp[i]))
			printf("%s\n", envp[i]);
		i++;
	}
	return (0);
}
