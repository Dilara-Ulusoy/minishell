#include "minishell.h"

/*****************************************************************************/
/*                           IMPLEMENTATION                                  */
/*****************************************************************************/

/**
 * @brief build_ast
 * Initializes a parser with the token list and calls parse_expression(0).
 * If leftover tokens or errors are found, sets parser.error_status accordingly.
 */
t_ast_node *build_ast(t_token *token_list)
{
    t_parser parser;

    parser.tokens = token_list;
    parser.current_token = token_list;
    parser.error_status = PARSE_OK;
	check_syntax_errors(&parser);
	if (parser.error_status == PARSE_SYNTAX_ERROR)
        return NULL;
    /* parse the whole line as an expression of min precedence = 0 */
    t_ast_node *root = parse_expression(&parser, 0);

    if (parser.error_status == PARSE_OK && parser.current_token != NULL)
    {
        /* After completing the AST construction, if `current_token` still holds a value,
        it means not all tokens have been processed. This is considered a syntax error,
        as all tokens are expected to be included in the AST.*/
        parser.error_status = PARSE_SYNTAX_ERROR;
    }
    if (parser.error_status != PARSE_OK)
    {
        /* free partial AST and return NULL */
        free_ast(root);
        return NULL;
    }
    return root;
}