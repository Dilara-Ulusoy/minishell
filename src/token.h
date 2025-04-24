/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:06:41 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/04/24 15:41:47 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

# include "minishell.h"

typedef struct s_shell	t_shell;

/*
	token_type:
	- Each enumerator corresponds to a possible category of token.
	- Requirements:
		1) We should handle word tokens (like "ls", "echo").
		2) We should handle single-character operators (<, >, |, (, )).
		3) We should handle multi-character operators (&&, ||, <<, >>).
		4) If needed, we can include TOKEN_UNKNOWN for unrecognized sequences.
*/
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_REDIR_HERE,
	TOKEN_PAREN_OPEN,
	TOKEN_PAREN_CLOSE,
	TOKEN_UNKNOWN,
}	t_token_type;

/*
	s_token:
	- This structure is used to build a linked list of tokens.
	- Each node (s_token) has:
		1) 'type'  -> Which category the token falls under (from token_type).
		2) 'value' -> The actual string text (e.g., "echo", "&&").
		3) 'next'  -> Pointer to the next token in the linked list.

	Requirements for a linked-list based token representation:
		- We can easily insert tokens in the order we find them while scanning
		  the input string.
		- After the tokenize method finishes, this linked list can be
		  passed to the parser to build an Abstract Syntax Tree (AST).
*/
typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

/* Token Creation and Management */
t_token			*tokenize(t_token *head, const char *line,
					int length, t_shell *shell);
t_token			*create_new_token_range(t_token_type type,
					const char *line, int startIndex, int length);
int				append_token(t_token **head, t_token *new_token);
void			free_tokens(t_token **head);

/* Token Utilities */
int				is_redir(t_token_type type);
int				is_operator(t_token_type type);
int				is_redirection(t_token_type type);
int				get_precedence(t_token_type type);
int				is_two_char_operator(char c);
t_token_type	match_two_char_operator(const char *line, int index);
t_token_type	match_single_char_operator(char c);

/* Debugging */
void			print_tokens(const t_token *head);

#endif
