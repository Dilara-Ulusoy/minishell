/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:59:15 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/03/21 12:50:54 by htopa            ###   ########.fr       */
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
void	init_shell(t_shell *shell);

/* Environment Variable Expansion */
char	*get_env_var_value(const char *line, int *index);
char	*handle_env_variable_without_space(const char *line,
			int *index, int start);
char	*handle_dollar_sign(const char *line, int *index, int start);

/* Quote Parsing */
char	*join_string_with_quoted_if_no_space(const char *line,
			int *index, int start);
char	*parse_quotes(const char *line, int *index);
int		init_parse_quote(t_parse_quote *p, const char *line, int *index);

/* Word Parsing */
char	*read_word_range(const char *line, int *index, int length);
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
int		parse_word(t_token **head, const char *line, int *pos, int length);
int		handle_newline(t_token **head, const char *line, int *pos);

/* Cleanup */
void	cleanup_shell(t_shell *shell);

/* Utility */
void	*ft_realloc(void *ptr, size_t size);
void	*free_this(char *s1, char *s2, char *s3, char *message);

#endif
