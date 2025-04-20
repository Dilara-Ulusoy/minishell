/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:17:23 by htopa             #+#    #+#             */
/*   Updated: 2025/04/20 20:31:52 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"

static int	try_to_go_folder(char *new_path)
{
	if (new_path == NULL)
	{
		ft_putstr_fd("Please specify path as an argument to cd.\n", 2);
		return (EXIT_FAILURE);
	}
	if (chdir(new_path) != 0)
	{
		perror(new_path);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

// ft_strjoin malloc
static int	try_getcwd(char ***envp)
{
	char	abs_new_path[PATH_MAX];
	int		i;

	if (getcwd(abs_new_path, sizeof(abs_new_path)) != NULL)
	{
		i = 0;
		while ((*envp)[i] != NULL && ft_strnstr((*envp)[i], "PWD=", 4) == 0)
			i++;
		free((*envp)[i]);
		(*envp)[i] = ft_strjoin("PWD=", abs_new_path);
	}
	else
	{
		ft_putstr_fd("getcwd() error.\n", 2);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

// ft_strjoin malloc
static int	set_pwd_and_oldpwd(char ***envp)
{
	char	*old_path;
	int		i;

	i = 0;
	while ((*envp)[i] != NULL && ft_strnstr((*envp)[i], "PWD=", 4) == 0)
		i++;
	old_path = ft_strjoin("OLD", (*envp)[i]);
	i = 0;
	while ((*envp)[i] != NULL && ft_strnstr((*envp)[i], "OLDPWD=", 7) == 0)
		i++;
	free((*envp)[i]);
	(*envp)[i] = old_path;
	return (try_getcwd(envp));
}

int	ft_cd(char *new_path, char ***envp)
{
	if (try_to_go_folder(new_path) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (set_pwd_and_oldpwd(envp));
}
