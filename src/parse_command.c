#include "minishell.h"

/*****************************************************************************/
/*                              parse_command                                 */
/*****************************************************************************/
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

static	t_ast_node *handle_ast_command_creation(char *cmd_args, t_io_node *io_list, t_parser *p)
{
	t_ast_node *cmd_node;
	cmd_node = create_ast_command_node(cmd_args, io_list);
	if (!cmd_node)
	{
		free_io_list(io_list);
		p->error_status = PARSE_MEMORY_ERROR;
		return NULL;
	}
	return (cmd_node);
}

t_ast_node	*parse_command(t_parser *p)
{
	t_io_node	*io_list;
	t_ast_node	*cmd_node;
	char		*cmd_args;

	if (!p->current_token || p->error_status != PARSE_OK)
		return NULL;

	io_list = NULL;

	/* 1) Build Command String */
	cmd_args = build_command_string(p);
	if (!cmd_args || p->error_status != PARSE_OK)
		return NULL;

	/* 2) PARSE REDIRECTIONS (like <, >, >>, <<). */
	if (!process_redirections(p, &cmd_args, &io_list) || p->error_status != PARSE_OK)
	{
		cleanup_resources(cmd_args, io_list);
		return NULL;
	}
	/* 3) CREATE THE AST COMMAND NODE. */
	cmd_node = handle_ast_command_creation(cmd_args, io_list, p);
	free(cmd_args);
	return cmd_node;
}

static char	*resize_buffer(char *buffer, size_t *buffer_size)
{
	size_t	new_size;
	char	*new_buffer;

	new_size = *buffer_size * 2;
	new_buffer = ft_calloc(new_size, 1);
	if (!new_buffer)
	{
		free(buffer);
		return NULL;
	}
	ft_memcpy(new_buffer, buffer, *buffer_size);
	free(buffer);
	*buffer_size = new_size;
	return new_buffer;
}

// This function appends a word to the buffer, resizing it if necessary.
static int append_to_buffer(char **buffer, size_t *buffer_size, size_t *buf_pos, const char *word_value, size_t word_len)
{
	// Tamponda yeterli alan yoksa genişlet
	while (*buf_pos + word_len + 2 >= *buffer_size)
	{
		*buffer = resize_buffer(*buffer, buffer_size);
		if (!*buffer)
			return 0; // Hata durumu
	}
	// Argümanları boşluk ile ayır
	if (*buf_pos > 0)
			(*buffer)[(*buf_pos)++] = ' ';

	// Yeni kelimeyi tampona kopyala
	ft_memcpy(&(*buffer)[*buf_pos], word_value, word_len);
	*buf_pos += word_len;
	(*buffer)[*buf_pos] = '\0'; // Sonlandırıcı karakter ekle

	return 1; // Başarı
}

char *build_command_string(t_parser *p)
{
	size_t buffer_size = 256;
	size_t buf_pos = 0;
	size_t word_len = 0;
	char *buffer;
	const char *word_value;

	buffer = ft_calloc(buffer_size, 1);
	if (!buffer)
	{
		p->error_status = PARSE_MEMORY_ERROR;
		return NULL;
	}
	while (p->current_token && p->current_token->type == TOKEN_WORD)
	{
		word_value = p->current_token->value;
		word_len = ft_strlen(word_value);
		if (!append_to_buffer(&buffer, &buffer_size, &buf_pos, word_value, word_len))
		{
			p->error_status = PARSE_MEMORY_ERROR;
			return NULL;
		}
		get_next_token(p); // Sonraki token'a geç
	}
	return buffer;
}
/**
 * process_redirections - Handles input/output redirections and updates command arguments.
 *
 * This function processes redirection operators (e.g., <, >, >>, <<) and ensures
 * that command arguments are correctly updated as tokens are parsed.
 *
 * Behavior:
 * 1. Initial Parsing:
 *    - Calls `parse_redirections` to handle any redirection tokens and update `io_list`.
 *
 * 2. Processing Remaining Tokens:
 *    - While the current token is of type `TOKEN_WORD`, it:
 *      - Builds the command string using `build_command_string`.
 *      - Frees the previous command string and updates it with the new one.
 *      - Calls `parse_redirections` to handle additional redirections.
 *
 * 3. Error Handling:
 *    - If `build_command_string` fails, the function returns 0 (failure).
 *    - Finally, it checks `p->error_status` to confirm parsing success.
 *
 * @param p         Pointer to the parser structure containing the current token and state.
 * @param cmd_args  Double pointer to the command arguments string (updated during parsing).
 * @param io_list   Pointer to the list of I/O redirection nodes to store redirection info.
 *
 * @return          1 if parsing was successful, 0 if an error occurred.
 */
int	process_redirections(t_parser *p, char **cmd_args, t_io_node **io_list)
{
	char	*new_args;

	parse_redirections(p, io_list);
	while (p->current_token && p->current_token->type == TOKEN_WORD)
	{
		new_args = build_command_string(p);
		if (!new_args)
			return (0);

		free(*cmd_args);
		*cmd_args = new_args;
		parse_redirections(p, io_list);
	}
	if(p->error_status != PARSE_OK)
		return (0);
	return (1);
}

void	cleanup_resources(char *cmd_args, t_io_node *io_list)
{
	if (cmd_args)
		free(cmd_args);
	if (io_list)
		free_io_list(io_list);
}


