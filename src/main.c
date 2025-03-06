/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:00:56 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/03/06 21:55:37 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"

/*
void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();  // Yeni satır başına geç
	rl_replace_line("", 0);  // Mevcut girdiyi temizle
	rl_redisplay();  // Yeni prompt'u göster
}

void	handle_sigquit(int sig)
{
	(void)sig;
}

void	setup_signal_handlers(void)
{
	signal(SIGINT, handle_sigint);  // `Ctrl-C` için
	signal(SIGQUIT, handle_sigquit);  // `Ctrl-\` için
}
*/


void parse_and_process_command(t_shell *shell, char **envp)
{
	int num_commands;

	if (*(shell->line) == '\0') /* Handle empty input */
	{
		free(shell->line);
		shell->line = NULL;
		return ;
	}
	shell->tokens = tokenize(shell->tokens, shell->line, shell->line_length);
	if (!shell->tokens)
	{
		printf("Error: Tokenization failed.\n");
		cleanup_shell(shell);
		return ;
	}
	shell->ast = build_ast(shell->tokens); /* Build the AST */
	if (!shell->ast)
	{
		printf("Error: Parsing failed.\n");
		cleanup_shell(shell);
		return ;
	}
	//find_ast_pipes(shell->ast);  // -------> FOR DUBEGGING
	//debug_ast(shell->ast, 0);   // -------> FOR DUBEGGING
	// execute_ast(shell->ast); /* Uncomment for actual execution */
	//print_tokens(shell->tokens);  // -------> FOR DUBEGGING
	num_commands = get_num_commands(shell);
	if (num_commands > 0)
		execute_commands(shell, num_commands, envp);
	cleanup_shell(shell);
}

int main(int argc, char **argv, char **envp)
{
	t_shell shell;

	(void)envp;
	(void)argv;
	if (argc != 1)
	{
		ft_putendl_fd("Too many args. Usage: ./minishell", 2);
		exit(1);
	}
	init_shell(&shell);
	//setup_signal_handlers();
	while (1)
	{
		shell.line = get_input("minishell$ ");
		if (!shell.line) /* Exit condition (Ctrl+D or EOF) */
			break ;
		shell.line_length = ft_strlen(shell.line);
		parse_and_process_command(&shell, envp);
	}
	cleanup_shell(&shell);
	return (0);
}

