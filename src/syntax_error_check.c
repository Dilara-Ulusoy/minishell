/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error_check.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoc@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 14:05:19 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/01/30 14:18:03 by dakcakoc         ###   ########.fr       */
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
static int handle_initial_token_check(t_parser *parser, t_token *current)
{
	if (is_redirection(current->type) && current->next && current->next->type == TOKEN_WORD)
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
		//set_error_number(parser, 1);
		return (0);
	}
	return (1);
}

/**
 * handle_consecutive_operators - Checks if two consecutive operators exist.
 *
 * If two operators are found back-to-back, a syntax error is set.
 *
 * @return 1 if parsing should continue, 0 if an error is detected.
 */
static int handle_consecutive_operators(t_parser *parser, t_token *current)
{
	if (is_operator(current->type) && is_operator(current->next->type))
	{
		set_syntax_error(parser, current->next->value);
		//set_error_number(parser, 1);
		return (0);
	}
	return 1;
}

/**
 * handle_redirection_followed_by_word - Checks redirection syntax.
 *
 * Ensures that every redirection token is followed by a valid word token.
 * EG: > file.txt, < file.txt, >> file.txt, << EOF, echo "Hello" >
 *
 * @return 1 if parsing should continue, 0 if an error is detected.
 */
static int handle_redirection_followed_by_word(t_parser *parser, t_token *current)
{
	if (is_redirection(current->type))
	{
		if (current->next == NULL || current->next->type != TOKEN_WORD)
		{
			if (current->next != NULL)
				set_syntax_error(parser, current->next->value);
			else
				set_syntax_error(parser, "\\n");
			return 0;
		}
	}
	return 1;
}

/**
 * handle_trailing_operator - Checks if the last token is an operator.
 *
 * If the last token is an operator, it sets a syntax error.
 *
 * @param parser Pointer to the parser structure.
 * @param current Pointer to the current token.
 */
static int handle_trailing_operator(t_parser *parser, t_token *current)
{
	if (current && is_operator(current->type))
	{
		if (!is_redirection((current)->type))
			set_syntax_error(parser, (current)->value);
		else
			set_syntax_error(parser, "\\n");
		//set_error_number(parser, 1);
		return 0;
	}
	return 1;
}

int check_syntax_errors(t_parser *parser)
{
	t_token *current;

	current = parser->tokens;
	if (!parser || !parser->tokens)
		return (0);
	if (!handle_initial_token_check(parser, current))
		return (0);
	while (current && current->next)
	{
		if (!handle_consecutive_operators(parser, current))
			return (0);
		if (!handle_redirection_followed_by_word(parser, current))
			return (0);
		current = current->next;
	}
	if (!handle_trailing_operator(parser, current))
		return (0);
	return (1);
}
