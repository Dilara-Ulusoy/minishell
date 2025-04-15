/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoc@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:12:38 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/04/15 16:05:47 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_ast(t_ast_node *root)
{
	if (!root)
		return ;
	if (root->left)
	{
		free_ast(root->left);
		root->left = NULL;
	}
	if (root->right)
	{
		free_ast(root->right);
		root->right = NULL;
	}
	if (root->cmd_args)
	{
		free(root->cmd_args);
		root->cmd_args = NULL;
	}
	if (root->io_redirects)
	{
		free_io_list(root->io_redirects);
		root->io_redirects = NULL;
	}
	free(root);
}

void	free_io_list(t_io_node *io_list)
{
	t_io_node	*temp;

	while (io_list)
	{
		temp = io_list->next;
		if (io_list->filename)
			free(io_list->filename);
		free(io_list);
		io_list = temp;
	}
}

void	cleanup_shell(t_shell *shell)
{
	if (shell->line)
	{
		free(shell->line);
		shell->line = NULL;
	}
	if (shell->tokens)
	{
		free_tokens(&shell->tokens);
		shell->tokens = NULL;
	}
	if (shell->ast)
	{
		free_ast(shell->ast);
		shell->ast = NULL;
	}
	if (shell->heredoc_tmp)
	{
		unlink(shell->heredoc_tmp);
		free(shell->heredoc_tmp);
		shell->heredoc_tmp = NULL;
	}
}

void	cleanup_resources(char *cmd_args, t_io_node *io_list)
{
	if (cmd_args)
		free(cmd_args);
	if (io_list)
		free_io_list(io_list);
}
