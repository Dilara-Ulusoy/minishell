/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 16:37:44 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/04/03 14:44:37 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*copy_after_equal(char *envp)
{
	int		i;
	char	*dest;

	i = 0;
	while (envp[i] && envp[i] != '=')
		i++;
	if (envp[i] == '=')
		i++;
	dest = ft_strdup(&envp[i]);
	return (dest);
}

char	*ft_getenv(char *var_name, t_shell *shell)
{
	int		i;
	char	*name_w_equal;
	char	*value;

	name_w_equal = ft_strjoin(var_name, "=");
	i = 0;
	while ((*(shell->envp))[i] != NULL && ft_strnstr((*(shell->envp))[i],
			name_w_equal, ft_strlen(name_w_equal)) == 0)
		i++;
	free(name_w_equal);
	if ((*(shell->envp))[i] == NULL)
		return (NULL);
	value = copy_after_equal((*(shell->envp))[i]);
	return (value);
}
