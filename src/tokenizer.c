/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 22:34:26 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/03/25 13:17:56 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "token.h"

static int	process_token_parsing(t_token **head,
		const char *line, int *i, int length, t_shell *shell)
{
	int	parse_result;

	parse_result = parse_two_char_operator(head, line, i, length);
	if (parse_result == -1)
		return (-1);
	if (parse_result)
		return (0);
	parse_result = parse_single_char_operator(head, line, i, length);
	if (parse_result == -1)
		return (-1);
	if (parse_result)
		return (0);
	parse_result = handle_newline(head, line, i);
	if (parse_result == -1)
		return (-1);
	if (parse_result)
		return (0);
	parse_result = parse_word(head, line, i, length, shell);
	if (parse_result == -1)
		return (-1);
	if (parse_result)
		return (0);
	return (0);
}

t_token	*tokenize(t_token *head, const char *line, int length, t_shell *shell)
{
	int	i;

	if (!line || !*line)
	{
		ft_putstr_fd("Error: Empty line\n", STDERR_FILENO);
		return (NULL);
	}
	i = 0;
	while (i < length)
	{
		i = skip_whitespace(line, i);
		if (i >= length)
			break ;
		if (process_token_parsing(&head, line, &i, length, shell) == -1)
		{
			ft_putstr_fd("Error: Token parsing failed\n", STDERR_FILENO);
			free_tokens(&head);
			return (NULL);
		}
	}
	//head->exit_code = exit_code;
	return (head);
}

/**
 * @brief Parses a two-character operator from the given position in the line.
 *
 * This function checks if the current position in the string `line` begins
 * with a valid two-character operator such as "&&", "||", "<<" or ">>".
 * If a match is found:
 *   - Creates a corresponding token.
 *   - Appends the token to the linked list pointed to by `*head`.
 *   - Advances the parsing index (`*pos`) by the operator length.
 *   - Returns 1 to indicate that the operator was successfully parsed.
 * If no two-character operator is found:
 *   - Does not modify `*head` or `*pos` except for reading.
 *   - Returns 0 to indicate no operator was handled.

**/
int	parse_two_char_operator(t_token **head, const char *line,
							int *pos, int length)
{
	t_token_type	doublechar;
	t_token			*op_token;
	int				i;

	i = *pos;
	if (is_two_char_operator(line[i]) && (i + 1 < length))
	{
		doublechar = match_two_char_operator(line, i);
		if (doublechar != TOKEN_UNKNOWN)
		{
			op_token = create_new_token_range(doublechar,
					line, i, 2);
			if (!op_token)
				return (-1);
			if (append_token(head, op_token) == -1)
			{
				free(op_token);
				return (-1);
			}
			*pos += 2;
			return (1);
		}
	}
	return (0);
}

/**
 * @brief Parses a single-character operator from the given position in the line.
 *
 * This function checks if the current position in the string `line` contains
 * a valid single-character operator. If a match is found:
 *   - Creates a corresponding token.
 *   - Appends the token to the linked list pointed to by `*head`.
 *   - Advances the parsing index (`*pos`) by the operator length.
 *   - Returns 1 to indicate that the operator was successfully parsed.
 * If no single-character operator is found:
 *   - Does not modify `*head` or `*pos` except for reading.
 *   - Returns 0 to indicate no operator was handled.
 *
 * @param head A pointer to the head of the token linked list.
 *             The new token will be appended here if a match is found.
 * @param line The input string being tokenized.
 * @param pos  A pointer to the current parsing index in `line`.
 *             This is updated if a single-character operator is matched.
 * @param length The total length of the input string `line`.
 *               Used to prevent out-of-bounds access.
 *
 * @return int 1 if a single-character operator was successfully parsed,
 * 0 otherwise.
 */
int	parse_single_char_operator(t_token **head, const char *line,
		int *pos, int length)
{
	t_token_type	single;
	t_token			*op_token;

	if (*pos >= length)
		return (0);
	single = match_single_char_operator(line[*pos]);
	if (single != TOKEN_UNKNOWN)
	{
		op_token = create_new_token_range(single, line, *pos, 1);
		if (!op_token)
			return (-1);
		if (append_token(head, op_token) == -1)
		{
			free(op_token);
			return (-1);
		}
		*pos += 1;
		return (1);
	}
	return (0);
}

int	handle_newline(t_token **head, const char *line, int *pos)
{
	t_token	*nl_token;

	if (line[*pos] == '\n')
	{
		nl_token = create_new_token_range(TOKEN_NL, "\\n", 0, 2);
		if (!nl_token)
			return (-1);
		if (append_token(head, nl_token) == -1)
		{
			free(nl_token);
			return (-1);
		}
		(*pos)++;
		return (1);
	}
	return (0);
}

/**
 * @brief Parses a word from the current position in the input line.
 *
 * This function reads a sequence of non-operator, non-whitespace characters
 * (a "word") starting from the position indicated by `*pos`. If a valid word
 * is found:
 *   - Allocates memory for the word and creates a TOKEN_WORD token.
 *   - Appends the new token to the token list pointed to by `*head`.
 *   - Updates `*pos` to the position after the parsed word.
 *   - Returns 1 to indicate success.
 *
 * If no valid word is found at the current position (e.g., the character is
 * an operator or whitespace), the function does not modify `*head` or `*pos`,
 * and returns 0.
 *
 * If memory allocation fails, it frees any allocated memory and the token list
 * to prevent leaks, then returns 0 to indicate failure.
 *
 * @param head A pointer to the head of the token linked list. The new token
 *             will be appended here if a word is successfully parsed.
 * @param line The input string being tokenized.
 * @param pos  A pointer to the current parsing index in the input line. This
 *             is updated to point to the first character after the parsed word.
 *
 * @return int 1 if a word was successfully parsed and added to the token list,
 *             0 otherwise (e.g., no word found or memory allocation failure).
 */
int	parse_word(t_token **head, const char *line, int *pos, int length, t_shell *shell)
{
	t_token	*token;
	char	*word;

	if (*pos >= length)
		return (0);
	word = read_word_range(line, pos, length, shell);
	token = (t_token *)malloc(sizeof(t_token));
	if (!word || !token)
	{
		ft_putstr_fd("Memory allocation error in parse_word\n", STDERR_FILENO);
		free(word);
		free(token);
		free_tokens(head);
		return (-1);
	}
	token->type = TOKEN_WORD;
	token->value = word;
	token->next = NULL;
	if (append_token(head, token) == -1)
	{
		free(token);
		return (-1);
	}
	return (1);
}
