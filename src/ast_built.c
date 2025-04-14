/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_built.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 09:03:53 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/04/14 08:02:04 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_parser(t_parser *parser, t_token *token_list)
{
	parser->tokens = token_list;
	parser->current_token = token_list;
	parser->error_status = PARSE_OK;
	parser->error_number = 0;
}

/*
📌 build_ast(token_list)

Purpose: Constructs an Abstract Syntax Tree (AST) from a list of tokens.

Example:
If token_list represents "echo hello", build_ast()
returns an AST node representing this command.
If token_list has syntax errors, it returns NULL.

Effect:
- Initializes the parser with the token list.
- Checks for syntax errors; if found, returns NULL.
- Parses expressions recursively to build the AST.
- If an error occurs during parsing,
frees the partially built AST and returns NULL.
*/

t_ast_node	*build_ast(t_token *token_list, t_shell *shell)
{
	t_parser	parser;
	t_ast_node	*root;

	root = NULL;
	init_parser(&parser, token_list);
	if (!check_syntax_errors(&parser))
		return (NULL);
	root = parse_expression(&parser, 0, shell);
	if (!root || parser.error_status != PARSE_OK)
	{
		free_ast(root);
		return (NULL);
	}
	return (root);
}
