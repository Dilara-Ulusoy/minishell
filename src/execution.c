/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 22:03:03 by htopa             #+#    #+#             */
/*   Updated: 2025/04/16 02:04:09 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"

// void find_ast_pipes(t_ast_node *node)
// {
// 	if (!node)
// 		return;
//     if (node->node_type == AST_PIPE)
//     {
//         printf("✅ Found AST_PIPE node at %p\n", (void *)node);

//         // Sol ve sağ düğümlerin cmd_args değerlerini yazdır
//         if (node->left && node->left->cmd_args)
//             printf("  ├── Left Command: %s\n", node->left->cmd_args);
//         else
//             printf("  ├── Left Command: NULL\n");

//         if (node->right && node->right->cmd_args)
//             printf("  └── Right Command: %s\n", node->right->cmd_args);
//         else
//             printf("  └── Right Command: NULL\n");
//     }
//     find_ast_pipes(node->left);
//     find_ast_pipes(node->right);
// }

void count_pipes(const t_token *head, int *i)
{
	const t_token *curr;

	if (!head)
		return ;
	curr = head;
	while (curr)
	{
		if (curr->type == TOKEN_PIPE)
			(*i)++;
		curr = curr->next;
	}
}

int get_num_commands(const t_token *head)
{
	int	num_pipes;
	int num_commands;

	num_pipes = 0;
	count_pipes(head, &num_pipes);
	num_commands = 1 + num_pipes;
	//printf("Number of commands: %d\n", num_commands);
	return (num_commands);
}

// void count_pipes(t_ast_node *node, int *i)
// {
// 	if (!node)
// 		return ;
//     if (node->node_type == AST_PIPE)
// 		(*i)++;
//     count_pipes(node->left, i);
//     count_pipes(node->right, i);
// }

// int get_num_commands(t_shell *shell)
// {
// 	int	num_pipes;
// 	int num_commands;

// 	num_pipes = 0;
// 	count_pipes(shell->ast, &num_pipes);
// 	num_commands = 1 + num_pipes;
// 	//printf("Number of commands: %d\n", num_commands);
// 	return (num_commands);
// }

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
			if (curr->type == TOKEN_REDIR_IN || curr->type == TOKEN_REDIR_HERE)
				(cmd_parts->n_in)++;
			if (curr->type == TOKEN_REDIR_OUT || curr->type == TOKEN_REDIR_APPEND)
				(cmd_parts->n_out)++;
			curr = curr->next->next;
		}
		if (curr && curr->type == TOKEN_WORD)
		{
			if (curr->value[0] != '\0')
				(cmd_parts->n_cmd)++;
        	curr = curr->next;
		}
    }
	cmd_parts->n_files = cmd_parts->n_in + cmd_parts->n_out;
	return (cmd_parts);
}

// t_cmd_parts *count_tokens(const t_token *head, int command_number)
// {
// 	t_cmd_parts *cmd_parts;
// 	int pipe_number;
// 	const t_token *curr; // = head;

// 	cmd_parts = malloc(sizeof(t_cmd_parts));
// 	pipe_number = 0;
// 	curr = head;
// 	while (pipe_number < (command_number -1))
// 	{
// 		while (curr && curr->type != TOKEN_PIPE)
// 			curr = curr->next;
// 		if (curr->type == TOKEN_PIPE)
// 		{
// 			pipe_number++;
// 			curr = curr->next;
// 		}
// 	}
// 	cmd_parts->n_cmd = 0;
// 	cmd_parts->n_in = 0;
// 	cmd_parts->n_out = 0;
//     while (curr && curr->type != TOKEN_PIPE)
//     {
// 		if (curr->type >= TOKEN_REDIR_IN && curr->type <= TOKEN_REDIR_HERE)
// 		{
// 			if (curr->type == TOKEN_REDIR_IN)
// 				(cmd_parts->n_in)++;
// 			if (curr->type == TOKEN_REDIR_OUT || curr->type == TOKEN_REDIR_APPEND)
// 				(cmd_parts->n_out)++;
// 			curr = curr->next->next;
// 		}
// 		if (curr && curr->type == TOKEN_WORD)
// 		{
// 			if (curr->value[0] != '\0')
// 				(cmd_parts->n_cmd)++;
//         	curr = curr->next;
// 		}
//     }
// 	return (cmd_parts);
// }

t_cmd_parts *get_command_array(const t_token *head, int command_number)
{
	t_cmd_parts *cmd_parts;
	int i;
	int j;
	int pipe_number;
	const t_token *curr;
	
	cmd_parts = count_tokens(head, command_number);
	if (cmd_parts == NULL)
		return (NULL);
	//printf("Count of tokens: %d\n", cmd_parts->n_cmd);
	cmd_parts->cmd_array = malloc(sizeof(char *) * (cmd_parts->n_cmd + 1));
	cmd_parts->files_array = malloc(sizeof(char *) * (cmd_parts->n_files + 1));
	cmd_parts->files_types = malloc(sizeof(int) * (cmd_parts->n_files));
	if (!cmd_parts->cmd_array || !cmd_parts->files_array || !cmd_parts->files_types)
	{
		// Free everything in case of allocation failure
		free(cmd_parts->cmd_array);
		free(cmd_parts->files_array);
		free(cmd_parts->files_types);
		free(cmd_parts);
		return NULL;
	}
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
    while (curr && curr->type != TOKEN_PIPE)
    {
		if (curr->type >= TOKEN_REDIR_IN && curr->type <= TOKEN_REDIR_HERE)
		{
			cmd_parts->files_array[i] = ft_strdup(curr->next->value);
			if (curr->type == TOKEN_REDIR_IN)
				cmd_parts->files_types[i] = 0;
			if (curr->type == TOKEN_REDIR_OUT)
				cmd_parts->files_types[i] = 1;
			if (curr->type == TOKEN_REDIR_APPEND)
				cmd_parts->files_types[i] = 2;
			if (curr->type == TOKEN_REDIR_HERE)
				cmd_parts->files_types[i] = 3;
			i++;
			curr = curr->next->next;
		}
		if (curr && curr->type == TOKEN_WORD)
		{
			//cmd_parts->cmd_array[j] = curr->value;
			if (curr->value[0] != '\0')
			{
				cmd_parts->cmd_array[j] = ft_strdup(curr->value);
				j++;
			}
        	curr = curr->next;
		}
    }
	cmd_parts->cmd_array[j] = NULL;
	cmd_parts->files_array[i] = NULL;
	return (cmd_parts);
}

// t_cmd_parts *get_command_array(const t_token *head, int command_number)
// {
// 	t_cmd_parts *cmd_parts;
// 	int i;
// 	int j;
// 	int k;
// 	int pipe_number;
// 	const t_token *curr;
	
// 	cmd_parts = count_tokens(head, command_number);
// 	if (cmd_parts == NULL)
// 		return (NULL);
// 	//printf("Count of tokens: %d\n", cmd_parts->n_cmd);
// 	cmd_parts->cmd_array = malloc(sizeof(char *) * (cmd_parts->n_cmd + 1));
// 	cmd_parts->infiles_array = malloc(sizeof(char *) * (cmd_parts->n_in + 1));
// 	cmd_parts->outfiles_array = malloc(sizeof(char *) * (cmd_parts->n_out + 1));
// 	cmd_parts->outfiles_types = malloc(sizeof(int) * (cmd_parts->n_out));
// 	if (!cmd_parts->cmd_array || !cmd_parts->infiles_array || !cmd_parts->outfiles_array || !cmd_parts->outfiles_types)
// 	{
// 		// Free everything in case of allocation failure
// 		free(cmd_parts->cmd_array);
// 		free(cmd_parts->infiles_array);
// 		free(cmd_parts->outfiles_array);
// 		free(cmd_parts->outfiles_types);
// 		free(cmd_parts);
// 		return NULL;
// 	}
// 	cmd_parts->command_number = command_number - 1;
// 	pipe_number = 0;
// 	curr = head;
// 	while (pipe_number < (command_number - 1))
// 	{
// 		while (curr && curr->type != TOKEN_PIPE)
// 			curr = curr->next;
// 		if (curr->type == TOKEN_PIPE)
// 		{
// 			pipe_number++;
// 			curr = curr->next;
// 		}
// 	}
// 	i = 0;
// 	j = 0;
// 	k = 0;
//     while (curr && curr->type != TOKEN_PIPE)
//     {
// 		if (curr->type >= TOKEN_REDIR_IN && curr->type <= TOKEN_REDIR_HERE)
// 		{
// 			if (curr->type == TOKEN_REDIR_IN)
// 			{
// 				//cmd_parts->infiles_array[i] = curr->next->value;
// 				cmd_parts->infiles_array[i] = ft_strdup(curr->next->value);
// 				i++;
// 			}
// 			if (curr->type == TOKEN_REDIR_OUT || curr->type == TOKEN_REDIR_APPEND)
// 			{
// 				//cmd_parts->outfiles_array[k] = curr->next->value;
// 				cmd_parts->outfiles_array[k] = ft_strdup(curr->next->value);
// 				if (curr->type == TOKEN_REDIR_OUT)
// 					cmd_parts->outfiles_types[k] = 1;
// 				else
// 					cmd_parts->outfiles_types[k] = 2;
// 				k++;
// 			}
// 			curr = curr->next->next;
// 		}
// 		if (curr && curr->type == TOKEN_WORD)
// 		{
// 			//cmd_parts->cmd_array[j] = curr->value;
// 			if (curr->value[0] != '\0')
// 			{
// 				cmd_parts->cmd_array[j] = ft_strdup(curr->value);
// 				j++;
// 			}
//         	curr = curr->next;
// 		}
//     }
// 	cmd_parts->cmd_array[j] = NULL;
// 	cmd_parts->infiles_array[i] = NULL;
// 	cmd_parts->outfiles_array[k] = NULL;
// 	return (cmd_parts);
// }

int ft_pwd(t_shell *shell)
{
	char cwd[PATH_MAX];
	char *env_var_value;

	env_var_value = ft_getenv("PWD", shell);
	if (env_var_value == NULL)
	{
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
	else
	{
		printf("%s\n", env_var_value);
		free(env_var_value);
		env_var_value=NULL;
		return (EXIT_SUCCESS);
	}
}

int is_number(char *str)
{
	int i;
	char *str_check;
	long long n;

	i = 0;
	while (str[i] != '\0')
	{
		if (i == 0 && (str[i] == '-' || str[i] == '+'))
			i++;
		if (ft_isdigit(str[i]) == 0)
			return (0);
		i++;
	}
	n = ft_atoi_longlong(str);
	str_check = ft_itoa_longlong(n);
	if (str[0] == '+')
		str++;
	if (strncmp(str, str_check, ft_strlen(str)) != 0)
	{
		free(str_check);
		str_check = NULL;
		return (0);
	}
	free(str_check);
	str_check = NULL;
	return (1);
}

int ft_exit(char *exit_code)
{
	int exit_number;

	ft_putstr_fd("exit\n", 1);
	if (is_number(exit_code) == 0)
	{
		ft_putstr_fd("exit: ", 2);
		ft_putstr_fd(exit_code, 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		//printf("exit: %s: numeric argument required\n", exit_code);
		return (2);
	}
	exit_number = ft_atoi(exit_code);
	if (exit_number < 0)
		exit_number = 156;
	if (exit_number > 255)
		exit_number = 42;
	// if (exit_number < 0 || exit_number > 255)
	// 	exit_number = 0;
	return (exit_number);
}

static int contains_equal(char *s)
{
	int i;

	i = 0;
	while(s[i] != '\0')
	{
		if (s[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

int ft_env(char **envp)
{
	int i;

	i = 0;
	while (envp[i] != NULL)
	{
		if (contains_equal(envp[i]))
			printf("%s\n",envp[i]);
		i++;
	}
	return (0);
}

static char *copy_until_equal(char *src)
{
	char *dest;
    int i;
	
	i = 0;
	while (src[i] != '=' && src[i] != '\0')
		i++;
	dest = (char *)malloc(sizeof(char) * (i + 2));
	i = 0;
    while (src[i] != '=' && src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
	dest[i] = '=';
    dest[i + 1] = '\0'; // Null-terminate the destination string
	return (dest);
}

static int check_export_var_name_is_valid(char *name_w_equal)
{
	int i;

	i = 0;
	if (name_w_equal[i] == '=')
	{
		ft_putstr_fd("export: `", 2);
		ft_putchar_fd(name_w_equal[i], 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (0);
	}
	while (name_w_equal[i] != '=')
	{
		if ((i == 0 && ft_isdigit(name_w_equal[i])) || !(ft_isalnum(name_w_equal[i]) || name_w_equal[i] == '_'))
		{
			ft_putstr_fd("export: `", 2);
			ft_putchar_fd(name_w_equal[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			return (0);
		}
		i++;
	}
	return (1);
}

static int ft_strcmp_wo_equal(char *s1, char *s2)
{
	int i;

	if (ft_strlen(s1) != (ft_strlen(s2) - 1))
		return (0);
	i = 0;
    while (s1[i] != '\0')
	{
        if (s1[i] != s2[i])
			return (0);
		i++;
    }
    return (1);
}

int	ft_set(char *var_eq_value, char ***envp)
{
	int		i;
	char *name_w_equal;
	int ret;

	if (contains_equal(var_eq_value))
		name_w_equal = copy_until_equal(var_eq_value);
	else
		name_w_equal = ft_strjoin(var_eq_value, "=");
	ret = check_export_var_name_is_valid(name_w_equal);
	if (!ret)
	{
		free(name_w_equal);
		return (EXIT_FAILURE);
	}
	i = 0;
	while ((*envp)[i] != NULL && (ft_strnstr((*envp)[i], name_w_equal, ft_strlen(name_w_equal)) == 0  && ft_strcmp_wo_equal((*envp)[i], name_w_equal) == 0))
		i++;
	//printf("Searched for: %s\n\n", name_w_equal);
	if ((*envp)[i] == NULL)
	{
		//printf("Var was not found in envp\n");
		ft_add_env_var(var_eq_value, envp);
		free(name_w_equal);
		return (EXIT_SUCCESS);
	}
	//printf("Var was found in envp: %s\n", (*envp)[i]);
	if ((ft_strnstr((*envp)[i], name_w_equal, ft_strlen(name_w_equal)) != 0) && (contains_equal(var_eq_value) == 0))
	{
		free(name_w_equal);
		return (EXIT_SUCCESS);
	}
	free(name_w_equal);
	free((*envp)[i]);
	//(*envp)[i] = NULL;
	//(*envp)[i] = malloc(sizeof(char *));
	(*envp)[i] = ft_strdup(var_eq_value);
	return (EXIT_SUCCESS);
}

static void    ft_unset(char *value, char ***envp)
{
	char *name_w_equal;
	int i;
	int len;
	int j;
	char **reduced_envp;
	
	len = 0;
	while ((*envp)[len] != NULL)
		len++;
	name_w_equal = ft_strjoin(value, "=");
	i = 0;
	while ((*envp)[i] != NULL && (ft_strnstr((*envp)[i], name_w_equal, ft_strlen(name_w_equal)) == 0  && ft_strcmp_wo_equal((*envp)[i], name_w_equal) == 0))
		i++;
	free(name_w_equal);
	if ((*envp)[i] == NULL)
		return ;
	reduced_envp = malloc(sizeof(char *) * (len));
	if (!reduced_envp)
		return ;
	len = 0;
	j = 0;
	while ((*envp)[j] != NULL)
	{
		if (j == i)
		{
			free((*envp)[j]);
			j++;
			continue ;
		}
		reduced_envp[len] = (*envp)[j];
		len++;
		j++;
	}
	reduced_envp[len] = NULL;
	free(*envp);
	*envp = reduced_envp;
}

int ft_cd(char *new_path, char ***envp)
{
	char *old_path;
	char abs_new_path[PATH_MAX];
	int		i;

	if (new_path == NULL)
	{
		ft_putstr_fd("Please specify path as an argument to cd.\n",2);
		return (EXIT_FAILURE);
	}
	if (chdir(new_path) != 0)
	{
		perror(new_path);
		return (EXIT_FAILURE);
	}
	i = 0;
	while ((*envp)[i] != NULL && ft_strnstr((*envp)[i], "PWD=", 4) == 0)
		i++;
	old_path = ft_strjoin("OLD",(*envp)[i]);
	i = 0;
	while ((*envp)[i] != NULL && ft_strnstr((*envp)[i], "OLDPWD=", 7) == 0)
		i++;
	free((*envp)[i]);
	(*envp)[i] = old_path;
	if (getcwd(abs_new_path, sizeof(abs_new_path)) != NULL)
	{
		i = 0;
		while ((*envp)[i] != NULL && ft_strnstr((*envp)[i], "PWD=", 4) == 0)
			i++;
		free((*envp)[i]);
		(*envp)[i] = ft_strjoin("PWD=", abs_new_path);
	}
	else
	{
		ft_putstr_fd("getcwd() error.\n",2);
		return (EXIT_FAILURE);
	}
	//ft_env(*envp);
	return (EXIT_SUCCESS);
}

static int check_echo_option(char *str)
{
    int i;

    if (str[0] == '-' && str[1] == 'n')
    {
        i = 2;
        while (str[i] != '\0')
        {
            if (str[i] == 'n')
                i++;
            else
                return (0);
        }
        return (1);
    }
    return (0);
}

int is_builtin(t_cmd_parts *cmd_parts)
{
	if (cmd_parts->cmd_array[0] == NULL)
		return (0);
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

int check_and_run_builtins(t_shell *shell, t_cmd_parts **cmd_parts, t_args *arg_struct)
{
	int k;
	int len;
	int exit_code;
	//char **envp;

	//envp = arg_struct->envp;
	if (ft_strncmp((*cmd_parts)->cmd_array[0], "unset\0", 6) == 0)
	{
		k = 1;
		while ((*cmd_parts)->cmd_array[k] != NULL)
		{
			ft_unset((*cmd_parts)->cmd_array[k], &arg_struct->envp);
			k++;
		}
		return (EXIT_SUCCESS);
	}
	if (ft_strncmp((*cmd_parts)->cmd_array[0], "export\0", 7) == 0)
	{
		//free_cmd_parts(cmd_parts);
		//cmd_parts = NULL;

		//ft_addA(&envp);
		//ft_addA(&(arg_struct->envp));
		//ft_set("A=55\0", &envp);
		if ((*cmd_parts)->cmd_array[1] == NULL)
		{
			k = 0;
			while(arg_struct->envp[k] != NULL)
			{
				ft_putstr_fd("declare -x ", 1);
				ft_putstr_fd(arg_struct->envp[k], 1);
				ft_putstr_fd("\n", 1);
				k++;
			}
		}
		else
		{
			k = 1;
			while ((*cmd_parts)->cmd_array[k] != NULL)
			{
				exit_code = ft_set((*cmd_parts)->cmd_array[k], &(arg_struct->envp));
				if (exit_code)
					return (exit_code);
				k++;
			}
		}
		return (EXIT_SUCCESS);
		//ft_env(envp);
		//free_cmd_parts(cmd_parts);
		//cmd_parts = NULL;
	}
	if (ft_strncmp((*cmd_parts)->cmd_array[0], "echo\0", 5) == 0)
	{
		k = 1;
		while (((*cmd_parts)->cmd_array[k]) && (check_echo_option((*cmd_parts)->cmd_array[k]) == 1))
			k++;
		while((*cmd_parts)->cmd_array[k] != NULL)
		{
			ft_putstr_fd((*cmd_parts)->cmd_array[k], STDOUT_FILENO);
			//printf("%s",cmd_parts->cmd_array[k]);
			if ((*cmd_parts)->cmd_array[k + 1] != NULL)
				ft_putstr_fd(" ", STDOUT_FILENO);
				//printf(" ");
			k++;
		}
		if (((*cmd_parts)->cmd_array[1] == NULL) || (((*cmd_parts)->cmd_array[1]) && (check_echo_option((*cmd_parts)->cmd_array[1]) == 0)))
			ft_putstr_fd("\n", STDOUT_FILENO);
			//printf("\n");
		free_cmd_parts(cmd_parts);
		//cmd_parts = NULL;
	}
	else if (ft_strncmp((*cmd_parts)->cmd_array[0], "exit\0", 5) == 0) // exit code number or if nonnumeric give error, 1'den fazla argument : too many arguments
	{
		len = 0;
		while ((*cmd_parts)->cmd_array[len] != NULL)
			len++;
		//ft_putstr_fd("exit\n", 2);
		if (len == 1)
		{
			ft_putstr_fd("exit\n", 1);
			free_cmd_parts(cmd_parts);
			free(arg_struct->pids);
			free_envp(arg_struct->envp);
			free(arg_struct);
			//cmd_parts = NULL;
			cleanup_shell(shell);
			exit(EXIT_SUCCESS);
		}
		else if (len == 2)
		{
			exit_code = ft_exit((*cmd_parts)->cmd_array[1]);
			free_cmd_parts(cmd_parts);
			free(arg_struct->pids);
			free_envp(arg_struct->envp);
			free(arg_struct);
			//cmd_parts = NULL;
			cleanup_shell(shell);
			exit(exit_code);
		}
		else if (len > 2)
		{
			if (is_number((*cmd_parts)->cmd_array[1]))
			{
				ft_putstr_fd("exit\n", 2);
				ft_putstr_fd("exit: too many arguments\n", 2);
				free_cmd_parts(cmd_parts);
				//cmd_parts = NULL;
				//cleanup_shell(shell);
				return (EXIT_FAILURE);
			}
			else
			{
				ft_putstr_fd("exit\n", 2);
				ft_putstr_fd("exit: ", 2);
				ft_putstr_fd((*cmd_parts)->cmd_array[1], 2);
				ft_putstr_fd(": numeric argument required\n", 2);
				//printf("exit: %s: numeric argument required\n", (*cmd_parts)->cmd_array[1]);
				free_cmd_parts(cmd_parts);
				//cmd_parts = NULL;
				free(arg_struct->pids);
				free_envp(arg_struct->envp);
				free(arg_struct);
				cleanup_shell(shell);
				exit(2);
			}
		}
	}
	else if (ft_strncmp((*cmd_parts)->cmd_array[0], "env\0", 4) == 0)
	{
		len = 0;
		while ((*cmd_parts)->cmd_array[len] != NULL)
			len++;
		if (len > 1)
		{
			write(2, "env currently does not accept any options or arguments\n", 55);
			return (EXIT_FAILURE);
		}
		else
			ft_env(arg_struct->envp);
		free_cmd_parts(cmd_parts);
		//cmd_parts = NULL;
	}
	else if (ft_strncmp((*cmd_parts)->cmd_array[0], "pwd\0", 4) == 0)
	{
		ft_pwd(shell);
		free_cmd_parts(cmd_parts);
		//cmd_parts = NULL;
	}
	else if (ft_strncmp((*cmd_parts)->cmd_array[0], "cd\0", 3) == 0)
	{
		//printf("Number of command args: %d\n", (*cmd_parts)->n_cmd);
		if ((*cmd_parts)->n_cmd > 2)
		{
			write(2, "bash: cd: too many arguments\n", 29);
			exit_code = EXIT_FAILURE;
		}
		else
			exit_code = ft_cd((*cmd_parts)->cmd_array[1], &arg_struct->envp);
		free_cmd_parts(cmd_parts);
		return (exit_code);
		//cmd_parts = NULL;
	}
	return (0);
}

int check_and_run_builtins_single(t_shell *shell, t_cmd_parts **cmd_parts, char ***envp, int *original_fd)
{
	int k;
	int len;
	int exit_code;

	//ft_env(arg_struct->envp);
	//ft_env(envp);
	if (ft_strncmp((*cmd_parts)->cmd_array[0], "unset\0", 6) == 0)
	{
		k = 1;
		while ((*cmd_parts)->cmd_array[k] != NULL)
		{
			ft_unset((*cmd_parts)->cmd_array[k], envp);
			k++;
		}
		return (EXIT_SUCCESS);
	}
	if (ft_strncmp((*cmd_parts)->cmd_array[0], "export\0", 7) == 0)
	{
		//free_cmd_parts(cmd_parts);
		//cmd_parts = NULL;
		//ft_addA(&envp);
		//ft_addA(&(arg_struct->envp));
		//ft_set("A=55", envp);
		if ((*cmd_parts)->cmd_array[1] == NULL)
		{
			k = 0;
			while((*envp)[k] != NULL)
			{
				ft_putstr_fd("declare -x ", 1);
				ft_putstr_fd((*envp)[k], 1);
				ft_putstr_fd("\n", 1);
				k++;
			}
		}
		else
		{
			k = 1;
			while ((*cmd_parts)->cmd_array[k] != NULL)
			{
				exit_code = ft_set((*cmd_parts)->cmd_array[k], envp);
				if (exit_code)
					return (exit_code);
				k++;
			}
			return (EXIT_SUCCESS);
		}
		//ft_env(*envp);
		//free_cmd_parts(cmd_parts);
		//cmd_parts = NULL;
	}
	if (ft_strncmp((*cmd_parts)->cmd_array[0], "echo\0", 5) == 0)
	{
		k = 1;
		while (((*cmd_parts)->cmd_array[k]) && (check_echo_option((*cmd_parts)->cmd_array[k]) == 1))
			k++;
		while((*cmd_parts)->cmd_array[k] != NULL)
		{
			ft_putstr_fd((*cmd_parts)->cmd_array[k], STDOUT_FILENO);
			//printf("%s",cmd_parts->cmd_array[k]);
			if ((*cmd_parts)->cmd_array[k + 1] != NULL)
				ft_putstr_fd(" ", STDOUT_FILENO);
				//printf(" ");
			k++;
		}
		if (((*cmd_parts)->cmd_array[1] == NULL) || (((*cmd_parts)->cmd_array[1]) && (check_echo_option((*cmd_parts)->cmd_array[1]) == 0)))
			ft_putstr_fd("\n", STDOUT_FILENO);
			//printf("\n");
		free_cmd_parts(cmd_parts);
		//cmd_parts = NULL;
	}
	else if (ft_strncmp((*cmd_parts)->cmd_array[0], "exit\0", 5) == 0) // exit code number or if nonnumeric give error, 1'den fazla argument : too many arguments
	{
		len = 0;
		while ((*cmd_parts)->cmd_array[len] != NULL)
			len++;
		//ft_putstr_fd("exit\n", 2);
		if (len == 1)
		{
			ft_putstr_fd("exit\n", 1);
			free_cmd_parts(cmd_parts);
			free_envp(*envp);
			dup2(original_fd[0], STDIN_FILENO);
			dup2(original_fd[1], STDOUT_FILENO);
			close(original_fd[0]);
			close(original_fd[1]);
			//cmd_parts = NULL;
			cleanup_shell(shell);
			exit(EXIT_SUCCESS);
		}
		else if (len == 2)
		{
			exit_code = ft_exit((*cmd_parts)->cmd_array[1]);
			free_cmd_parts(cmd_parts);
			free_envp(*envp);
			dup2(original_fd[0], STDIN_FILENO);
			dup2(original_fd[1], STDOUT_FILENO);
			close(original_fd[0]);
			close(original_fd[1]);
			//cmd_parts = NULL;
			cleanup_shell(shell);
			exit(exit_code);
		}
		else if (len > 2)
		{
			if (is_number((*cmd_parts)->cmd_array[1]))
			{
				ft_putstr_fd("exit\n", 2);
				ft_putstr_fd("exit: too many arguments\n", 2);
				free_cmd_parts(cmd_parts);
				//cmd_parts = NULL;
				//cleanup_shell(shell);
				return (EXIT_FAILURE);
			}
			else
			{
				ft_putstr_fd("exit\n", 2);
				ft_putstr_fd("exit: ", 2);
				ft_putstr_fd((*cmd_parts)->cmd_array[1], 2);
				ft_putstr_fd(": numeric argument required\n", 2);
				//printf("exit: %s: numeric argument required\n", (*cmd_parts)->cmd_array[1]);
				free_cmd_parts(cmd_parts);
				free_envp(*envp);
				dup2(original_fd[0], STDIN_FILENO);
				dup2(original_fd[1], STDOUT_FILENO);
				close(original_fd[0]);
				close(original_fd[1]);
				//cmd_parts = NULL;
				cleanup_shell(shell);
				exit(2);
			}
		}
	}
	else if (ft_strncmp((*cmd_parts)->cmd_array[0], "env\0", 4) == 0)
	{
		len = 0;
		while ((*cmd_parts)->cmd_array[len] != NULL)
			len++;
		if (len > 1)
		{
			write(2, "env currently does not accept any options or arguments\n", 55);
			return (EXIT_FAILURE);
		}
		else
			ft_env(*envp);
		free_cmd_parts(cmd_parts);
		//cmd_parts = NULL;
	}
	else if (ft_strncmp((*cmd_parts)->cmd_array[0], "pwd\0", 4) == 0)
	{
		ft_pwd(shell);
		free_cmd_parts(cmd_parts);
		//cmd_parts = NULL;
	}
	else if (ft_strncmp((*cmd_parts)->cmd_array[0], "cd\0", 3) == 0)
	{
		//printf("Number of command args: %d\n", (*cmd_parts)->n_cmd);
		if ((*cmd_parts)->n_cmd > 2)
		{
			write(2, "bash: cd: too many arguments\n", 29);
			exit_code = EXIT_FAILURE;
		}
		else
			exit_code = ft_cd((*cmd_parts)->cmd_array[1], envp);
		free_cmd_parts(cmd_parts);
		return (exit_code);
		//cmd_parts = NULL;
	}
	return (0);
}
