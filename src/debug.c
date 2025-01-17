#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>


/*
    shell struct vars:
       - shell.line   => user input (dynamically allocated)
       - shell.tokens => token list
    Make sure you have definitions for these in your "minishell.h" or wherever
    you define t_shell, t_token, etc.
*/

void debug_print_io_list(const t_io_node *io_list, int depth)
{
    /*
       For each redirection, print its type and filename.
       Example: "REDIR: IO_IN -> file.txt" or "REDIR: IO_OUT -> out.txt"
    */
    const t_io_node *curr = io_list;
    while (curr)
    {
        /* Indent to show hierarchy */
        for (int i = 0; i < depth; i++)
            printf("  ");

        switch (curr->io_kind)
        {
            case IO_IN:       printf("REDIR: IO_IN    -> \"%s\"\n", curr->filename); break;
            case IO_OUT:      printf("REDIR: IO_OUT   -> \"%s\"\n", curr->filename); break;
            case IO_APPEND:   printf("REDIR: IO_APPEND-> \"%s\"\n", curr->filename); break;
            case IO_HEREDOC:  printf("REDIR: IO_HEREDOC-> \"%s\"\n", curr->filename); break;
        }
        curr = curr->next;
    }
}

void debug_ast(const t_ast_node *root, int depth)
{
    /* If the tree is empty, just return. */
    if (!root)
        return;

    /* Print indentation for this depth level. */
    for (int i = 0; i < depth; i++)
        printf("  ");

    /* Check node type and print accordingly. */
    switch (root->node_type)
    {
        case AST_COMMAND:
        {
            /* It's a command node => print cmd_args. */
            if (root->cmd_args && *(root->cmd_args))
                printf("CMD: %s [depth=%d]\n", root->cmd_args, depth);
            else
                printf("CMD: (no command text) [depth=%d]\n", depth);

            /* If there are redirections, print them as well. */
            if (root->io_redirects)
                debug_print_io_list(root->io_redirects, depth + 1);
            break;
        }
        case AST_AND:
            printf("AST_AND [depth=%d]\n", depth);
            /* Recursively print left and right subtrees with depth+1. */
            debug_ast(root->left, depth + 1);
            debug_ast(root->right, depth + 1);
            break;
        case AST_OR:
            printf("AST_OR [depth=%d]\n", depth);
            debug_ast(root->left, depth + 1);
            debug_ast(root->right, depth + 1);
            break;
        case AST_PIPE:
            printf("AST_PIPE [depth=%d]\n", depth);
            debug_ast(root->left, depth + 1);
            debug_ast(root->right, depth + 1);
            break;
        default:
            /* Possibly for debugging or fallback, shouldn't happen if we've covered all node types. */
            printf("UNKNOWN NODE TYPE [depth=%d]\n", depth);
            break;
    }
}

