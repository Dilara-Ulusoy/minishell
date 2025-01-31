#include "minishell.h"

/**
 * Initializes the shell structure.
 */
void	init_shell(t_shell *shell)
{
	shell->line = NULL;
	shell->tokens = NULL;
	shell->current_token = NULL;
	shell->ast = NULL;
	shell->parser = NULL;
	shell->exit_code = 0;
	shell->line_length = 0;
}

/*
    get_input:
    - Prompts the user for input via readline.
    - If user presses Ctrl+D, readline returns NULL.
    - We trim leading/trailing spaces with ft_strtrim (ensure it's available).
    - Return the trimmed string. The caller must free it.
*/
char	*get_input(const char *prompt)
{
	char	*line;
	char	*trimmed_line;

	line = readline(prompt);
	if (!line) /* If user pressed Ctrl+D or EOF occurred */
		return (NULL);
	trimmed_line = ft_strtrim(line, " \t\n");     /* Trim whitespace (assuming ft_strtrim is defined somewhere) */
	free(line);
	if (!trimmed_line)
	{
		ft_putstr_fd("Error: Memory allocation failed\n", STDERR_FILENO);
		return (NULL);
	}
	if (*trimmed_line)
		add_history(trimmed_line);
	return (trimmed_line);
}

