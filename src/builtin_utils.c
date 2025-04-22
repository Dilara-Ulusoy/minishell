/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:05:54 by htopa             #+#    #+#             */
/*   Updated: 2025/04/22 20:44:15 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	contains_equal(char *s)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

// Free if malloc fails
char	*copy_until_equal(char *src)
{
	char	*dest;
	int		i;

	i = 0;
	while (src[i] != '=' && src[i] != '\0')
		i++;
	dest = (char *)malloc(sizeof(char) * (i + 2));
	if (dest == NULL)
	{
		ft_putstr_fd("Malloc failed!\n", 2);
		return (NULL);
	}
	i = 0;
	while (src[i] != '=' && src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '=';
	dest[i + 1] = '\0';
	return (dest);
}

int	ft_strcmp_wo_equal(char *s1, char *s2)
{
	int	i;

	if (ft_strlen(s1) != (ft_strlen(s2) - 1))
		return (0);
	i = 0;
	while (s1[i] != '\0')
	{
		if (s1[i] != s2[i])
			return (0);
		i++;
	}
	return (1);
}

int	check_echo_option(char *str)
{
	int	i;

	if (str[0] == '-' && str[1] == 'n')
	{
		i = 2;
		while (str[i] != '\0')
		{
			if (str[i] == 'n')
				i++;
			else
				return (0);
		}
		return (1);
	}
	return (0);
}

int	is_builtin(t_cmd_parts *cmd_parts)
{
	if (cmd_parts->cmd_array[0] == NULL)
		return (0);
	if (ft_strncmp(cmd_parts->cmd_array[0], "echo\0", 5) == 0)
		return (1);
	else if (ft_strncmp(cmd_parts->cmd_array[0], "exit\0", 5) == 0)
		return (1);
	else if (ft_strncmp(cmd_parts->cmd_array[0], "env\0", 4) == 0)
		return (1);
	else if (ft_strncmp(cmd_parts->cmd_array[0], "pwd\0", 4) == 0)
		return (1);
	else if (ft_strncmp(cmd_parts->cmd_array[0], "cd\0", 3) == 0)
		return (1);
	else if (ft_strncmp(cmd_parts->cmd_array[0], "export\0", 7) == 0)
		return (1);
	else if (ft_strncmp(cmd_parts->cmd_array[0], "unset\0", 6) == 0)
		return (1);
	else
		return (0);
}
