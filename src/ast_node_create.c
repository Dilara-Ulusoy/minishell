#include "minishell.h"

t_ast_node_type get_ast_node_type(t_token_type operator_type)
{
	if (operator_type == TOKEN_AND)
		return AST_AND;
	else if (operator_type == TOKEN_OR)
		return AST_OR;
	else if (operator_type == TOKEN_PIPE)
		return AST_PIPE;
	else
		 return AST_INVALID;
}

/*
   create_ast_operator_node:
   - For operators like &&, ||, |
   - left and right subtrees are assigned
   - cmd_args = NULL, io_redirects = NULL
*/
t_ast_node *create_ast_operator_node(t_ast_node_type type, t_ast_node *left, t_ast_node *right)
{
    t_ast_node *node = (t_ast_node *)malloc(sizeof(t_ast_node));
    if (!node)
        return NULL;

    node->node_type = type;
    node->cmd_args = NULL;
    node->io_redirects = NULL;
    node->left = left;
    node->right = right;
    return node;
}


/*****************************************************************************/
/*             create_ast_command_node & free_io_list (for reference)        */
/*****************************************************************************/
/*
   create_ast_command_node(cmd_args, io_list):
   - Allocates an AST node of type AST_COMMAND.
   - Duplicates 'cmd_args' if non-empty, assigns 'io_list' to node->io_redirects.
   - left/right remain NULL because a command node doesn't have children.

   free_io_list:
   - Iterate over the t_io_node linked list, free each filename, and free the node.
*/

/*
    CONCLUSION:
    The parse_command function is simplified:
    - Collect words in a small local buffer (or a dynamic approach in real code).
    - parse_redirections handles <, >, >>, << and updates the i/o list.
    - We build an AST_COMMAND node with the final strings.

    For advanced shells, you'd handle expansions, environment variables,
    aliasing, etc. But this is enough to demonstrate a beginner-level approach
    to building a command node from tokens.
*/
t_ast_node *create_ast_command_node(const char *cmd_text, t_io_node *io_list)
{
    t_ast_node *node = (t_ast_node *)malloc(sizeof(t_ast_node));
    if (!node)
        return NULL;

    node->node_type = AST_COMMAND;
    if (cmd_text && *cmd_text)
    {
        node->cmd_args = ft_strdup(cmd_text);
        if (!node->cmd_args)
        {
            free(node);
            return NULL;
        }
    }
    else
    {
        /* no command text => empty string or possible "commandless" redirection */
        node->cmd_args = NULL;
    }
    node->io_redirects = io_list;
    node->left = NULL;
    node->right = NULL;
    return node;
}
// Helper function: Create a new IO node
t_io_node *create_io_node(t_io_type kind, const char *filename, t_parser *p)
{
    t_io_node *new_io = (t_io_node *)malloc(sizeof(t_io_node));
    if (!new_io)
    {
        p->error_status = PARSE_MEMORY_ERROR;
        return NULL;
    }

    new_io->io_kind = kind;
    new_io->filename = ft_strdup(filename);
    new_io->next = NULL;

    if (!new_io->filename)
    {
        free(new_io);
        p->error_status = PARSE_MEMORY_ERROR;
        return NULL;
    }
    return new_io;
}

// Helper function: Attach a new IO node to the IO list
void attach_io_node(t_io_node **io_list, t_io_node *new_io)
{
    if (!*io_list)
    {
        *io_list = new_io;
    }
    else
    {
        t_io_node *tmp = *io_list;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new_io;
    }
}
