/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 10:22:37 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/04/22 17:01:23 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_initial_redirection(t_parser *p, t_io_node **io_list,
	t_shell *shell)
{
	if (is_redirection(p->current_token->type))
	{
		if (parse_redirections(p, io_list, shell) == -1)
			return (-1);
	}
	return (0);
}

static char	*handle_command_string(t_parser *p, t_io_node *io_list)
{
	char	*cmd_args;

	cmd_args = build_command_string(p);
	if (!cmd_args || p->error_status != PARSE_OK || cmd_args[0] == '\0')
	{
		if (io_list)
			p->error_status = PARSE_SYNTAX_ERROR;
		free_io_list(io_list);
		free(cmd_args);
		return (NULL);
	}
	return (cmd_args);
}

static int	handle_post_redirection(t_parser *p, t_io_node **io_list,
		char *cmd_args, t_shell *shell)
{
	if (parse_redirections(p, io_list, shell) == -1)
	{
		cleanup_resources(cmd_args, *io_list);
		return (-1);
	}
	return (0);
}

static t_ast_node	*create_command_node_or_cleanup(t_parser *p, char *cmd_args,
	t_io_node *io_list)
{
	t_ast_node	*cmd_node;

	cmd_node = create_ast_command_node(cmd_args, io_list);
	if (!cmd_node)
	{
		p->error_status = PARSE_MEMORY_ERROR;
		cleanup_resources(cmd_args, io_list);
		return (NULL);
	}
	return (cmd_node);
}

t_ast_node	*parse_command(t_parser *p, t_shell *shell)
{
	t_io_node	*io_list;
	t_ast_node	*cmd_node;
	char		*cmd_args;

	cmd_node = NULL;
	io_list = NULL;
	cmd_args = NULL;
	if (!p->current_token || p->error_status != PARSE_OK)
		return (NULL);
	if (handle_initial_redirection(p, &io_list, shell) == -1)
		return (NULL);
	if (p->current_token != NULL && p->current_token->type == TOKEN_WORD)
	{
		cmd_args = handle_command_string(p, io_list);
		if (!cmd_args)
			return (NULL);
	}
	if (handle_post_redirection(p, &io_list, cmd_args, shell) == -1)
		return (NULL);
	cmd_node = create_command_node_or_cleanup(p, cmd_args, io_list);
	if (!cmd_node)
		return (NULL);
	free(cmd_args);
	return (cmd_node);
}
