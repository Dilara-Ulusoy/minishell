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
     // AST Node türüne göre yazdırma
    if (root->node_type == AST_AND || root->node_type == AST_OR || root->node_type == AST_PIPE) {
        /*
        printf("Freed AST Operator Node: %p ----> %s\n",
               (void *)root,
               root->node_type == AST_AND ? "AND" : root->node_type == AST_OR ? "OR" : "PIPE");
        */
    } else if (root->node_type == AST_COMMAND) {
        /*
        printf("Freed AST Command Node: %p ----> %s\n",
               (void *)root,
               root->cmd_args ? root->cmd_args : "(null)");
               */
    }
    /* free left and right subtrees */
    free_ast(root->left);
    free_ast(root->right);
    /* free command text if command node */
    if (root->cmd_args)
        free(root->cmd_args);
    /* free i/o redirection list */
    if (root->io_redirects)
    {
        t_io_node *curr = root->io_redirects;
        free_io_list(curr);
    }
    /* finally free the node itself */
    free(root);
}

void free_io_list(t_io_node *io_list)
{
    while (io_list)
    {
        t_io_node *temp = io_list->next;
        printf("Freeing I/O List for AST Node: %p ----> %s\n", (void *)io_list, io_list->filename);
        /*
           Each t_io_node may have a dynamically allocated 'filename'.
           We must free it.
        */
        if (io_list->filename)
            free(io_list->filename);
        /*
           Finally, free the I/O node structure itself.
        */
        free(io_list);
        /*
           Move on to the next node in the list.
        */
        io_list = temp;
    }
}
/*
    cleanup_shell:
    - Frees shell->line (if non-null).
    - Frees token list shell->tokens (if non-null).
*/
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
        //printf("Freed AST: %p\n", (void *)shell->ast);
    }
}
