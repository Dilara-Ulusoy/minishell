/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_node_create.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 22:16:56 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/03/21 11:01:00 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast_node_type	get_ast_node_type(t_token_type operator_type)
{
	if (operator_type == TOKEN_PIPE)
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
	return (node);
}

static char	*allocate_command_string(const char *cmd_text,
		t_ast_node *node, t_io_node *io_list)
{
	char	*command_string;

	if (cmd_text && *cmd_text)
	{
		command_string = ft_strdup(cmd_text);
		if (!command_string)
		{
			free(node);
			free_io_list(io_list);
			return (NULL);
		}
	}
	else
	{
		command_string = ft_strdup("");
		if (!command_string)
		{
			free(node);
			free_io_list(io_list);
			return (NULL);
		}
	}
	return (command_string);
}

/*
   create_ast_command_node(cmd_args, io_list):
   - Allocates an AST node of type AST_COMMAND.
   - Duplicates 'cmd_args' if non-empty, assigns
	'io_list' to node->io_redirects.
   - left/right remain NULL because a command node doesn't have children.

   Returns the newly created AST node.
*/
t_ast_node	*create_ast_command_node(const char *cmd_text, t_io_node *io_list)
{
	t_ast_node	*node;

	node = (t_ast_node *)malloc(sizeof(t_ast_node));
	if (!node)
	{
		ft_putstr_fd("Memory allocation failed at create_ast_cmd_node.\n", 2);
		free_ast(node);
		return (NULL);
	}
	//printf("Actual----> %s\n", cmd_text);
	node->node_type = AST_COMMAND;
	node->cmd_args = NULL;
	node->cmd_args = allocate_command_string(cmd_text, node, io_list);
	node->io_redirects = io_list;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

t_io_node	*create_io_node(t_io_type kind, const char *filename, t_parser *p)
{
	t_io_node	*new_io;

	new_io = (t_io_node *)malloc(sizeof(t_io_node));
	if (!new_io)
	{
		p->error_status = PARSE_MEMORY_ERROR;
		return (NULL);
	}
	new_io->io_kind = kind;
	new_io->filename = ft_strdup(filename);
	if (!new_io->filename)
	{
		ft_putstr_fd("Memory allocation failed at create_io_node.\n", 2);
		free(new_io);
		p->error_status = PARSE_MEMORY_ERROR;
		return (NULL);
	}
	new_io->next = NULL;
	return (new_io);
}
