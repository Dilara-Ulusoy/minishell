/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:18:10 by htopa             #+#    #+#             */
/*   Updated: 2025/04/22 20:47:36 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Free if malloc fails
static char	**get_reduced_envp(int i, int len, char ***envp)
{
	char	**reduced_envp;
	int		j;

	reduced_envp = malloc(sizeof(char *) * (len));
	if (reduced_envp == NULL)
	{
		ft_putstr_fd("Malloc failed!\n", 2);
		return (NULL);
	}
	len = 0;
	j = 0;
	while ((*envp)[j] != NULL)
	{
		if (j == i)
		{
			free((*envp)[j]);
			j++;
			continue ;
		}
		reduced_envp[len] = (*envp)[j];
		len++;
		j++;
	}
	reduced_envp[len] = NULL;
	return (reduced_envp);
}

int	ft_unset(char *value, char ***envp)
{
	char	*name_w_equal;
	int		i;
	int		len;
	char	**reduced_envp;

	len = 0;
	while ((*envp)[len] != NULL)
		len++;
	name_w_equal = ft_strjoin(value, "=");
	i = 0;
	while ((*envp)[i] != NULL
		&& (ft_strnstr((*envp)[i], name_w_equal, ft_strlen(name_w_equal)) == 0
		&& ft_strcmp_wo_equal((*envp)[i], name_w_equal) == 0))
		i++;
	free(name_w_equal);
	if ((*envp)[i] == NULL)
		return (EXIT_SUCCESS);
	reduced_envp = get_reduced_envp(i, len, envp);
	if (reduced_envp == NULL)
		exit(EXIT_FAILURE);
	free(*envp);
	*envp = reduced_envp;
	return (EXIT_SUCCESS);
}
