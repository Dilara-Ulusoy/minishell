/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 10:22:37 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/04/03 18:02:31 by htopa            ###   ########.fr       */
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
	if (p->current_token->type == TOKEN_WORD)
	{
		cmd_args = handle_command_string(p, io_list);
		if (!cmd_args)
			return (NULL);
	}
	if (handle_post_redirection(p, &io_list, cmd_args, shell) == -1)
		return (NULL);
	cmd_node = create_ast_command_node(cmd_args, io_list);
	if (!cmd_node)
	{
		p->error_status = PARSE_MEMORY_ERROR;
		cleanup_resources(cmd_args, io_list);
		return (NULL);
	}
	free(cmd_args);
	return (cmd_node);
}

// t_ast_node	*parse_command(t_parser *p, t_shell *shell)
// {
// 	t_io_node	*io_list;
// 	t_ast_node	*cmd_node;
// 	char		*cmd_args;

// 	cmd_node = NULL;
// 	io_list = NULL;
// 	cmd_args = NULL;
// 	if (!p->current_token || p->error_status != PARSE_OK)
// 		return (NULL);
// 	if (handle_initial_redirection(p, &io_list, shell) == -1)
// 		return (NULL);
// 	cmd_args = handle_command_string(p, io_list);
// 	if (!cmd_args)
// 		return (NULL);
// 	if (handle_post_redirection(p, &io_list, cmd_args, shell) == -1)
// 		return (NULL);
// 	cmd_node = create_ast_command_node(cmd_args, io_list);
// 	if (!cmd_node)
// 	{
// 		p->error_status = PARSE_MEMORY_ERROR;
// 		cleanup_resources(cmd_args, io_list);
// 		return (NULL);
// 	}
// 	free(cmd_args);
// 	return (cmd_node);
// }

/*
📌 build_command_string(p)
Purpose: Constructs a single command string by
concatenating consecutive WORD tokens.

Steps:
1) Initializes a dynamic buffer (`cmd_buffer`) with an initial size of 256 bytes.
2) Iterates through `TOKEN_WORD` tokens, appending their values to `cmd_buffer`.
3) Expands the buffer if needed using `append_to_buffer()`.
4) Stops when a non-WORD token is encountered.
5) Returns the built command string or NULL on memory allocation failure.

Example:
- Tokens: [ WORD("echo"), WORD("hello"), WORD("world") ]
- Returns: `"echo hello world"`

Effect:
- Moves `p->current_token` forward past the processed WORD tokens.
- If memory allocation fails,
sets `p->error_status = PARSE_MEMORY_ERROR` and returns NULL.
*/
char	*build_command_string(t_parser *p)
{
	t_buffer	cmd_buffer;

	cmd_buffer.size = 256;
	cmd_buffer.pos = 0;
	cmd_buffer.data = ft_calloc(cmd_buffer.size, 1);
	if (!cmd_buffer.data)
	{
		p->error_status = PARSE_MEMORY_ERROR;
		return (NULL);
	}
	while (p->current_token && p->current_token->type == TOKEN_WORD)
	{
		if (!append_to_buffer(&cmd_buffer, p->current_token->value))
		{
			p->error_status = PARSE_MEMORY_ERROR;
			free(cmd_buffer.data);
			return (NULL);
		}
		get_next_token(p);
	}
	return (cmd_buffer.data);
}
