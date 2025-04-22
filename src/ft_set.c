/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_set.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:05:28 by htopa             #+#    #+#             */
/*   Updated: 2025/04/20 21:55:58 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"

static int	check_export_var_name_is_valid(char *name_w_equal)
{
	int	i;

	i = 0;
	if (name_w_equal[i] == '=')
	{
		ft_putstr_fd("export: `", 2);
		ft_putchar_fd(name_w_equal[i], 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (0);
	}
	while (name_w_equal[i] != '=')
	{
		if ((i == 0 && ft_isdigit(name_w_equal[i]))
			|| !(ft_isalnum(name_w_equal[i]) || name_w_equal[i] == '_'))
		{
			ft_putstr_fd("export: `", 2);
			ft_putchar_fd(name_w_equal[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			return (0);
		}
		i++;
	}
	return (1);
}

static int	add_to_envp(char *name_w_equal, char *var_eq_value, char ***envp)
{
	int	i;

	i = 0;
	while ((*envp)[i] != NULL
		&& (ft_strnstr((*envp)[i], name_w_equal, ft_strlen(name_w_equal)) == 0
		&& ft_strcmp_wo_equal((*envp)[i], name_w_equal) == 0))
		i++;
	if ((*envp)[i] == NULL)
	{
		ft_add_env_var(var_eq_value, envp);
		free(name_w_equal);
		return (EXIT_SUCCESS);
	}
	if ((ft_strnstr((*envp)[i], name_w_equal, ft_strlen(name_w_equal)) != 0)
		&& (contains_equal(var_eq_value) == 0))
	{
		free(name_w_equal);
		return (EXIT_SUCCESS);
	}
	free(name_w_equal);
	free((*envp)[i]);
	(*envp)[i] = ft_strdup(var_eq_value);
	return (EXIT_SUCCESS);
}

int	ft_set(char *var_eq_value, char ***envp)
{
	char	*name_w_equal;
	int		ret;

	if (contains_equal(var_eq_value))
		name_w_equal = copy_until_equal(var_eq_value);
	else
		name_w_equal = ft_strjoin(var_eq_value, "=");
	ret = check_export_var_name_is_valid(name_w_equal);
	if (!ret)
	{
		free(name_w_equal);
		return (EXIT_FAILURE);
	}
	return (add_to_envp(name_w_equal, var_eq_value, envp));
}
