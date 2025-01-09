#include "minishell.h"

/*****************************************************************************/
/*                         FREE THE AST RECURSIVELY                          */
/*****************************************************************************/
void free_ast(t_ast_node *root)
{
    if (!root)
        return;

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
        while (curr)
        {
            t_io_node *next = curr->next;
            if (curr->filename)
                free(curr->filename);
            free(curr);
            curr = next;
        }
    }

    /* finally free the node itself */
    free(root);
}



void free_io_list(t_io_node *io_list)
{
    while (io_list)
    {
        t_io_node *temp = io_list->next;

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