/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:00:56 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/03/25 12:15:17 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"


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

// void handle_child_exit(int sig)
// {
//     (void)sig;  // Mark parameter as unused
//     int status;
//     pid_t pid;

//     while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
//     {
//         printf("Child process %d exited. Terminating parent.\n", pid);
//         exit(EXIT_SUCCESS);
//     }
// }

void	setup_signal_handlers(void)
{
	signal(SIGINT, handle_sigint);  // `Ctrl-C` için
	signal(SIGQUIT, handle_sigquit);  // `Ctrl-\` için
	//signal(SIGCHLD, handle_child_exit);
}

void parse_and_process_command(t_shell *shell, char ***envp_copy)
{
	int num_commands;

	if (*(shell->line) == '\0')
	{
		free(shell->line);
		shell->line = NULL;
		return ;
	}
	shell->tokens = tokenize(shell->tokens, shell->line, shell->line_length);
	if (!shell->tokens)
	{
		//printf("Error: Tokenization failed.\n");
		cleanup_shell(shell);
		return ;
	}
	shell->ast = build_ast(shell->tokens); /* Build the AST */
	if (!shell->ast)
	{
		//printf("Error: Parsing failed.\n");
		cleanup_shell(shell);
		return ;
	}
	//debug_ast(shell->ast, 0);   // -------> FOR DUBEGGING
	// execute_ast(shell->ast); /* Uncomment for actual execution */
	//print_tokens(shell->tokens);  // -------> FOR DUBEGGING
	printf("Old exit code in shell: %d\n", shell->exit_code);
	num_commands = get_num_commands(shell);
	if (num_commands > 0)
		shell->exit_code = execute_commands(shell, num_commands, envp_copy);
	//printf("%c", (*envp_copy)[0][0]);
	printf("New exit code in shell: %d\n", shell->exit_code);
	cleanup_shell(shell);
}

int main(int argc, char **argv, char **envp)
{
	t_shell shell;
	char **envp_copy;

	envp_copy = copy_envp(envp);

	printf("\n\nOur A is %s\n\n", ft_getenv("A", &envp_copy));

	//ft_set("A=12", &envp_copy);
	//printf("\n\n\n\n");
	//ft_env(envp_copy);
	//ft_cd("src", &envp_copy);
	//printf("HELLO\n");
	//ft_env(envp_copy);
	(void)envp;
	(void)argv;
	if (argc != 1)
	{
		ft_putendl_fd("Too many args. Usage: ./minishell", 2);
		exit(1);
	}
	init_shell(&shell, &envp_copy);
	setup_signal_handlers();
	// pid_t pid = fork();
    // if (pid == 0) 
    // {
    //     // Child process
    //     printf("Child process %d running...\n", getpid());
    //     sleep(2);  // Simulate some work
    //     printf("Child process %d exiting...\n", getpid());
    //     exit(0);
    // } 
    // else if (pid > 0) 
    // {
    //     // Parent process
    //     printf("Parent process %d waiting...\n", getpid());
    //     while (1)
    //     {
    //         pause();  // Wait indefinitely for signals
    //     }
    // } 
    // else 
    // {
    //     perror("fork failed");
    //     return 1;
    // }
	while (1)
	{
		shell.line = get_input("minishell$ ");
		if (!shell.line) /* Exit condition (Ctrl+D or EOF) */
			break ;
		shell.line_length = ft_strlen(shell.line);
		parse_and_process_command(&shell, &envp_copy);
		//printf("%c", (envp_copy)[0][0]);
	}
	free_envp(envp_copy);
	envp_copy = NULL;
	cleanup_shell(&shell);
	return (0);
}

