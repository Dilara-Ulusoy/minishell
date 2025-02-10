#include "minishell.h"

/*****************************************************************************/
/*                         FREE                                               */
/*****************************************************************************/

void free_tokens(t_token **head)
{
	t_token *temp;

	while (*head)
	{
		//printf("Freeing Token: %p -------> %s\n", (void *)(*head), (*head)->value ? (*head)->value : "(null)");
		temp = (*head)->next;
		if ((*head)->value)
			free((*head)->value);
		free(*head);
		*head = temp;
	}
}

void free_ast(t_ast_node *root)
{
	if (!root)
		return;
	if (root->left)
	{
		free_ast(root->left);
		root->left = NULL;
	}
	if (root->right)
	{
		free_ast(root->right);
		root->right = NULL;
	}
	if (root->cmd_args)
	{
		free(root->cmd_args);
		root->cmd_args = NULL;
	}
	if (root->io_redirects)
	{
		free_io_list(root->io_redirects);
		root->io_redirects = NULL;
	}
	free(root);
}

void free_io_list(t_io_node *io_list)
{
	while (io_list)
	{
		t_io_node *temp = io_list->next;
		printf("Freeing I/O List for AST Node: %p ----> %s\n", (void *)io_list, io_list->filename);
		if (io_list->filename)
			free(io_list->filename);
		free(io_list);
		io_list = temp;
	}
}

void cleanup_shell(t_shell *shell)
{
	if (shell->line)
	{
		free(shell->line);
		//printf("Freed Line: %p\n", (void *)shell->line);
		shell->line = NULL;
	}
	if (shell->tokens)
	{
		free_tokens(&shell->tokens);
		//printf("Freed Tokens: %p\n", (void *)shell->tokens);
		shell->tokens = NULL;
	}
	if(shell->ast)
	{
		free_ast(shell->ast);
		shell->ast = NULL;
		//printf("Freed AST: %p\n", (void *)shell->ast);
	}
}
