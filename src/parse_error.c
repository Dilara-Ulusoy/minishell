/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoc@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 12:09:11 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/04/15 16:21:03 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	set_syntax_error(t_parser *parser, const char *token_value)
{
	parser->error_status = PARSE_SYNTAX_ERROR;
	ft_putstr_fd("bash: syntax error near unexpected token '", STDERR_FILENO);
	ft_putstr_fd((char *)token_value, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
	return (1);
}

void	*handle_parse_error(t_parser *p, t_ast_node *left_node,
	t_ast_node *right_node, const char *error_message)
{
	if (p->error_status == PARSE_OK)
		p->error_status = PARSE_SYNTAX_ERROR;
	if (error_message)
		printf("Error: %s\n", error_message);
	if (left_node)
		free_ast(left_node);
	if (right_node)
		free_ast(right_node);
	return ((void *) NULL);
}

char	*handle_allocation_error(t_parser *parser, char *var_name,
	const char *error_message)
{
	if (error_message)
		printf("Error: %s\n", error_message);
	if (var_name)
		free(var_name);
	if (parser)
		parser->error_status = PARSE_MEMORY_ERROR;
	return (NULL);
}
