/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoc@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 22:34:26 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/01/30 16:55:38 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief tokenize
 * Splits the line string into a linked list of tokens.
 * Uses smaller helper methods for readability and memory correctness.
 *
 * @param line The user line (e.g. "echo hello && cat < file.txt").
 * @return Pointer to the first token in a linked list, or NULL if empty.
 */
t_token	*tokenize(t_token *head, const char *line)
{
	int	length;
	int	i;

	if (!line || !*line)
	{
		ft_putstr_fd("Error: Empty line", STDERR_FILENO);
		return (NULL);
	}
	length = (int)ft_strlen(line);
	i = 0;
	while (i < length)
	{
		i = skip_whitespace(line, i);
		if (i >= length)
			break; // no more content left
		if (parse_two_char_operator(&head, line, &i, length) ||
			parse_single_char_operator(&head, line, &i, length) ||
			handle_newline(&head, line, &i))
			continue;
		if (!parse_word(&head, line, &i, length))
		{
			free_tokens(&head);
			return (NULL);
		}
	}
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
int	parse_two_char_operator(t_token **head, const char *line, int *pos, int length)
{
	t_token_type doublechar;
	t_token *opToken;
	int i;
	int operator_length;

	i = *pos;
	if (is_two_char_operator(line[i]) && (i + 1 < length))
	{
		doublechar = match_two_char_operator(line, i);
		if (doublechar != TOKEN_UNKNOWN)
		{
			operator_length = 2;
			/* Create the new token */
			opToken = create_new_token_range(doublechar, line, i, operator_length);
			append_token(head, opToken);
			/* Move pos forward by operator_length */
			*pos += operator_length;
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
 * @return int 1 if a single-character operator was successfully parsed, 0 otherwise.
 */
int	parse_single_char_operator(t_token **head, const char *line, int *pos, int length)
{
	t_token_type	single;
	t_token			*opToken;
	int				operator_length;

	if (*pos >= length) // Boundary check to prevent out-of-bounds access
		return (0);
	single = match_single_char_operator(line[*pos]);
	if (single != TOKEN_UNKNOWN)
	{
		operator_length = 1;
		opToken = create_new_token_range(single, line, *pos, operator_length);
		append_token(head, opToken);
		*pos += operator_length;
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

int	parse_word(t_token **head, const char *line, int *pos, int length)
{
	t_token	*token;
	char	*word;

	if (*pos >= length) // Check if *pos is within bounds
		return (0);
	word = read_word_range(line, pos, length);
	if (!word)
		return (0);
	//printf("Allocated Token: %p ------>%s\n", (void *)word, word);
	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
	{
		free(word);
		free_tokens(head);
		return (0);
	}
	token->type = TOKEN_WORD;
	token->value = word;
	token->next = NULL;
	append_token(head, token);
	return (1);
}

static char	*process_quoted_string(const char *line, int *index, char quote_char)
{
	char	*processed;

	processed = handle_quotes(line, index, quote_char);
	if (!processed)
	{
		ft_putstr_fd("Error: Handle quotes failed\n", STDERR_FILENO);
		return (NULL);
	}
	return (processed);
}

static char *expand_env_variable(const char *line, int *index)
{
	char *var_name;
	char *env_value;
	char *result;

	(*index)++; /* Skip $ character */
	var_name = extract_env_var_name(line, index);
	if (!var_name)
		return (NULL);
	env_value = getenv(var_name);
	free(var_name);
	if (!env_value)
		return (NULL);
	result = ft_strdup(env_value);
	if (!result)
	{
		ft_putstr_fd("Error: Memory allocation failed\n", STDERR_FILENO);
		return (NULL);
	}
	return (result);
}

/**
 * @brief Extracts a word from the input line, handling special cases like quotes.
 *
 * This function reads a sequence of characters (a "word") starting from the
 * current index and ending at the next delimiter (e.g., whitespace, operator).
 * If quotes are encountered, it processes the quoted string correctly.
 *
 * @param line The input string being parsed.
 * @param index A pointer to the current position in the input string. Updated
 *              to the position just after the end of the word on exit.
 * @param length The total length of the input string. Used to ensure bounds safety.
 *
 * @return A dynamically allocated string containing the extracted word,
 *         or NULL if no valid word is found. The caller is responsible for
 *         freeing the returned string.
 */
char *read_word_range(const char *line, int *index, int length)
{
	int		start; /* Remember where the word starts */
	int		wordLength;
	char	c;

	start = *index;
	while (*index < length)
	{
		c = line[*index];
		if (c == '"' || c == '\'')
			return process_quoted_string(line, index, c);
		if (c == '$')
			return expand_env_variable(line, index);
		if (is_space(c) || is_two_char_operator(c) || c == '(' || c == ')' || c == '\n')
			break;
		(*index)++;
	}
	wordLength = (*index) - start;
	if (wordLength == 0)
		return (NULL);
	return allocate_word(line, start, wordLength);
}

int	handle_newline(t_token **head, const char *line, int *pos)
{
	t_token	*nl_token;

	if (line[*pos] == '\n')
	{
		nl_token = create_new_token_range(TOKEN_NL, "\\n", 0, 2);
		append_token(head, nl_token);
		(*pos)++;
		return (1);
	}
	return (0);
}

