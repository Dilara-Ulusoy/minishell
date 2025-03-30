/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error_messages.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 12:58:25 by htopa             #+#    #+#             */
/*   Updated: 2025/03/28 17:19:27 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "execution.h"

int	display_error_message(int i, t_args *arg_struct)
{
	ft_putstr_fd("*************************\n", 2);
	if (i == 1)
	{
		ft_putstr_fd("pipe() failed!\n", 2);
		free(arg_struct->pids);
		free(arg_struct);
	}
	else if (i == 2)
	{
		ft_putstr_fd("fork() failed!\n", 2);
		return (close_and_free(arg_struct, 1));
	}
	else if (i == 3)
	{
		ft_putstr_fd("dup2() failed!\n", 2);
		return (close_and_free(arg_struct, 1));
	}
	else if (i == 4)
		ft_putstr_fd("mallloc() failed!\n", 2);
	ft_putstr_fd("*************************\n", 2);
	return (EXIT_FAILURE);
}

int	check_dup2(int file_d, t_args *arg_struct, int std_type)
{
	if (file_d == -1)
		return (close_and_free(arg_struct, 1));
	else if (dup2(file_d, std_type) == -1)
		return (display_error_message(3, arg_struct));
	else
		return (0);
}

void free_cmd_parts(t_cmd_parts **cmd_parts)
{
    int i;

    if (cmd_parts == NULL || *cmd_parts == NULL)
        return;

    if ((*cmd_parts)->cmd_array != NULL)
    {
        i = 0;
        while ((*cmd_parts)->cmd_array[i] != NULL)
        {
            free((*cmd_parts)->cmd_array[i]);
            i++;
        }
        free((*cmd_parts)->cmd_array);
        (*cmd_parts)->cmd_array = NULL;
    }
    if ((*cmd_parts)->files_array != NULL)
    {
        i = 0;
        while ((*cmd_parts)->files_array[i] != NULL)
        {
            free((*cmd_parts)->files_array[i]);
            i++;
        }
        free((*cmd_parts)->files_array);
        (*cmd_parts)->files_array = NULL;
    }
    if ((*cmd_parts)->files_types != NULL)
    {
        free((*cmd_parts)->files_types);
        (*cmd_parts)->files_types = NULL;
    }
    free(*cmd_parts);
    *cmd_parts = NULL;
}

// void free_cmd_parts(t_cmd_parts **cmd_parts)
// {
//     int i;

//     if (cmd_parts == NULL || *cmd_parts == NULL)
//         return;

//     if ((*cmd_parts)->cmd_array != NULL)
//     {
//         i = 0;
//         while ((*cmd_parts)->cmd_array[i] != NULL)
//         {
//             free((*cmd_parts)->cmd_array[i]);
//             i++;
//         }
//         free((*cmd_parts)->cmd_array);
//         (*cmd_parts)->cmd_array = NULL;
//     }

//     if ((*cmd_parts)->infiles_array != NULL)
//     {
//         i = 0;
//         while ((*cmd_parts)->infiles_array[i] != NULL)
//         {
//             free((*cmd_parts)->infiles_array[i]);
//             i++;
//         }
//         free((*cmd_parts)->infiles_array);
//         (*cmd_parts)->infiles_array = NULL;
//     }

//     if ((*cmd_parts)->outfiles_array != NULL)
//     {
//         i = 0;
//         while ((*cmd_parts)->outfiles_array[i] != NULL)
//         {
//             free((*cmd_parts)->outfiles_array[i]);
//             i++;
//         }
//         free((*cmd_parts)->outfiles_array);
//         (*cmd_parts)->outfiles_array = NULL;
//     }

//     if ((*cmd_parts)->outfiles_types != NULL)
//     {
//         free((*cmd_parts)->outfiles_types);
//         (*cmd_parts)->outfiles_types = NULL;
//     }

//     free(*cmd_parts);
//     *cmd_parts = NULL;
// }

char **copy_envp(char **envp) {
    size_t count = 0;
    while (envp[count] != NULL) {
        count++;
    }

    char **new_envp = malloc((count + 1) * sizeof(char *));
    if (!new_envp) {
        perror("malloc failed");
        return NULL;
    }

    for (size_t i = 0; i < count; i++) {
        new_envp[i] = ft_strdup(envp[i]);
        if (!new_envp[i]) {
            perror("strdup failed");
            while (i > 0) free(new_envp[--i]);
            free(new_envp);
            return NULL;
        }
    }

    new_envp[count] = NULL;  // Null-terminate the array
    return new_envp;
}

void free_envp(char **envp) {
	int i;

    if (!envp)
		return;  // Avoid freeing NULL
    i = 0;
	while (envp[i] != NULL)
	{
        free(envp[i]);  // Free each string
		i++;
    }
	free(envp[i]); 
    free(envp);  // Free the array itself
}

// void	free_array(void **arr, int is_int, int j)
// {
// 	if (j == -1)
// 	{
// 		while (arr[++j] != NULL)
// 		{
// 			if (is_int == 1)
// 				free((int *)arr[j]);
// 			else
// 				free((char *)arr[j]);
// 		}
// 	}
// 	else
// 	{
// 		while (--j >= 0)
// 		{
// 			if (is_int == 1)
// 				free((int *)arr[j]);
// 			else
// 				free((char *)arr[j]);
// 		}
// 	}
// 	free(arr);
// }

// int	close_and_free(t_args *arg_struct, int free_pid)
// {
// 	close_pipes(arg_struct->fd, arg_struct->argc + 1);
// 	free_array((void **)arg_struct->fd, 1, arg_struct->argc + 1);
// 	if (free_pid == 1)
// 	{
// 		free(arg_struct->pids);
// 		free(arg_struct);
// 		return (-1);
// 	}
// 	else
// 		return (0);
// }

// void free_cmd_parts(t_cmd_parts *cmd_parts)
// {
// 	if (cmd_parts == NULL)
// 		return ;
// 	if (cmd_parts->cmd_array != NULL)
// 		free(cmd_parts->cmd_array);
// 	if (cmd_parts->infiles_array != NULL)
// 		free(cmd_parts->infiles_array);
// 	if (cmd_parts->outfiles_array != NULL)
// 		free(cmd_parts->outfiles_array);
// 	if (cmd_parts->outfiles_types != NULL)
// 		free(cmd_parts->outfiles_types);
// 	free(cmd_parts);
// }

// void free_cmd_parts(t_cmd_parts *cmd_parts)
// {
//     int i;

//     if (cmd_parts == NULL)
//         return;

//     if (cmd_parts->cmd_array != NULL)
//     {
//         i = 0;
//         while (cmd_parts->cmd_array[i] != NULL)
//         {
//             free(cmd_parts->cmd_array[i]);
//             i++;
//         }
//         free(cmd_parts->cmd_array);
//         cmd_parts->cmd_array = NULL;
//     }

//     if (cmd_parts->infiles_array != NULL)
//     {
//         i = 0;
//         while (cmd_parts->infiles_array[i] != NULL)
//         {
//             free(cmd_parts->infiles_array[i]);
//             i++;
//         }
//         free(cmd_parts->infiles_array);
//         cmd_parts->infiles_array = NULL;
//     }

//     if (cmd_parts->outfiles_array != NULL)
//     {
//         i = 0;
//         while (cmd_parts->outfiles_array[i] != NULL)
//         {
//             free(cmd_parts->outfiles_array[i]);
//             i++;
//         }
//         free(cmd_parts->outfiles_array);
//         cmd_parts->outfiles_array = NULL;
//     }

//     if (cmd_parts->outfiles_types != NULL)
//     {
//         free(cmd_parts->outfiles_types);
//         cmd_parts->outfiles_types = NULL;
//     }

//     free(cmd_parts);
// }