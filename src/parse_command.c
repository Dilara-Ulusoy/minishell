/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoc@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 10:22:37 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/02/19 12:07:45 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_ast_node	*handle_ast_command_creation(char *cmd_args,
	t_io_node *io_list, t_parser *p)
{
	t_ast_node	*cmd_node;

	cmd_node = create_ast_command_node(cmd_args, io_list);
	if (!cmd_node)
	{
		free_io_list(io_list);
		p->error_status = PARSE_MEMORY_ERROR;
		return (NULL);
	}
	return (cmd_node);
}

/*
	parse_command():
	---------------
	This function parses a simple command node. A "command" in this context
	is one or more WORD tokens (like "echo", "hello") that get combined into
	a single string, plus any optional redirections (<, >, >>, <<) that follow.

	STEP-BY-STEP (Beginner-Friendly):
	  1) If there's a WORD token, we capture it (and possibly more WORD tokens)
		 into one command string (e.g., "echo hello world").
	  2) After we finish reading WORD tokens, we look for redirection tokens
		 like TOKEN_REDIR_IN (<), TOKEN_REDIR_OUT (>), etc.
	  3) For each redirection, we consume the operator token, then expect a
		 WORD token for the filename. We store these in a simple linked list
		 (io_redirects).
	  4) Finally, we create and return a command node (AST_COMMAND) with
		 cmd_args = the combined command string, and io_redirects = the
		 redirection list. If we have no words at all but we do have
		 redirections, we can still form a command node (though a real shell
		 might treat that as an error or do something special).

	EXAMPLE:
	  Tokens: [ WORD("echo"), WORD("hello"), TOKEN_REDIR_OUT(">"),
				WORD("file.txt") ]
	  - Combine "echo" + "hello" => "echo hello" for cmd_args
	  - Redirection: > file.txt => store in io_redirects => IO_OUT("file.txt")
	  - Build a node => AST_COMMAND with:
		   node->cmd_args = "echo hello"
		   node->io_redirects => [IO_OUT("file.txt")]
	  - Return that node.

	IMPORTANT:
	  - In a real shell, you might handle special cases like 'redirection
		with no command' differently. For simplicity, we allow an empty
		command string if no WORD tokens appear but do parse redirections.
	  - If memory fails, we set p->error_status = PARSE_MEMORY_ERROR.
	  - If we find a redirection operator but no WORD after it for the filename,
		we set p->error_status = PARSE_SYNTAX_ERROR.

	CODE FLOW (Simplified):
	  1) Gather all WORD tokens into a single string 'cmd_args'.
	  2) parse_redirections => attach them to a linked list for io_redirects
	  3) create an AST_COMMAND node and return it.
*/
t_ast_node	*parse_command(t_parser *p)
{
	t_io_node	*io_list;
	t_ast_node	*cmd_node;
	char		*cmd_args;

	if (!p->current_token || p->error_status != PARSE_OK)
		return (NULL);
	io_list = NULL;
	cmd_args = build_command_string(p);
	if (!cmd_args || p->error_status != PARSE_OK)
		return (NULL);
	if (!process_redirections(p, &cmd_args, &io_list)
		|| p->error_status != PARSE_OK)
	{
		cleanup_resources(cmd_args, io_list);
		return (NULL);
	}
	cmd_node = handle_ast_command_creation(cmd_args, io_list, p);
	free(cmd_args);
	return (cmd_node);
}

/*
📌 append_to_buffer(buf, word_value)
Purpose: Appends a new word to a growing buffer,
resizing it dynamically if needed.

Steps:
1) Calculates the length of `word_value`.
2) Ensures the buffer has enough space:
   - If needed, calls `resize_buffer()` to increase `buf->size`.
   - If memory allocation fails, returns `0`.
3) Adds a space before appending `word_value`,
but only if it's **not the first word**.
4) Copies `word_value` into `buf->data`,
updates `buf->pos`, and null-terminates the string.
5) Returns `1` on success, `0` on failure.

Example:
- **Initial buffer:** `"echo"`
- `append_to_buffer(buf, "hello")` → `"echo hello"`
- `append_to_buffer(buf, "world")` → `"echo hello world"`

Effect:
- Ensures words are correctly spaced.
- Dynamically resizes the buffer when needed.
- Maintains a **null-terminated** string for safe usage.
*/
static int	append_to_buffer(t_buffer *buf, const char *word_value)
{
	size_t	word_len;
	char	*tmp;

	word_len = ft_strlen(word_value);
	while (buf->pos + word_len + 2 >= buf->size)
	{
		tmp = resize_buffer(buf->data, &(buf->size));
		if (!tmp)
			return (0);
		buf->data = tmp;
	}
	if (buf->pos > 0)
		buf->data[buf->pos++] = ' ';
	ft_memcpy(&buf->data[buf->pos], word_value, word_len);
	buf->pos += word_len;
	buf->data[buf->pos] = '\0';
	return (1);
}

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

int	process_redirections(t_parser *p, char **cmd_args, t_io_node **io_list)
{
	char	*new_args;
	char	*tmp;

	parse_redirections(p, io_list);
	while (p->current_token && p->current_token->type == TOKEN_WORD)
	{
		new_args = build_command_string(p);
		if (!new_args)
			return (0);
		tmp = ft_strjoin(" ", new_args);
		free(new_args);
		if (!tmp)
			return (0);
		new_args = ft_strjoin(*cmd_args, tmp);
		free(tmp);
		free(*cmd_args);
		if (!new_args)
			return (0);
		*cmd_args = new_args;
		parse_redirections(p, io_list);
	}
	return (p->error_status == PARSE_OK);
}
