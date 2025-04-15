/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoc@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:35:01 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/04/15 15:48:29 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	write_heredoc_line(int fd, char *line, t_shell *shell)
{
	char	*env;
	char	*var_value;

	shell->index = 0;
	if (line[shell->index] == '$')
	{
		shell->index++;
		env = get_var_name(line, &shell->index);
		var_value = ft_getenv(env, shell);
		if (var_value)
			write(fd, var_value, ft_strlen(var_value));
		write(fd, "\n", 1);
		free(env);
		free(var_value);
	}
	else
	{
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
	}
}

static int	heredoc_loop(int tmp_fd, const char *delimiter,
		t_shell *shell, const char *path)
{
	char	*line;

	while (1)
	{
		line = get_input("> ");
		if (g_signal == SIGINT)
		{
			free(line);
			close(tmp_fd);
			unlink(path);
			return (-1);
		}
		if (!line || ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
		{
			free(line);
			break ;
		}
		write_heredoc_line(tmp_fd, line, shell);
		free(line);
	}
	return (0);
}

int	heredoc_to_tempfile(const char *delimiter, t_shell *shell, const char *path)
{
	int	tmp_fd;
	int	result;

	set_signals(NULL, SIGNAL_HEREDOC);
	tmp_fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (tmp_fd == -1)
		return (-1);
	result = heredoc_loop(tmp_fd, delimiter, shell, path);
	close(tmp_fd);
	set_signals(NULL, SIGNAL_PARENT);
	return (result);
}

int	handle_heredoc(char **delimiter, t_shell *shell, int index)
{
	char	*path;
	char	*suffix;
	char	*base;

	base = "/tmp/minishell_heredoc_";
	suffix = ft_itoa(index);
	if (!suffix)
		return (-1);
	path = ft_strjoin(base, suffix);
	free(suffix);
	if (!path)
		return (-1);
	if (heredoc_to_tempfile(*delimiter, shell, path) == -1)
	{
		free(path);
		return (-1);
	}
	if (*delimiter)
		free(*delimiter);
	*delimiter = ft_strdup(path);
	free(path);
	return (0);
}

int	process_io(t_parser *p, t_io_node **io_list, t_io_type kind)
{
	t_io_node	*new_io;

	new_io = create_io_node(kind, p->current_token->value, p);
	if (!new_io)
	{
		free_io_list(*io_list);
		return (-1);
	}
	if (attach_io_node(io_list, new_io) == -1)
		return (-1);
	if (p->current_token && p->current_token->next
		&& p->current_token->next->next && p->current_token->type == TOKEN_WORD)
		get_next_token(p);
	return (0);
}
