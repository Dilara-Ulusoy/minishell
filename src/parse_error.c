#include "minishell.h"

void set_syntax_error(t_parser *parser, const char *token_value)
{
	parser->error_status = PARSE_SYNTAX_ERROR;
	printf("Syntax error near unexpected token '%s'\n", token_value);
}

void *handle_parse_error(t_parser *p, t_ast_node *left_node, t_ast_node *right_node, const char *error_message)
{
	// Hata durumunu ayarla
	if (p->error_status == PARSE_OK)
	{
		p->error_status = PARSE_SYNTAX_ERROR;
	}
	// Hata mesajını yazdır
	if (error_message)
	{
		printf("Error: %s\n", error_message);
	}
	// Bellek temizliği
	if (left_node)
		free_ast(left_node);
	if (right_node)
		free_ast(right_node);
	return (void *)NULL;
}

char *handle_allocation_error(t_parser *parser, char *var_name, const char *error_message)
{
	if (error_message)
		perror(error_message); // Hata mesajını yazdır
	if (var_name)
		free(var_name);							   // Belleği temizle
	if (parser)									   // `parser` NULL değilse kontrol et
		parser->error_status = PARSE_MEMORY_ERROR; // Hata durumunu işaretle
	return NULL;								   // NULL döndür
}
