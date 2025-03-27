/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error_check_2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoc@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 16:46:46 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/03/27 16:47:30 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
📌 handle_redir_followed_by_word(parser, current)

Purpose: Ensures that every redirection token (>, <, >>, <<)
is followed by a valid word token.

Example:
- If current = ">" and current->next = "file.txt", it returns 1 (valid case).
- If current = ">>" and current->next = NULL or another operator
(e.g., ">> &&"), it sets a syntax error and returns 0.

Effect:
- If a redirection token is not followed by a word, a syntax error is set.
- Returns 1 if the syntax is correct, otherwise returns 0.
*/
int	handle_redir_followed_by_word(t_parser *parser, t_token *current)
{
	if (is_redirection(current->type))
	{
		if (current->next == NULL || current->next->type != TOKEN_WORD)
		{
			if (current->next != NULL)
				set_syntax_error(parser, current->next->value);
			else
				set_syntax_error(parser, "\\n");
			return (0);
		}
	}
	return (1);
}

/**
 * handle_trailing_operator - Checks if the last token is an operator.
 *
 * If the last token is an operator, it sets a syntax error.
 *
 * @param parser Pointer to the parser structure.
 * @param current Pointer to the current token.
 */
int	handle_trailing_operator(t_parser *parser, t_token *current)
{
	if (current && is_operator(current->type))
	{
		if (!is_redirection((current)->type))
			set_syntax_error(parser, (current)->value);
		else
			set_syntax_error(parser, "\\n");
		return (0);
	}
	return (1);
}
