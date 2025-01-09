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

static void debug_print_io_list(const t_io_node *io_list, int depth)
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


void print_tokens(const t_token *head)
{
    const t_token *curr = head;
    while (curr)
    {
        printf("TOKEN: type=%d, value= %s\n", curr->type, curr->value);
        curr = curr->next;
    }
}

void free_tokens(t_token **head)
{
    t_token *temp;

    while (*head)
    {
        temp = (*head)->next;
        if ((*head)->value)
            free((*head)->value);
        free(*head);
        *head = temp;
    }
}

/*
    get_input:
    - Prompts the user for input via readline.
    - If user presses Ctrl+D, readline returns NULL.
    - We trim leading/trailing spaces with ft_strtrim (ensure it's available).
    - Return the trimmed string. The caller must free it.
*/
char *get_input(void)
{
    char *line = readline("minishell$ ");
    if (!line) /* If user pressed Ctrl+D or EOF occurred */
        return NULL;

    /* If line is not empty, add to history */
    if (*line)
        add_history(line);

    /* Trim whitespace (assuming ft_strtrim is defined somewhere) */
    char *trimmed_line = ft_strtrim(line, " \t\n");
    free(line);

    return trimmed_line;
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
        shell->line = NULL;
    }
    if (shell->tokens)
    {
        free_tokens(&shell->tokens);
        shell->tokens = NULL;
    }
}

/*
    Usage:
      ./minishell
    If user passes extra arguments, we print usage error and exit(1).
*/
int main(int argc, char **argv, char **envp)
{
    t_shell shell;  /* Our shell struct containing 'line' and 'tokens' */

    (void)envp;     /* If not used, silence warnings */
    (void)argv;
    if (argc != 1)
    {
        ft_putendl_fd("Too many args. Usage: ./minishell", 2);
        exit(1);
    }
    shell.line = NULL;
    shell.tokens = NULL;
    while (1)
    {
        shell.line = get_input(); /* Get user input (trimmed) */
        if (!shell.line)
        {
            /* Ctrl+D or EOF => exit the shell */
            printf("Exit\n");
            cleanup_shell(&shell);
            break;
        }
        /* If user typed an empty string after trimming, just free and continue */
        if (*(shell.line) == '\0')
        {
            free(shell.line);
            shell.line = NULL;
            continue;
        }

        /* Tokenize the user input */
        shell.tokens = tokenize(shell.line);
        t_ast_node *ast_root = build_ast(shell.tokens);
        if (!ast_root)
        {
            printf("Parsing failed.\n");
        }
        else
        {
            debug_ast(ast_root, 0);
            //execute_ast(ast_root);
        }
        //print_tokens(shell.tokens);
        free_tokens(&shell.tokens);
        shell.tokens = NULL;
        /* Free the input line as well */
        free(shell.line);
        shell.line = NULL;
        free_ast(ast_root);
    }
    return 0;

}
