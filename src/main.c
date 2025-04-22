/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:00:56 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/04/21 20:41:59 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"

void	parse_and_process_command(t_shell *shell, char ***envp_copy)
{
	int	num_commands;

	if (*(shell->line) == '\0')
	{
		free(shell->line);
		shell->line = NULL;
		return ;
	}
	shell->tokens = tokenize(shell->tokens, shell->line,
			shell->line_length, shell);
	if (!shell->tokens)
	{
		//printf("Error: Tokenization failed.\n");
		cleanup_shell(shell);
		return ;
	}
	shell->ast = build_ast(shell->tokens, shell); /* Build the AST */
	if (!shell->ast)
	{
		//printf("Error: Parsing failed.\n");
		cleanup_shell(shell);
		return ;
	}
	//debug_ast(shell->ast, 0);   // -------> FOR DUBEGGING
	//print_tokens(shell->tokens);  // -------> FOR DUBEGGING
	num_commands = get_num_commands(shell->tokens);
	if (num_commands > 0)
		shell->exit_code = execute_commands(shell, num_commands, envp_copy);
	cleanup_shell(shell);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	char	**envp_copy;

	shell.exit_code = 0;
	envp_copy = copy_envp(envp);
	(void)argv;
	if (argc != 1)
	{
		ft_putendl_fd("Too many args. Usage: ./minishell", 2);
		exit(1);
	}
	init_shell(&shell, &envp_copy);
	init_term_and_signal(argc, argv, &shell.exit_code);
	while (1)
	{
		set_signals(&shell.exit_code, SIGNAL_PARENT);
		g_signal = 1;
		shell.line = get_input("minishell$ ");
		g_signal = 0;
		if (!shell.line)
		{
			shell.exit_code = 0;
			printf("exit\n");
			break ;
		}
		shell.line_length = ft_strlen(shell.line);
		set_signals(NULL, SIGNAL_CHILD);
		parse_and_process_command(&shell, &envp_copy);
	}
	free_envp(envp_copy);
	envp_copy = NULL;
	cleanup_shell(&shell);
	return (0);
}
