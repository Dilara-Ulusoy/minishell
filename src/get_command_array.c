/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_command_array.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 13:26:34 by htopa             #+#    #+#             */
/*   Updated: 2025/04/22 20:47:48 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	get_n_files_in_cmd_parts(t_cmd_parts *cmd_parts,
	const t_token *curr)
{
	while (curr && curr->type != TOKEN_PIPE)
	{
		if (curr->type >= TOKEN_REDIR_IN && curr->type <= TOKEN_REDIR_HERE)
		{
			if (curr->type == TOKEN_REDIR_IN || curr->type == TOKEN_REDIR_HERE)
				(cmd_parts->n_in)++;
			if (curr->type == TOKEN_REDIR_OUT
				|| curr->type == TOKEN_REDIR_APPEND)
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
}

static const t_token	*move_to_command_node(const t_token *head,
	int command_number)
{
	int				pipe_number;
	const t_token	*curr;

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
	return (curr);
}

// Check if frees needed before exit(EXIT_FAILURE);
static t_cmd_parts	*get_cmd_parts(const t_token *head, int command_number)
{
	t_cmd_parts		*cmd_parts;
	const t_token	*curr;

	cmd_parts = initialize_cmd_parts();
	if (cmd_parts == NULL)
	{
		ft_putstr_fd("Malloc failed!\n", 2);
		exit(EXIT_FAILURE);
	}
	curr = move_to_command_node(head, command_number);
	get_n_files_in_cmd_parts(cmd_parts, curr);
	cmd_parts->cmd_array = malloc(sizeof(char *) * (cmd_parts->n_cmd + 1));
	cmd_parts->files_array = malloc(sizeof(char *) * (cmd_parts->n_files + 1));
	cmd_parts->files_types = malloc(sizeof(int) * (cmd_parts->n_files));
	if (!cmd_parts->cmd_array || !cmd_parts->files_array
		|| !cmd_parts->files_types)
	{
		ft_putstr_fd("Malloc failed!\n", 2);
		free(cmd_parts->cmd_array);
		free(cmd_parts->files_array);
		free(cmd_parts->files_types);
		free(cmd_parts);
		exit(EXIT_FAILURE);
	}
	return (cmd_parts);
}

// Null check after ft_strdup
static void	fill_arrays_in_cmd_parts(t_cmd_parts *cmd_parts,
	const t_token *curr)
{
	int				i;
	int				j;

	i = -1;
	j = -1;
	while (curr && curr->type != TOKEN_PIPE)
	{
		if (curr->type >= TOKEN_REDIR_IN && curr->type <= TOKEN_REDIR_HERE)
		{
			cmd_parts->files_array[++i] = ft_strdup(curr->next->value);
			cmd_parts->files_types[i] = get_file_type(curr);
			curr = curr->next->next;
		}
		if (curr && curr->type == TOKEN_WORD)
		{
			if (curr->value[0] != '\0')
				cmd_parts->cmd_array[++j] = ft_strdup(curr->value);
			curr = curr->next;
		}
	}
	cmd_parts->cmd_array[++j] = NULL;
	cmd_parts->files_array[++i] = NULL;
}

t_cmd_parts	*get_command_array(const t_token *head, int command_number)
{
	t_cmd_parts		*cmd_parts;
	const t_token	*curr;

	cmd_parts = get_cmd_parts(head, command_number);
	cmd_parts->command_number = command_number - 1;
	curr = move_to_command_node(head, command_number);
	fill_arrays_in_cmd_parts(cmd_parts, curr);
	return (cmd_parts);
}
