/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoc@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:59:15 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/04/15 15:58:03 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <stdbool.h>
# include <dirent.h>
# include <errno.h>
# include <signal.h>
# include "parsing.h"
# include "token.h"
# include "execution.h"
# include "signal.h"
# include "../Libft/libft.h"

typedef struct s_shell
{
	char		*line;
	t_token		*tokens;
	t_token		*current_token;
	t_ast_node	*ast;
	t_parser	*parser;
	int			exit_code;
	int			line_length;
	char		*heredoc_tmp;
	char		***envp;

	// 👇 Geçici parsing index’i için
	int			index;
}	t_shell;


typedef struct s_parse_quote
{
	const char	*line;
	int			index;
	char		quote;
	int			quote_is_double;
	int			result_index;
	char		*result;
	size_t		result_size;
}	t_parse_quote;

typedef struct s_buffer
{
	char	*data;
	size_t	size;
	size_t	pos;
}	t_buffer;

/* Input Handling and Initialization */
char	*get_input(const char *prompt);
void	init_shell(t_shell *shell, char ***envp);
int	init_term_and_signal(int argc, char **argv, int *exit_code);
int	event(void);

/* Environment Variable Expansion */
char	*get_var_name(const char *line, int *index);
char	*get_env_var_value(const char *line, int *index, t_shell *shell);
char	*handle_env_variable_without_space(const char *line,
			int *index, int start, t_shell *shell);
char	*handle_dollar_sign(const char *line, int *index, int start, t_shell *shell);
char	*handle_special_cases(const char *line, int *index, int start, t_shell *shell);

/* Quote Parsing */
char	*join_string_with_quoted_if_no_space(const char *line,
			int *index, int start, t_shell *shell);
char	*parse_quotes(const char *line, int *index, t_shell *shell);
char	*process_quote_loop(t_parse_quote *p, t_shell *shell);
char	*check_unmatched_quote(t_parse_quote *p);
int		init_parse_quote(t_parse_quote *p, const char *line, int *index);

/* Word Parsing */
char	*read_word_range(const char *line, int *index, int length, t_shell *shell);
char	*resize_buffer(char *buffer, size_t *buffer_size);

/* Whitespace Handling */
int		skip_whitespace(const char *line, int i);
int		is_space(char c);

/*****************************************************************************/
/*                        PARSE STEPS                                        */
/*****************************************************************************/

/* Step 2: Parse Two-Char Operator */
int		parse_two_char_operator(t_token **head, const char *line,
			int *pos, int length);

/* Step 3: Parse Single-Char Operator */
int		parse_single_char_operator(t_token **head, const char *line,
			int *pos, int length);

/* Step 4: Parse Word */
int		parse_word(t_token **head, t_shell *shell);
int		handle_newline(t_token **head, const char *line, int *pos);

// Heredoc

void	write_heredoc_line(int fd, char *line, t_shell *shell);
int		heredoc_to_tempfile(const char *delimiter, t_shell *shell, const char *path);
int		handle_heredoc(char **delimiter, t_shell *shell, int index);
int		process_heredoc(t_parser *p, t_io_node **io_list, t_shell *shell, int *index);
int		process_io(t_parser *p, t_io_node **io_list, t_io_type kind);

/* Cleanup */
void	cleanup_shell(t_shell *shell);

/* Utility */
void	*ft_realloc(void *ptr, size_t size);
void	*free_this(char *s1, char *s2, char *s3, char *message);
int		append_to_buffer(t_buffer *buf, const char *word_value);

#endif
