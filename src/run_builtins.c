/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_builtins.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 23:15:20 by htopa             #+#    #+#             */
/*   Updated: 2025/04/22 20:48:42 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_unset(t_cmd_parts **cmd_parts, char ***envp)
{
	int	k;
	int	ret;

	k = 1;
	while ((*cmd_parts)->cmd_array[k] != NULL)
	{
		ret = ft_unset((*cmd_parts)->cmd_array[k], envp);
		if (ret == EXIT_FAILURE)
			return (ret);
		k++;
	}
	return (EXIT_SUCCESS);
}

int	run_export(t_cmd_parts **cmd_parts, char ***envp)
{
	int	k;
	int	exit_code;

	if ((*cmd_parts)->cmd_array[1] == NULL)
	{
		k = -1;
		while ((*envp)[++k] != NULL)
		{
			ft_putstr_fd("declare -x ", 1);
			ft_putstr_with_quotes_fd((*envp)[k], 1);
			ft_putstr_fd("\n", 1);
		}
	}
	else
	{
		k = 1;
		while ((*cmd_parts)->cmd_array[k] != NULL)
		{
			exit_code = ft_set((*cmd_parts)->cmd_array[k], envp);
			if (exit_code)
				return (exit_code);
			k++;
		}
	}
	return (EXIT_SUCCESS);
}

int	run_cd(t_cmd_parts **cmd_parts, char ***envp)
{
	int	exit_code;

	if ((*cmd_parts)->n_cmd > 2)
	{
		ft_putstr_fd("bash: cd: too many arguments\n", 2);
		exit_code = EXIT_FAILURE;
	}
	else
		exit_code = ft_cd((*cmd_parts)->cmd_array[1], envp);
	free_cmd_parts(cmd_parts);
	return (exit_code);
}

int	run_env(t_cmd_parts **cmd_parts, char ***envp)
{
	int	len;

	len = 0;
	while ((*cmd_parts)->cmd_array[len] != NULL)
		len++;
	if (len > 1)
	{
		ft_putstr_fd(
			"env currently does not accept any options or arguments\n", 2);
		return (EXIT_FAILURE);
	}
	else
		ft_env(*envp);
	free_cmd_parts(cmd_parts);
	return (0);
}

int	run_echo(t_cmd_parts **cmd_parts)
{
	int	k;

	k = 1;
	while (((*cmd_parts)->cmd_array[k])
		&& (check_echo_option((*cmd_parts)->cmd_array[k]) == 1))
		k++;
	while ((*cmd_parts)->cmd_array[k] != NULL)
	{
		ft_putstr_fd((*cmd_parts)->cmd_array[k], STDOUT_FILENO);
		if ((*cmd_parts)->cmd_array[k + 1] != NULL)
			ft_putstr_fd(" ", STDOUT_FILENO);
		k++;
	}
	if (((*cmd_parts)->cmd_array[1] == NULL) || (((*cmd_parts)->cmd_array[1])
			&& (check_echo_option((*cmd_parts)->cmd_array[1]) == 0)))
		ft_putstr_fd("\n", STDOUT_FILENO);
	free_cmd_parts(cmd_parts);
	return (0);
}
