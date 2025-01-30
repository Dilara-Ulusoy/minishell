#include "minishell.h"

/*****************************************************************************/
/*                           IMPLEMENTATION                                  */
/*****************************************************************************/

/**
 * @brief build_ast
 * Initializes a parser with the token list and calls parse_expression(0).
 * If leftover tokens or errors are found, sets parser.error_status accordingly.
 */

static void	init_parser(t_parser *parser, t_token *token_list)
{
	parser->tokens = token_list;
	parser->current_token = token_list;
	parser->error_status = PARSE_OK;
	parser->error_number = 0;
}

t_ast_node	*build_ast(t_token *token_list)
{
	t_parser	parser;
	t_ast_node	*root;

	root = NULL;
	init_parser(&parser, token_list);
	if(!check_syntax_errors(&parser) || parser.error_status == PARSE_SYNTAX_ERROR)
		return NULL;
	root = parse_expression(&parser, 0);     // Parse the token list into an AST
	if (parser.error_status != PARSE_OK)     // Handle errors during parsing
	{
		free_ast(root); // Clean up the partially constructed AST
		return NULL;
	}
	return (root);    // Return the constructed AST
}

