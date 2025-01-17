#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

/*
    get_input:
    - Prompts the user for input via readline.
    - If user presses Ctrl+D, readline returns NULL.
    - We trim leading/trailing spaces with ft_strtrim (ensure it's available).
    - Return the trimmed string. The caller must free it.
*/
char *get_input(const char *prompt)
{
    /* Use the custom prompt passed as a parameter */
    char *line = readline(prompt);
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

/**
 * Initializes the shell structure.
 */
void init_shell(t_shell *shell)
{
    shell->line = NULL;
    shell->tokens = NULL;
    shell->current_token = NULL;
    shell->ast = NULL;
    shell->parser = NULL;
    shell->exit_code = 0;
}

void handle_input(t_shell *shell)
{
    shell->line = get_input("minishell$ ");
    if (!shell->line) /* Ctrl+D or EOF */
    {
        printf("Exit\n");
        cleanup_shell(shell);
    }
}

void parse_and_process_command(t_shell *shell)
{
    if (*(shell->line) == '\0') /* Handle empty input */
    {
        printf("Error: Empty line\n");
        free(shell->line);
        shell->line = NULL;
        return;
    }
    /* Tokenize the user input */
    shell->tokens = tokenize(shell->line);
    /* Build the AST */
    shell->ast = build_ast(shell->tokens);
    if (!shell->ast)
    {
        printf("Parsing failed.\n");
    }
    else
    {
        debug_ast(shell->ast, 0); /* Debugging function */
        // execute_ast(shell->ast); /* Uncomment for actual execution */
    }
    /* Debugging tokens */
    print_tokens(shell->tokens);
    cleanup_shell(shell);     /* Cleanup resources for this command */
}

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
    init_shell(&shell);
    while (1)
    {
        handle_input(&shell);
        if (!shell.line) /* Exit condition (Ctrl+D or EOF) */
            break;
        parse_and_process_command(&shell);
    }
    return 0;
}
