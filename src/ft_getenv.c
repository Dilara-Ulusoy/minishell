/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 16:37:44 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/04/03 16:16:23 by htopa            ###   ########.fr       */
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

static void	free_str_array(char **arr, int limit)
{
	while (limit > 0)
		free(arr[--limit]);
	free(arr);
}

static int	copy_env_vars(char **new_envp, char **old_envp, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		new_envp[i] = ft_strdup(old_envp[i]);
		if (!new_envp[i])
		{
			perror("strdup failed");
			free_str_array(new_envp, i);
			return (0);
		}
		i++;
	}
	return (1);
}

void	ft_add_env_var(char *var_eq_value, char ***envp)
{
	int		count;
	char	**new_envp;

	if (!envp || !(*envp))
		return ;
	count = 0;
	while ((*envp)[count] != NULL)
		count++;
	new_envp = malloc(sizeof(char *) * (count + 2));
	if (!new_envp)
		return (perror("malloc failed"));
	if (!copy_env_vars(new_envp, *envp, count))
		return ;
	new_envp[count] = ft_strdup(var_eq_value);
	if (!new_envp[count])
	{
		perror("ft_strdup failed");
		free_str_array(new_envp, count);
		return ;
	}
	new_envp[count + 1] = NULL;
	free_envp(*envp);
	(*envp) = new_envp;
}
