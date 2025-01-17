#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

void parse_and_process_command(t_shell *shell)
{
    if (*(shell->line) == '\0') /* Handle empty input */
    {
        printf("Error: Empty line\n");
        free(shell->line);
        shell->line = NULL;
        return;
    }
    shell->tokens = tokenize(shell->line); /* Tokenize the user input */
     if (!shell->tokens)
    {
        printf("Tokenization failed.\n");
        cleanup_shell(shell); /* Cleanup before returning */
        return;
    }
    shell->ast = build_ast(shell->tokens); /* Build the AST */
     if (!shell->ast)
    {
        printf("Parsing failed.\n");
        cleanup_shell(shell); /* Cleanup before returning */
        return;
    }
    debug_ast(shell->ast, 0);   // -------> FOR DUBEGGING
    // execute_ast(shell->ast); /* Uncomment for actual execution */
    print_tokens(shell->tokens);  // -------> FOR DUBEGGING
    cleanup_shell(shell); /* Cleanup resources for this command */
}

int main(int argc, char **argv, char **envp)
{
    t_shell shell; /* Our shell struct containing 'line' and 'tokens' */

    (void)envp; /* If not used, silence warnings */
    (void)argv;
    if (argc != 1)
    {
        ft_putendl_fd("Too many args. Usage: ./minishell", 2);
        exit(1);
    }
    init_shell(&shell);
    while (1)
    {
        shell.line = get_input("minishell$ "); /* Directly call get_input */
        if (!shell.line) /* Exit condition (Ctrl+D or EOF) */
            break;
        parse_and_process_command(&shell);
    }
    cleanup_shell(&shell);
    return 0;
}
