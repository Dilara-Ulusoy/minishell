/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_functions.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 16:52:08 by htopa             #+#    #+#             */
/*   Updated: 2025/01/30 16:52:10 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <linux/limits.h>
#include "minishell.h"

int ft_cd(char *new_path)
{
	if (chdir(new_path) != 0)
	{
		fprintf(stderr, "cd: %s: %s\n", strerror(errno), new_path);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

void ft_pwd()
{
	char cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
        printf("%s\n", cwd);
    else 
        write(2, "getcwd() error\n", 15);
}

void ft_echo(char *str)
{
	printf("%s\n", str);
}

int execute_pwd(char *cmd)
{
	//cmd = shell->ast->cmd_args;
	int i;
	i = 4;
	if (ft_strncmp(cmd, "pwd\0", 4) == 0)
		ft_pwd();
	if (ft_strncmp(cmd, "pwd ", 4) == 0)
	{		
		while (cmd[i] != '\0')
		{
			if (cmd[i] != ' ') // && cmd[i] != '\f' && cmd[i] != '\r' && cmd[i] != '\n' && cmd[i] != '\t ' && cmd[i] != '\v')
			{
				write(2,"pwd: too many arguments\n", 24);
				return (1);
			}
			i++;
		}
		ft_pwd();
	}
	return (0);
}