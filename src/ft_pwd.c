/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 15:26:10 by htopa             #+#    #+#             */
/*   Updated: 2025/04/20 15:29:31 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"

int	ft_pwd(t_shell *shell)
{
	char	cwd[PATH_MAX];
	char	*env_var_value;

	env_var_value = ft_getenv("PWD", shell);
	if (env_var_value == NULL)
	{
		if (getcwd(cwd, sizeof(cwd)) != NULL)
		{
			printf("%s\n", cwd);
			return (EXIT_SUCCESS);
		}
		else
		{
			ft_putstr_fd("getcwd() error\n", 2);
			return (EXIT_FAILURE);
		}
	}
	else
	{
		printf("%s\n", env_var_value);
		free(env_var_value);
		env_var_value = NULL;
		return (EXIT_SUCCESS);
	}
}
