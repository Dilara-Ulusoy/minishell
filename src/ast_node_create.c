/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_node_create.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoc@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 22:16:56 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/01/31 14:07:41 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast_node_type	get_ast_node_type(t_token_type operator_type)
{
	if (operator_type == TOKEN_AND)
		return (AST_AND);
	else if (operator_type == TOKEN_OR)
		return (AST_OR);
	else if (operator_type == TOKEN_PIPE)
		return (AST_PIPE);
	else
		return (AST_INVALID);
}

/*
   create_ast_operator_node:
   - For operators like &&, ||, |
   - left and right subtrees are assigned
   - cmd_args = NULL, io_redirects = NULL
*/
t_ast_node	*create_ast_operator_node(t_ast_node_type type,
		t_ast_node *left, t_ast_node *right)
{
	t_ast_node	*node;

	node = (t_ast_node *)malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->node_type = type;
	node->cmd_args = NULL;
	node->io_redirects = NULL;
	node->left = left;
	node->right = right;
	printf("Allocated AST Operator Node: %p ----> %s\n",
			(void *)node,
			node->node_type == AST_AND ? "AND" : node->node_type == AST_OR ? "OR" : "PIPE");
	return (node);
}

/*
   create_ast_command_node(cmd_args, io_list):
   - Allocates an AST node of type AST_COMMAND.
   - Duplicates 'cmd_args' if non-empty, assigns
    'io_list' to node->io_redirects.
   - left/right remain NULL because a command node doesn't have children.

   free_io_list:
   - Iterate over the t_io_node linked list, free each filename, and free the node.
*/

t_ast_node	*create_ast_command_node(const char *cmd_text, t_io_node *io_list)
{
	t_ast_node	*node;

	node = (t_ast_node *)malloc(sizeof(t_ast_node));
	if (!node)
	{
		free_ast(node); // Prevent memory leak
		return (NULL);
	}
	printf("Allocated AST Command Node: %p ----> %s\n", (void *)node, cmd_text);
	node->node_type = AST_COMMAND;
	node->cmd_args = NULL;
	if (cmd_text && *cmd_text)
	{
		node->cmd_args = ft_strdup(cmd_text);
		if (!node->cmd_args)
		{
			free(node);
			free_io_list(io_list); // Bellek sızıntısını önlemek için
			return (NULL);
		}
	}
	else
	{
		// NULL yerine boş bir string atanabilir
		node->cmd_args = ft_strdup("");
		if (!node->cmd_args)
		{
			free(node);
			free_io_list(io_list); // Bellek sızıntısını önlemek için
			return (NULL);
		}
	}
	node->io_redirects = io_list;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

// Helper function: Create a new IO node
t_io_node	*create_io_node(t_io_type kind, const char *filename, t_parser *p)
{
	t_io_node	*new_io;

	new_io = (t_io_node *)malloc(sizeof(t_io_node));
	printf("Allocated AST IO Node: %p ----> %s\n",
		(void *)new_io, filename);
	if (!new_io)
	{
		p->error_status = PARSE_MEMORY_ERROR;
		return (NULL);
	}
	new_io->io_kind = kind;
	new_io->filename = ft_strdup(filename);
	new_io->next = NULL;
	if (!new_io->filename)
	{
		free(new_io);
		p->error_status = PARSE_MEMORY_ERROR;
		return (NULL);
	}
	return (new_io);
}
