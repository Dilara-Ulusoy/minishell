/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoc@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:21:29 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/04/15 19:44:40 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	ft_strspn(const char *s, const char *accept)
{
	size_t	i;
	size_t	j;
	int		found;

	i = 0;
	while (s[i])
	{
		found = 0;
		j = 0;
		while (accept[j])
		{
			if (s[i] == accept[j])
			{
				found = 1;
				break;
			}
			j++;
		}
		if (!found)
			break;
		i++;
	}
	return (i);
}

void	init_shell(t_shell *shell, char ***envp)
{
	shell->line = NULL;
	shell->tokens = NULL;
	shell->current_token = NULL;
	shell->ast = NULL;
	shell->parser = NULL;
	shell->exit_code = 0;
	shell->line_length = 0;
	shell->envp = envp;
	shell->heredoc_tmp = NULL;
}

/*
    get_input:
    - Prompts the user for input via readline.
    - If user presses Ctrl+D, readline returns NULL.
    - We trim leading/trailing spaces with ft_strtrim (ensure it's available).
    - Return the trimmed string. The caller must free it.
*/
char	*get_input(const char *prompt)
{
	char	*line;
	char	*trimmed_line;

	line = readline(prompt);
	if (!line)
		return (NULL);
	if (ft_strspn(line, " \t\n") == ft_strlen(line))
	{
		add_history(line);
		return (line);
	}
	trimmed_line = ft_strtrim(line, " \t\n");
	free(line);
	if (!trimmed_line)
	{
		ft_putstr_fd("Error: Memory allocation failed\n", STDERR_FILENO);
		return (NULL);
	}
	if (*trimmed_line)
		add_history(trimmed_line);
	return (trimmed_line);
}

/*
** p->line: The input string to be parsed.
** p->index: A pointer to an integer that
keeps track of the current position in the input string.
** p->quote: The type of quote (single or double).
** p->quote_in_use: A flag to indicate if the quote is in use.
Prevents variable expansion inside single quotes.
** p->result_index: The current position in the result string.
** p->result: An array to store the processed output string.
Eg. "$HOME" -> "/home/user"
** p->result_size: The length of the result array. We use this to reallocate
memory result array needs to expand.
*/
int	init_parse_quote(t_parse_quote *p, const char *line, int *index)
{
	p->line = line;
	p->index = *index;
	p->quote = 0;
	p->quote_is_double = 0;
	p->result_index = 0;
	p->result = ft_calloc(ft_strlen(line) + 1, sizeof(char));
	p->result_size = ft_strlen(line) + 1;
	if (!p->result)
	{
		ft_putstr_fd("Memory allocation failed in init_parse_quote\n",
			STDERR_FILENO);
		return (1);
	}
	return (0);
}
