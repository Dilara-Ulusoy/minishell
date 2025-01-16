
#include "minishell.h"

int is_operator(t_token_type type)
{
    return (type == TOKEN_AND || type == TOKEN_OR || type == TOKEN_PIPE ||
            type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT ||
            type == TOKEN_REDIR_APPEND || type == TOKEN_REDIR_HERE);
}

int is_redirection(t_token_type type)
{
    return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT ||
            type == TOKEN_REDIR_APPEND || type == TOKEN_REDIR_HERE);
}

void set_syntax_error(t_parser *parser, const char *token_value)
{
    parser->error_status = PARSE_SYNTAX_ERROR;
    printf("Syntax error near unexpected token '%s'\n", token_value ? token_value : "newline");
}

void check_syntax_errors(t_parser *parser)
{
    t_token *current = parser->tokens;

	if (!parser || !parser->tokens) /* Early return if no tokens */
        return;

       /* Check if the first token is invalid */
    if (is_operator(current->type))
    {
        set_syntax_error(parser, current->value);
        return;
    }
   while (current && current->next)
{
    if ((is_operator(current->type) && is_operator(current->next->type)) ||
        (is_redirection(current->type) && (!current->next || current->next->type != TOKEN_WORD)))
    {
        set_syntax_error(parser, current->next ? current->next->value : "newline");
        return;
    }
    current = current->next;
}
    if (current && is_operator(current->type))     /* Check if the last token is an invalid operator */
        set_syntax_error(parser, current->value);
}
void *handle_parse_error(t_parser *p, t_ast_node *left_node, t_ast_node *right_node, const char *error_message)
{
    // Hata durumunu ayarla
    if (p->error_status == PARSE_OK) {
        p->error_status = PARSE_SYNTAX_ERROR;
    }
    // Hata mesajını yazdır
    if (error_message) {
        printf("Error: %s\n", error_message);
    }
    // Bellek temizliği
    if (left_node)
        free_ast(left_node);
    if (right_node)
        free_ast(right_node);
       return(void *)NULL;
}

char *handle_allocation_error(t_parser *parser, char *var_name, const char *error_message)
{
    if (error_message)
        perror(error_message); // Hata mesajını yazdır
    if (var_name)
        free(var_name); // Belleği temizle
    if (parser) // `parser` NULL değilse kontrol et
        parser->error_status = PARSE_MEMORY_ERROR; // Hata durumunu işaretle
    return NULL; // NULL döndür
}


void get_next_token(t_parser *p)
{
    if (p->current_token)
        p->current_token = p->current_token->next;
}

/*
   is_binary_operator:
   - returns 1 if it's &&, ||, or |,
   - 0 otherwise
*/
int is_binary_operator(t_token_type ttype)
{
    if (ttype == TOKEN_AND || ttype == TOKEN_OR || ttype == TOKEN_PIPE)
        return 1;
    return 0;
}

/*
   get_precedence:
   - a simple numeric precedence system
     OR (||) -> 10
     AND (&&) -> 20
     PIPE (|) -> 30
   - higher => parse first
*/
int get_precedence(t_token_type type)
{
    if(type == TOKEN_OR)
		return 10;
	else if(type == TOKEN_AND)
		return 20;
	else if(type == TOKEN_PIPE)
		return 30;
	else
		return 0; /* lowest precedence */
}