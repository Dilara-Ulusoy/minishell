/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirections.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoc@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 12:57:34 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/03/30 21:03:58 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void redirect_stdin_from_file(const char *path)
{
	int fd = open(path, O_RDONLY);
	if (fd == -1)
	{
		perror("open for stdin redirection");
		exit(1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		exit(1);
	}
	close(fd);
}

char *generate_heredoc_filename(int index)
{
	char *base = "/tmp/minishell_heredoc_";
	char *suffix = ft_itoa(index); // Kendi ft_itoa fonksiyonunu kullan
	if (!suffix)
		return (NULL);
	char *filename = ft_strjoin(base, suffix);
	free(suffix);
	return filename;
}

void write_heredoc_line(int fd, char *line, t_shell *shell)
{
	char *env;
	char *var_value;

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
		free(var_value); // var_value NULL olsa bile free(NULL) güvenlidir
	}
	else
	{
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
	}
}


int heredoc_to_tempfile(const char *delimiter, t_shell *shell, const char *path)
{
	char *line;
	int tmp_fd;

	tmp_fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (tmp_fd == -1)
		return (-1);

	while (1)
	{
		line = get_input("heredoc> ");
		if (!line || ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
		{
			free(line);
			break;
		}
		write_heredoc_line(tmp_fd, line, shell);
		free(line);
	}
	close(tmp_fd);
	return (0);
}

int handle_heredoc(const char *delimiter, t_shell *shell, int index, char **out_path)
{
	char *path = generate_heredoc_filename(index);
	if (!path)
		return (-1);
	if (heredoc_to_tempfile(delimiter, shell, path) == -1)
	{
		perror("heredoc");
		free(path);
		return (-1);
	}
	*out_path = path;
	return (0);
}

int parse_redirections(t_parser *p, t_io_node **io_list, t_shell *shell)
{
	t_io_type kind;
	t_io_node *new_io;
	int index = 0;
	char *path = NULL;
	static char *last_heredoc_path = NULL;

	while (p->current_token && is_redirection(p->current_token->type))
	{
		kind = map_token_to_io_type(p->current_token->type);
		get_next_token(p);
		if (!p->current_token || p->current_token->type != TOKEN_WORD)
		{
			p->error_status = PARSE_SYNTAX_ERROR;
			return (-1);
		}
		if (kind == IO_HEREDOC)
		{
			if (handle_heredoc(p->current_token->value, shell, index, &path) == -1)
				return (-1);
			index++;
			// önceki path'i sil, son heredoc path'i tutulsun
			if (last_heredoc_path)
				free(last_heredoc_path);
			last_heredoc_path = path;
			redirect_stdin_from_file(path);
			get_next_token(p);
		}
		new_io = create_io_node(kind, p->current_token->value, p);
		if (!new_io)
		{
			free_io_list(*io_list);
			return (-1);
		}
		if (attach_io_node(io_list, new_io) == -1)
			return (-1);
		get_next_token(p);
		if (p->current_token && p->current_token->type == TOKEN_WORD)
			get_next_token(p);
	}
	return (0);
}

/*
   map_token_to_io_type:
   - Convert a token type for redirection (<, >, >>, <<)
     into an i/o type (IO_IN, IO_OUT, IO_APPEND, IO_HEREDOC).
*/
t_io_type	map_token_to_io_type(t_token_type type)
{
	if (type == TOKEN_REDIR_IN)
		return (IO_IN);
	else if (type == TOKEN_REDIR_OUT)
		return (IO_OUT);
	else if (type == TOKEN_REDIR_APPEND)
		return (IO_APPEND);
	else if (type == TOKEN_REDIR_HERE)
		return (IO_HEREDOC);
	else
		return (IO_IN);
}

/* Helper function: Attach a new IO node to the IO list */
int	attach_io_node(t_io_node **io_list, t_io_node *new_io)
{
	t_io_node	*tmp;

	if (!new_io)
		return (-1);
	if (!*io_list)
		*io_list = new_io;
	else
	{
		tmp = *io_list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_io;
	}
	return (0);
}

void	get_next_token(t_parser *p)
{
	if (p->current_token)
		p->current_token = p->current_token->next;
}

int	is_redirection(t_token_type type)
{
	if (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_APPEND || type == TOKEN_REDIR_HERE)
		return (1);
	return (0);
}
