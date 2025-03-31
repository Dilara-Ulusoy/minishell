
#include "minishell.h"
#include "execution.h"

static int	command_not_found(t_cmd_parts **cmd_parts, t_args *arg_struct)
{
	if ((*cmd_parts)->cmd_array[0] == NULL)
	{
		//ft_putstr_fd(arg_struct->argv[i], 2);
		ft_putstr_fd(": command not found\n", 2);
	}
	else
	{
		ft_putstr_fd((*cmd_parts)->cmd_array[0], 2);
		ft_putstr_fd(": command not found\n", 2);
	}
	//free_cmd_parts(cmd_parts);
	//cmd_parts = NULL;
	//free_array((void **)(cmd_parts->cmd_array), 0, -1);
	free_cmd_parts(cmd_parts);
	// if (cmd_parts != NULL)
	// {
	// 	free_cmd_parts(cmd_parts);
	// 	cmd_parts = NULL;
	// }
	free(arg_struct->pids);
	free_envp(arg_struct->envp);
	free(arg_struct);
	// if (arg_struct != NULL)
	// {
	// 	if (arg_struct->pids != NULL)
	// 	{
	// 		free(arg_struct->pids);
	// 		arg_struct->pids = NULL;
	// 	}
	// 	if (arg_struct->fd != NULL)
	// 	{
	// 		free_array((void **)arg_struct->fd, 1, arg_struct->argc + 1);
	// 		arg_struct->fd = NULL;
	// 		//free(arg_struct->fd);
	// 	}
	// 	free(arg_struct);
	// 	arg_struct = NULL;
	// }
	return (127);
}

int	get_exit_code(void)
{
	if (errno == 13 || errno == 21)
		return (126);
	if (errno == 2)
		return (127);
	else
		return (errno);
}

// static int	execute_command(char *path, t_cmd_parts **cmd_parts,
// 							t_args *arg_struct)
// {
// 	int	fd;

// 	fd = open(path, O_DIRECTORY);
// 	if (fd != -1)
// 	{
// 		close(fd);
// 		ft_putstr_fd(path, 2);
// 		ft_putstr_fd(": ", 2);
// 		ft_putstr_fd(strerror(21), 2);
// 		// Free before returning
// 		free(path);
// 		free_cmd_parts(cmd_parts);
// 		free(arg_struct->pids);
// 		free_array((void **)arg_struct->fd, 1, arg_struct->argc + 1);
// 		free(arg_struct);
// 		//free(arg_struct->pids);
// 		//free(arg_struct);
// 		//free_array((void **)(cmd_parts->cmd_array), 0, -1);
// 		return (126);
// 	}
// 	else
// 	{
// 		execve(path, (*cmd_parts)->cmd_array, arg_struct->envp);
// 		perror((*cmd_parts)->cmd_array[0]);
// 	}
// 	// Free before returning
// 	free(path);
// 	free_cmd_parts(cmd_parts);
// 	free(arg_struct->pids);
// 	free_array((void **)arg_struct->fd, 1, arg_struct->argc + 1);
// 	free(arg_struct);
// 	//free(arg_struct->pids);
// 	//free(arg_struct);
// 	//free_array((void **)(cmd_parts->cmd_array), 0, -1);
// 	return (get_exit_code());
// }

static int	execute_command(char *path, t_cmd_parts **cmd_parts,
							t_args *arg_struct)
{
	int	fd;
	//printf("Path: %s\n", path);
	//printf("cmdprts: %s\n", (*cmd_parts)->cmd_array[0]);
	fd = open(path, O_DIRECTORY);
	if (fd != -1)
	{
		close(fd);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(21), 2);
		// Free before returning
		if (path != (*cmd_parts)->cmd_array[0])  // Added to avoid double free in the case /test_files
			free(path);
		free_cmd_parts(cmd_parts);
		free(arg_struct->pids);
		//free_array((void **)arg_struct->fd, 1, arg_struct->argc - 1);
		free_envp(arg_struct->envp);
		free(arg_struct);
		//free(arg_struct->pids);
		//free(arg_struct);
		//free_array((void **)(cmd_parts->cmd_array), 0, -1);
		return (126);
	}
	else
	{
		execve(path, (*cmd_parts)->cmd_array, arg_struct->envp);
		perror((*cmd_parts)->cmd_array[0]);
	}
	// Free before returning
	if (path != (*cmd_parts)->cmd_array[0])  // Added to avoid double free in the case /test_files
		free(path);
	free_cmd_parts(cmd_parts);
	free(arg_struct->pids);
	//free_array((void **)arg_struct->fd, 1, arg_struct->argc - 1);
	free_envp(arg_struct->envp);
	free(arg_struct);
	//free(arg_struct->pids);
	//free(arg_struct);
	//free_array((void **)(cmd_parts->cmd_array), 0, -1);
	return (get_exit_code());
}

void ft_add_envVar(char *var_eq_value, char ***envp)
{
	if (!envp || !*envp) return; // Safety check

	//(*envp)[0][0] = 'A';
	size_t count = 0;
    while ((*envp)[count] != NULL) {
        count++;
    }

    char **new_envp = malloc((count + 2) * sizeof(char *));
    if (!new_envp) {
        perror("malloc failed");
        return ;
    }

    for (size_t i = 0; i < count; i++) {
        new_envp[i] = ft_strdup((*envp)[i]);
        if (!new_envp[i]) {
            perror("strdup failed");
            while (i > 0) free(new_envp[--i]);
            free(new_envp);
            return ;
        }
    }
	new_envp[count] = ft_strdup(var_eq_value);
	if (!new_envp[count]) {
        perror("ft_strdup failed");
        while (count > 0) free(new_envp[--count]);
        free(new_envp);
        return;
    }
    new_envp[count + 1] = NULL;  // Null-terminate the array
	// Free old envp before reassigning
	if (*envp)
	{
    	for (size_t i = 0; (*envp)[i] != NULL; i++) {
        	free((*envp)[i]);
    	}
    	free(*envp);
	}
	(*envp) = new_envp;
}

