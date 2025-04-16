#include "minishell.h"
#include "execution.h"


int	execute_commands(t_shell *shell, int num_commands, char ***envp)
{
	int				wstatus;
	int				j;
	int				exit_code;
	t_args			*arg_struct;
	t_cmd_parts		*cmd_parts;
	int printed_newline;

	if (num_commands == 1)
	{
		cmd_parts = get_command_array(shell->tokens, 1);
		cmd_parts->num_commands = num_commands;
		if (is_builtin(cmd_parts))
		{
			//int original_stdin = dup(STDIN_FILENO);
			//int original_stdout = dup(STDOUT_FILENO);
			int		original_fd[2];
			original_fd[0] = dup(STDIN_FILENO);
			original_fd[1] = dup(STDOUT_FILENO);
			exit_code = set_pipe_single_builtin(cmd_parts);
			if (exit_code != 0)
			{
				//dup2(original_stdin, STDIN_FILENO);
				//dup2(original_stdout, STDOUT_FILENO);
				//close(original_stdin);
				//close(original_stdout);
				dup2(original_fd[0], STDIN_FILENO);
				dup2(original_fd[1], STDOUT_FILENO);
				close(original_fd[0]);
				close(original_fd[1]);
				free_cmd_parts(&cmd_parts);
				cleanup_shell(shell);
				return (exit_code);
			}
			exit_code = run_single_builtin(&cmd_parts, shell, envp, original_fd);
			dup2(original_fd[0], STDIN_FILENO);
			dup2(original_fd[1], STDOUT_FILENO);
			close(original_fd[0]);
			close(original_fd[1]);
			free_cmd_parts(&cmd_parts);
			cleanup_shell(shell);
			return (exit_code);
		}
		free_cmd_parts(&cmd_parts);
	}

	arg_struct = prepare_struct(num_commands, *envp);
	if (arg_struct == NULL)
		exit(EXIT_FAILURE);

	j = -1;
	while (++j < num_commands)
	{
		cmd_parts = get_command_array(shell->tokens, j + 1);
		cmd_parts->num_commands = num_commands;

		arg_struct->pids[j] = fork();
		if (arg_struct->pids[j] < 0)
		{
			free_cmd_parts(&cmd_parts);
			cleanup_shell(shell);
			exit(display_error_message(2, arg_struct));
		}
		if (arg_struct->pids[j] == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);

			exit_code = run_pid(arg_struct, &cmd_parts, shell, is_builtin(cmd_parts));
			free_cmd_parts(&cmd_parts);
			cleanup_shell(shell);
			exit(exit_code);
		}
		else
		{
			signal(SIGINT, SIG_IGN);
			signal(SIGQUIT, SIG_IGN);
		}
		free_cmd_parts(&cmd_parts);
	}

	close_and_free(arg_struct, 0);
	printed_newline = 0;
	j = -1;
	while (++j < num_commands)
	{
		waitpid(arg_struct->pids[j], &wstatus, 0);
		if (WIFSIGNALED(wstatus) && WTERMSIG(wstatus) == SIGINT && !printed_newline)
		{
			write(1, "\n", 1);
			printed_newline = 1;
		}
	}
	free(arg_struct->pids);
	free(arg_struct);
	if (WIFEXITED(wstatus))
		return (WEXITSTATUS(wstatus));
	else if (WIFSIGNALED(wstatus))
		return (128 + WTERMSIG(wstatus));
	return (EXIT_FAILURE);
}
	// // Wait for all child processes
	// last_exit_code = EXIT_FAILURE;
	// j = -1;
	// while (++j < num_commands)
	// {
	// 	exited_pid = waitpid(-1, &wstatus, 0);
	// 	if (exited_pid > 0 && WIFEXITED(wstatus))
	// 	{
	// 		exit_code = WEXITSTATUS(wstatus);
	// 		k = -1;
	// 		while (++k < num_commands)
	// 		{
	// 			if (arg_struct->pids[k] == exited_pid)
	// 			{
	// 				if (used_execve[k] == 1)
	// 					last_exit_code = exit_code;
	// 				else
	// 				{
	// 					if (arg_struct->pids != NULL)
	// 					{
	// 						free(arg_struct->pids);
	// 						arg_struct->pids = NULL;
	// 					}
	// 					if (arg_struct != NULL)
	// 					{
	// 						free(arg_struct);
	// 						arg_struct = NULL;
	// 					}
	// 					exit(exit_code); // Exit parent
	// 				}
	// 				break ;
	// 			}
	// 		}
	// 	}
	// }
	// // Cleanup and return last exit code
	// if (arg_struct->pids != NULL)
	// {
	// 	free(arg_struct->pids);
	// 	arg_struct->pids = NULL;
	// }
	// if (arg_struct != NULL)
	// {
	// 	free(arg_struct);
	// 	arg_struct = NULL;
	// }
	// return (last_exit_code);



// int	execute_commands(t_shell *shell, int num_commands, char **envp)
// {
//  	int		wstatus;
//  	int		j;
//  	t_args	*arg_struct;
//  	t_cmd_parts *cmd_parts;

// 	//printf("num_commands: %d\n", num_commands);
// 	arg_struct = prepare_struct(num_commands, envp);
// 	if (arg_struct == NULL)
// 		exit(EXIT_FAILURE);
// 	j = -1;
// 	while (++j < num_commands)
// 	{
// 		cmd_parts = get_command_array(shell->tokens, j + 1);
// 		cmd_parts->num_commands = num_commands;
// 		arg_struct->pids[j] = fork();
// 		if (arg_struct->pids[j] < 0)
// 			return (display_error_message(2, arg_struct)); // exit
// 		if (arg_struct->pids[j] == 0)
// 			run_pid(arg_struct, cmd_parts, shell, is_builtin(cmd_parts));
// 		free(cmd_parts);
// 	}
// 	close_and_free(arg_struct, 0);
// 	j = -1;
// 	while (++j < num_commands)
// 		waitpid(arg_struct->pids[j], &wstatus, 0);
// 	free(arg_struct->pids);
// 	free(arg_struct);
// 	if (WIFEXITED(wstatus))
// 		return (WEXITSTATUS(wstatus)); //exit
// 	return (EXIT_FAILURE); //exit
// }
