/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoc@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:18:55 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/03/18 14:03:40 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "minishell.h"
# include "token.h"

/*
    e_ast_node_type: Types of AST nodes we handle:
      - AST_COMMAND: a node that represents a command + arguments + redirections
      - AST_AND:     represents `&&`
      - AST_OR:      represents `||`
      - AST_PIPE:    represents `|`
*/
typedef enum e_ast_node_type
{
	AST_COMMAND,
	AST_AND,
	AST_OR,
	AST_PIPE,
	AST_INVALID,
}	t_ast_node_type;

/*
    e_io_type: For classifying redirection tokens:
      - IO_IN  : `<`
      - IO_OUT : `>`
      - IO_APPEND : `>>`
      - IO_HEREDOC: `<<`
*/
typedef enum e_io_type
{
	IO_IN,
	IO_OUT,
	IO_APPEND,
	IO_HEREDOC
}	t_io_type;

/*
    s_io_node: linked list for storing redirection info in a command node.
      - io_kind:  e.g., IO_IN, IO_OUT, etc.
      - filename: typically the word after `<`, `>`, etc.
*/
typedef struct s_io_node
{
	t_io_type			io_kind;
	char				*filename;
	struct s_io_node	*next;
}	t_io_node;

/*
    s_ast_node: an AST node can be:
      - AST_COMMAND (with cmd_args and io_redirects set)
      - AST_AND / AST_OR / AST_PIPE (operator nodes with left/right children)
	 If it's a command node:
	 - cmd_args: combined string e.g. "echo hello"
	 - io_redirects: linked list of redirections

	 If it's an operator node (AND, OR, PIPE):
	 - left: left subtree
	 - right: right subtree

*/
typedef struct s_ast_node
{
	t_ast_node_type		node_type;

	char				*cmd_args;
	t_io_node			*io_redirects;

	struct s_ast_node	*left;
	struct s_ast_node	*right;
}	t_ast_node;

typedef enum e_parse_error
{
	PARSE_OK,
	PARSE_SYNTAX_ERROR,
	PARSE_MEMORY_ERROR
}	t_parse_error;

/*
    t_parser: holds the tokens, a pointer to the current token, and error status.
*/
typedef struct s_parser
{
	t_token			*tokens;
	t_token			*current_token;
	t_parse_error	error_status;
	int				error_number;
	int				op_prec;
}	t_parser;

/* AST creation and manipulation */
t_ast_node		*create_ast_command_node(const char *cmd_text,
					t_io_node *io_list);
t_ast_node		*create_ast_operator_node(t_ast_node_type type,
					t_ast_node *left, t_ast_node *right);
t_io_node		*create_io_node(t_io_type kind, const char *filename,
					struct s_parser *p);
int			attach_io_node(t_io_node **io_list, t_io_node *new_io);
t_ast_node		*built_operator_node(t_ast_node *left_node,
					t_ast_node *right_node, t_token_type operator_type,
					struct s_parser *p);

/* AST cleanup */
void			free_ast(t_ast_node *root);
void			free_io_list(t_io_node *io_list);

/* Parsing functions */
t_ast_node		*build_ast(t_token *token_list);
t_ast_node		*parse_expression(t_parser *p, int min_prec);
t_ast_node		*parse_term(t_parser *p);
t_ast_node		*parse_command(t_parser *p);
int			parse_redirections(t_parser *p, t_io_node **io_list);
void			cleanup_resources(char *cmd_args, t_io_node *io_list);

/* Syntax validation */
int				check_syntax_errors(t_parser *parser);
int				set_syntax_error(t_parser *parser, const char *token_value);
//int			set_error_number(t_parser *parser, int number);

/* Error handling */
void			*handle_parse_error(t_parser *p, t_ast_node *left_node,
					t_ast_node *right_node, const char *error_message);
char			*handle_allocation_error(t_parser *parser,
					char *var_name, const char *error_message);

/* Utility */
t_ast_node_type	get_ast_node_type(t_token_type operator_type);
char			*build_command_string(t_parser *p);
void			get_next_token(t_parser *p);
t_io_type		map_token_to_io_type(t_token_type ttype);

//Debugging
void			debug_print_io_list(const t_io_node *io_list, int depth);
void			debug_ast(const t_ast_node *root, int depth);

#endif
