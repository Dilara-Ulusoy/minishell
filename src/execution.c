/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 22:03:03 by htopa             #+#    #+#             */
/*   Updated: 2025/03/18 14:52:37 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"

void find_ast_pipes(t_ast_node *node)
{
	if (!node)
		return;
    if (node->node_type == AST_PIPE)
    {
        printf("✅ Found AST_PIPE node at %p\n", (void *)node);

        // Sol ve sağ düğümlerin cmd_args değerlerini yazdır
        if (node->left && node->left->cmd_args)
            printf("  ├── Left Command: %s\n", node->left->cmd_args);
        else
            printf("  ├── Left Command: NULL\n");

        if (node->right && node->right->cmd_args)
            printf("  └── Right Command: %s\n", node->right->cmd_args);
        else
            printf("  └── Right Command: NULL\n");
    }
    find_ast_pipes(node->left);
    find_ast_pipes(node->right);
}

void count_pipes(t_ast_node *node, int *i)
{
	if (!node)
		return ;
    if (node->node_type == AST_PIPE)
		(*i)++;
    count_pipes(node->left, i);
    count_pipes(node->right, i);
}

t_cmd_parts *count_tokens(const t_token *head, int command_number)
{
	t_cmd_parts *cmd_parts;
	int pipe_number;
	const t_token *curr; // = head;

	cmd_parts = malloc(sizeof(t_cmd_parts));
	pipe_number = 0;
	curr = head;
	while (pipe_number < (command_number -1))
	{
		while (curr && curr->type != TOKEN_PIPE)
			curr = curr->next;
		if (curr->type == TOKEN_PIPE)
		{
			pipe_number++;
			curr = curr->next;
		}
	}
	cmd_parts->n_cmd = 0;
	cmd_parts->n_in = 0;
	cmd_parts->n_out = 0;
    while (curr && curr->type != TOKEN_PIPE)
    {
		if (curr->type >= TOKEN_REDIR_IN && curr->type <= TOKEN_REDIR_HERE)
		{
			if (curr->type == TOKEN_REDIR_IN)
				(cmd_parts->n_in)++;
			if (curr->type == TOKEN_REDIR_OUT || curr->type == TOKEN_REDIR_APPEND)
				(cmd_parts->n_out)++;
			curr = curr->next->next;
		}
		if (curr && curr->type == TOKEN_WORD)
		{
			(cmd_parts->n_cmd)++;
        	curr = curr->next;
		}
    }
	return (cmd_parts);
}

t_cmd_parts *get_command_array(const t_token *head, int command_number)
{
	t_cmd_parts *cmd_parts;
	int i;
	int j;
	int k;
	int pipe_number;
	const t_token *curr;
	
	cmd_parts = count_tokens(head, command_number);
	//printf("Count of tokens: %d\n", cmd_parts->n_cmd);
	cmd_parts->cmd_array = malloc(sizeof(char *) * (cmd_parts->n_cmd + 1));
	cmd_parts->infiles_array = malloc(sizeof(char *) * (cmd_parts->n_in + 1));
	cmd_parts->outfiles_array = malloc(sizeof(char *) * (cmd_parts->n_out + 1));
	cmd_parts->outfiles_types = malloc(sizeof(int) * (cmd_parts->n_out));
	cmd_parts->command_number = command_number - 1;
	pipe_number = 0;
	curr = head;
	while (pipe_number < (command_number - 1))
	{
		while (curr && curr->type != TOKEN_PIPE)
			curr = curr->next;
		if (curr->type == TOKEN_PIPE)
		{
			pipe_number++;
			curr = curr->next;
		}
	}
	i = 0;
	j = 0;
	k = 0;
    while (curr && curr->type != TOKEN_PIPE)
    {
		if (curr->type >= TOKEN_REDIR_IN && curr->type <= TOKEN_REDIR_HERE)
		{
			if (curr->type == TOKEN_REDIR_IN)
			{
				cmd_parts->infiles_array[i] = curr->next->value;
				i++;
			}
			if (curr->type == TOKEN_REDIR_OUT || curr->type == TOKEN_REDIR_APPEND)
			{
				cmd_parts->outfiles_array[k] = curr->next->value;
				if (curr->type == TOKEN_REDIR_OUT)
					cmd_parts->outfiles_types[k] = 1;
				else
					cmd_parts->outfiles_types[k] = 2;
				k++;
			}
			curr = curr->next->next;
		}
		if (curr && curr->type == TOKEN_WORD)
		{
			cmd_parts->cmd_array[j] = curr->value;
			j++;
        	curr = curr->next;
		}
    }
	cmd_parts->cmd_array[j] = NULL;
	cmd_parts->infiles_array[i] = NULL;
	cmd_parts->outfiles_array[k] = NULL;
	return (cmd_parts);
}

int ft_pwd()
{
	char cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
        printf("%s\n", cwd);
		return (EXIT_SUCCESS);
	}
    else
	{
        write(2, "getcwd() error\n", 15);
		return (EXIT_FAILURE);
	}
}

int is_number(char *str)
{
	int i;

	i = 0;
	while (str[i] != '\0')
	{
		if (i == 0 && (str[i] == '-' || str[i] == '+'))
			i++;
		if (ft_isdigit(str[i]) == 0)
			return (0);
		i++;
	}
	return (1);
}

int ft_exit(char *exit_code)
{
	int exit_number;

	if (is_number(exit_code) == 0)
	{
		printf("exit: %s: numeric argument required\n", exit_code);
		return (255);
	}
	exit_number = ft_atoi(exit_code);
	if (exit_number < 0 || exit_number > 255)
		exit_number = 0;
	return (exit_number);
}

int ft_env(char **envp)
{
	int i;

	i = 0;
	while (envp[i] != NULL)
	{
		printf("%s\n",envp[i]);
		i++;
	}
	return (0);
}

int ft_cd(char *new_path)
{
	if (chdir(new_path) != 0)
	{
		fprintf(stderr, "cd: %s: %s\n", strerror(errno), new_path);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int get_num_commands(t_shell *shell)
{
	int	num_pipes;
	int num_commands;

	num_pipes = 0;
	count_pipes(shell->ast, &num_pipes);
	num_commands = 1 + num_pipes;
	//printf("Number of commands: %d\n", num_commands);
	return (num_commands);
}

int is_builtin(t_cmd_parts *cmd_parts)
{
	if (ft_strncmp(cmd_parts->cmd_array[0], "echo\0", 5) == 0)
		return (1);
	else if (ft_strncmp(cmd_parts->cmd_array[0], "exit\0", 5) == 0)
		return (1);
	else if (ft_strncmp(cmd_parts->cmd_array[0], "env\0", 4) == 0)
		return (1);
	else if (ft_strncmp(cmd_parts->cmd_array[0], "pwd\0", 4) == 0)
		return (1);
	else if (ft_strncmp(cmd_parts->cmd_array[0], "cd\0", 3) == 0)
		return (1);
	else if (ft_strncmp(cmd_parts->cmd_array[0], "export\0", 7) == 0)
		return (1);
	else if (ft_strncmp(cmd_parts->cmd_array[0], "unset\0", 6) == 0)
		return (1);
	else
		return (0);
}

int check_and_run_builtins(t_shell *shell, t_cmd_parts *cmd_parts, char **envp)
{
	int k;
	int len;
	int exit_code;

	if (ft_strncmp(cmd_parts->cmd_array[0], "echo\0", 5) == 0)
	{
		k = 1;
		while ((cmd_parts->cmd_array[k]) && (ft_strncmp(cmd_parts->cmd_array[k], "-n\0", 3) == 0))
			k++;
		while(cmd_parts->cmd_array[k] != NULL)
		{
			ft_putstr_fd(cmd_parts->cmd_array[k], STDOUT_FILENO);
			//printf("%s",cmd_parts->cmd_array[k]);
			if (cmd_parts->cmd_array[k + 1] != NULL)
				ft_putstr_fd(" ", STDOUT_FILENO);
				//printf(" ");
			k++;
		}
		if ((cmd_parts->cmd_array[1]) && (ft_strncmp(cmd_parts->cmd_array[1], "-n\0", 3) != 0))
			ft_putstr_fd("\n", STDOUT_FILENO);
			//printf("\n");
		free(cmd_parts->cmd_array);
	}
	else if (ft_strncmp(cmd_parts->cmd_array[0], "exit\0", 5) == 0) // exit code number or if nonnumeric give error, 1'den fazla argument : too many arguments
	{
		len = 0;
		while (cmd_parts->cmd_array[len] != NULL)
			len++;
		printf("exit\n");
		if (len == 1)
		{
			free(cmd_parts->cmd_array);
			cleanup_shell(shell);
			exit(EXIT_SUCCESS);
		}
		else if (len == 2)
		{
			exit_code = ft_exit(cmd_parts->cmd_array[1]);
			free(cmd_parts->cmd_array);
			cleanup_shell(shell);
			exit(exit_code);
		}
		else if (len > 2)
		{
			if (is_number(cmd_parts->cmd_array[1]))
			{
				printf("exit: too many arguments\n");
				free(cmd_parts->cmd_array);
				//cleanup_shell(shell);
				return (EXIT_FAILURE);
			}
			else
			{
				printf("exit: %s: numeric argument required\n", cmd_parts->cmd_array[1]);
				free(cmd_parts->cmd_array);
				cleanup_shell(shell);
				exit(255);
			}
		}
	}
	else if (ft_strncmp(cmd_parts->cmd_array[0], "env\0", 4) == 0)
	{
		len = 0;
		while (cmd_parts->cmd_array[len] != NULL)
			len++;
		if (len > 1)
		{
			write(2, "env currently does not accept any options or arguments\n", 55);
			return (EXIT_FAILURE);
		}
		else
			ft_env(envp);
		free(cmd_parts->cmd_array);
	}
	else if (ft_strncmp(cmd_parts->cmd_array[0], "pwd\0", 4) == 0)
	{
		ft_pwd();
		free(cmd_parts->cmd_array);
	}
	else if (ft_strncmp(cmd_parts->cmd_array[0], "cd\0", 3) == 0)
	{
		ft_cd(cmd_parts->cmd_array[1]);
		free(cmd_parts->cmd_array);
	}
	return (0);
}
