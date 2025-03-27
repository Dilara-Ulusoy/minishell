/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error_check_1.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoc@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 16:46:54 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/03/27 16:46:57 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * handle_initial_token_check - Checks the first token for syntax errors.
 *
 * If the first token is an operator, it sets an error. If it's a redirection
 * followed by a word, it moves the pointer forward.
 *
 * @return 1 if parsing should continue, 0 if an error is detected.
 */
static int	handle_initial_token_check(t_parser *parser, t_token *current)
{
	if (is_redirection(current->type) && current->next
		&& current->next->type == TOKEN_WORD)
	{
		current = current->next;
		return (1);
	}
	else if (is_operator(current->type))
	{
		if (!is_redirection(current->type))
			set_syntax_error(parser, current->value);
		else
			set_syntax_error(parser, "\\n");
		return (0);
	}
	return (1);
}

/*
📌 handle_consecutive_operators(parser, current)

Purpose: Checks if two consecutive operators exist in the token list,
setting a syntax error if found.

Example:
- If current = "&&" and current->next = "||", it sets a syntax error, returns 0.
- If current = "|" and current->next = "echo", it returns 1 (valid case).

Effect:
- If two consecutive operators are found, a syntax error is set,
and parsing stops (returns 0).
- Otherwise, parsing continues (returns 1).
*/
static int	handle_consecutive_operators(t_parser *parser, t_token *current)
{
	if (current->type == TOKEN_PIPE && !is_redir(current->next->type))
	{
		set_syntax_error(parser, current->next->value);
		return (0);
	}
	return (1);
}

static int	handle_and_or(t_parser *parser, t_token *current)
{
	if (current->type == TOKEN_WORD)
	{
		if (current->next && current->next->type == TOKEN_AND)
		{
			set_syntax_error(parser, current->value);
			return (0);
		}
		if (current->next && current->next->type == TOKEN_OR)
		{
			set_syntax_error(parser, current->value);
			return (0);
		}
	}
	return (1);
}

static int	handle_parantesis(t_parser *parser, t_token *current)
{
	if (is_operator(current->type) || current->type == TOKEN_WORD)
	{
		if (current->next && current->next->type == TOKEN_PAREN_OPEN)
		{
			set_syntax_error(parser, current->value);
			return (0);
		}
	}
	return (1);
}

int	check_syntax_errors(t_parser *parser)
{
	t_token	*current;

	current = parser->tokens;
	if (!parser || !parser->tokens)
		return (0);
	if (!handle_initial_token_check(parser, current))
		return (0);
	while (current && current->next)
	{
		if (!handle_consecutive_operators(parser, current))
			return (0);
		if (!handle_redir_followed_by_word(parser, current))
			return (0);
		if (!handle_parantesis(parser, current))
			return (0);
		if (!handle_and_or(parser, current))
			return (0);
		current = current->next;
	}
	if (!handle_trailing_operator(parser, current))
		return (0);
	return (1);
}