static int	run_single_builtin(t_cmd_parts **cmd_parts, t_shell *shell, char ***envp)
{
	char	*path;
	char	**command_array;
	int ret;

	command_array = (*cmd_parts)->cmd_array;
	path = find_command_path(command_array[0], *envp);
	ret = check_and_run_builtins_single(shell, cmd_parts, envp);
	free(path);
	return (ret);
}

static int	run_pid(t_args *arg_struct, t_cmd_parts **cmd_parts, t_shell *shell, int is_builtin)
{
	char	*path;
	char	**command_array;
	int		check_status;
	int ret;

	//ft_addA(&(arg_struct->envp));
	command_array = (*cmd_parts)->cmd_array;
	path = find_command_path(command_array[0], arg_struct->envp);
	check_status = set_pipe((*cmd_parts), arg_struct, path, is_builtin);
	if (check_status == -1)
	{
		//free_array((void **)command_array, 0, -1);
		// if (cmd_parts)
		// {
		// 	free_cmd_parts(cmd_parts);
		// 	cmd_parts = NULL;
		// }
		free_cmd_parts(cmd_parts);
		free(path);
		return (EXIT_FAILURE); // exit
	}
	if (is_builtin == 1)
	{
		ret = check_and_run_builtins(shell, cmd_parts, arg_struct);
		free(path);
		free(arg_struct->pids);
		free_envp(arg_struct->envp);
		free(arg_struct);
		return (ret);
	}
	else if (path != NULL)
		return (execute_command(path, cmd_parts, arg_struct)); // exit
	else
		return (command_not_found(cmd_parts, arg_struct)); // exit
}

int	execute_commands(t_shell *shell, int num_commands, char ***envp)
{
	int		wstatus;
	int		j;
	//int		k;
	t_args	*arg_struct;
	t_cmd_parts *cmd_parts;
	//int used_execve[num_commands];
	//int last_exit_code;
	//pid_t exited_pid;
	int exit_code;

	if (num_commands == 1)
	{
		cmd_parts = get_command_array(shell->tokens, 1);
		cmd_parts->num_commands = num_commands;
		if (is_builtin(cmd_parts))
		{
			int original_stdin = dup(STDIN_FILENO);
			int original_stdout = dup(STDOUT_FILENO);
			exit_code = set_pipe_single_builtin(cmd_parts);
			if (exit_code != 0)
			{
				dup2(original_stdin, STDIN_FILENO);
				dup2(original_stdout, STDOUT_FILENO);
				close(original_stdin);
				close(original_stdout);

				free_cmd_parts(&cmd_parts);
				cleanup_shell(shell);
				return (exit_code);
			}
			exit_code = run_single_builtin(&cmd_parts, shell, envp);

			dup2(original_stdin, STDIN_FILENO);
			dup2(original_stdout, STDOUT_FILENO);
			close(original_stdin);
			close(original_stdout);

			free_cmd_parts(&cmd_parts);
			cleanup_shell(shell);
			return (exit_code);
		}
		free_cmd_parts(&cmd_parts);
	}
	//(*envp)[0][0] = 'H';
	//ft_addA(envp);
	//ft_env(*envp);
	//printf("num_commands: %d\n", num_commands);
	arg_struct = prepare_struct(num_commands, *envp);
	//ft_env(arg_struct->envp);
	//ft_addA(&(arg_struct->envp)); // NOPE
	if (arg_struct == NULL)
		exit(EXIT_FAILURE);
	//setup_signal_handlers();
	j = -1;
	while (++j < num_commands)
	{
		cmd_parts = get_command_array(shell->tokens, j + 1);
		cmd_parts->num_commands = num_commands;
		// if (is_builtin(cmd_parts) == 1)
		// 	used_execve[j] = 0;
		// else
		// 	used_execve[j] = 1;
		arg_struct->pids[j] = fork();
		if (arg_struct->pids[j] < 0)
		{
			free_cmd_parts(&cmd_parts);
			// if (cmd_parts)
			// {
			// 	free_cmd_parts(cmd_parts);
			// 	cmd_parts = NULL;
			// }
			cleanup_shell(shell);
			exit(display_error_message(2, arg_struct)); // exit
		}
		if (arg_struct->pids[j] == 0)
		{
			exit_code = run_pid(arg_struct, &cmd_parts, shell, is_builtin(cmd_parts));
			free_cmd_parts(&cmd_parts);
			// if (cmd_parts)
			// {
			// 	free_cmd_parts(cmd_parts);
			// 	cmd_parts = NULL;
			// }
			cleanup_shell(shell);
			exit(exit_code);
		}
		free_cmd_parts(&cmd_parts); // Always free after use
		// if (cmd_parts)
		// {
		// 	free_cmd_parts(cmd_parts);
		// 	cmd_parts = NULL;
		// }
	}
	close_and_free(arg_struct, 0);
	j = -1;
	while (++j < num_commands)
		waitpid(arg_struct->pids[j], &wstatus, 0);
	free(arg_struct->pids);
	free(arg_struct);
	if (WIFEXITED(wstatus))
		return (WEXITSTATUS(wstatus));
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