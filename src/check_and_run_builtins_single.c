/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_and_run_builtins_single.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 23:45:55 by htopa             #+#    #+#             */
/*   Updated: 2025/04/22 20:44:19 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_before_exit_single(t_shell *shell, t_cmd_parts **cmd_parts,
	char ***envp, int *original_fd)
{
	free_cmd_parts(cmd_parts);
	free_envp(*envp);
	restore_fd(original_fd);
	cleanup_shell(shell);
}

static int	exit_w_more_than_two_args_single(t_shell *shell,
	t_cmd_parts **cmd_parts, char ***envp, int *original_fd)
{
	if (is_number((*cmd_parts)->cmd_array[1]))
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		free_cmd_parts(cmd_parts);
		return (EXIT_FAILURE);
	}
	else
	{
		ft_putstr_fd("exit: ", 2);
		ft_putstr_fd((*cmd_parts)->cmd_array[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		free_before_exit_single(shell, cmd_parts, envp, original_fd);
		exit(2);
	}
}

static int	run_exit_single(t_shell *shell, t_cmd_parts **cmd_parts,
	char ***envp, int *original_fd)
{
	int	exit_code;
	int	len;

	len = 0;
	while ((*cmd_parts)->cmd_array[len] != NULL)
		len++;
	if (len == 1)
	{
		free_before_exit_single(shell, cmd_parts, envp, original_fd);
		exit(EXIT_SUCCESS);
	}
	else if (len == 2)
	{
		exit_code = ft_exit((*cmd_parts)->cmd_array[1]);
		free_before_exit_single(shell, cmd_parts, envp, original_fd);
		exit(exit_code);
	}
	else
	{
		exit_code = exit_w_more_than_two_args_single(shell, cmd_parts,
				envp, original_fd);
		return (exit_code);
	}
}

int	check_and_run_builtins_single(t_shell *shell, t_cmd_parts **cmd_parts,
	char ***envp, int *original_fd)
{
	if (ft_strncmp((*cmd_parts)->cmd_array[0], "unset\0", 6) == 0)
		return (run_unset(cmd_parts, envp));
	else if (ft_strncmp((*cmd_parts)->cmd_array[0], "export\0", 7) == 0)
		return (run_export(cmd_parts, envp));
	else if (ft_strncmp((*cmd_parts)->cmd_array[0], "echo\0", 5) == 0)
		return (run_echo(cmd_parts));
	else if (ft_strncmp((*cmd_parts)->cmd_array[0], "env\0", 4) == 0)
		return (run_env(cmd_parts, envp));
	else if (ft_strncmp((*cmd_parts)->cmd_array[0], "cd\0", 3) == 0)
		return (run_cd(cmd_parts, envp));
	else if (ft_strncmp((*cmd_parts)->cmd_array[0], "exit\0", 5) == 0)
		return (run_exit_single(shell, cmd_parts, envp, original_fd));
	else if (ft_strncmp((*cmd_parts)->cmd_array[0], "pwd\0", 4) == 0)
	{
		ft_pwd(shell);
		free_cmd_parts(cmd_parts);
	}
	return (0);
}
