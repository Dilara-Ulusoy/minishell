#include "minishell.h"

/**
 * expand_env_variable - Expands an environment variable found in the input string.
 *
 * @line: The input string.
 * @index: Pointer to the current position in the string (updated after expansion).
 * @buffer: Pointer to the buffer where expanded content is stored.
 * @res_index: Pointer to the buffer index where content should be added.
 * @buffer_size: Pointer to the size of the buffer (expanded if needed).
 *
 * Return: 1 if expansion is successful, -1 if an error occurs.
 */
static int expand_env_variable(const char *line, int *index, char **buffer, int *res_index, int *buffer_size)
{
	char	*env_value;
	int		env_len;

	env_value = get_env_var_value(line, index);
	if (!env_value)
		return (0);
	env_len = strlen(env_value);
	if (*res_index + env_len >= *buffer_size)
	{
		*buffer_size += env_len;
		*buffer = ft_realloc(*buffer, *buffer_size);
		if (!*buffer)
		{
			free(env_value);
			return (-1);
		}
	}
	ft_memcpy(*buffer + *res_index, env_value, env_len);
	*res_index += env_len;
	free(env_value);
	return (1);
}
/**
 * extract_quoted_content - Extracts content from within quotes.
 *
 * @line: The input string.
 * @index: Pointer to the current position in the string (updated during parsing).
 * @quote: The type of quote (single or double).
 * @result: Pointer to the buffer where the extracted content is stored.
 * @res_index: Pointer to the index in the result buffer where content should be added.
 *
 * Return: 0 if successful, -1 if an error occurs.
 */
static int extract_quoted_content(const char *line, int *index, char quote, char **result, int *res_index)
{
	int	buffer_size;
	char *new_buffer;

	buffer_size = 32;
	while (*index < buffer_size && line[*index] != quote)
	{
		if (quote == '"' && line[*index] == '$')
		{
			if (expand_env_variable(line, index, result, res_index, &buffer_size) == -1)
				return (-1);
			continue;
		}
		if (*res_index >= buffer_size - 1)
		{
			buffer_size *= 2; // Double the buffer size
			new_buffer = ft_realloc(*result, buffer_size);
			if (!new_buffer)
			{
				free(*result); // Prevent memory leak
				return (-1);
			}
			*result = new_buffer;
		}
		(*result)[(*res_index)++] = line[(*index)++];
	}
	return 0;
}

/**
 * handle_nested_quotes - Processes nested quotes within quoted content.
 *
 * @line: The input string.
 * @index: Pointer to the current position in the string (updated during parsing).
 * @len: The total length of the input string.
 * @buffer: Pointer to the buffer where extracted content is stored.
 * @res_index: Pointer to the index in the result buffer.
 *
 * Return: The updated buffer if successful, NULL if an error occurs.
 */
static char *handle_nested_quotes(const char *line, int *index, int len, char *buffer, int *res_index)
{
	char inner_quote;

	while (*index < len && (line[*index] == '\'' || line[*index] == '"'))
	{
		inner_quote = line[*index];
		(*index)++;

		while (*index < len && line[*index] != inner_quote)
		{
			buffer[*res_index] = line[*index];
			(*res_index)++;
			(*index)++;
		}
		if (*index >= len || line[*index] != inner_quote) // Error: unclosed inner quote
		{
			ft_putstr_fd("Error: unclosed inner quote\n", STDERR_FILENO);
			free(buffer);
			return NULL;
		}
		(*index)++; // Move past the closing inner quote
	}
	return buffer;
}

/**
 * check_closing_quote - Ensures that a quoted section has a proper closing quote.
 *
 * @line: The input string.
 * @index: Pointer to the current position in the string.
 * @len: The total length of the input string.
 * @buffer: The buffer containing the quoted content.
 * @quote: The expected closing quote character.
 *
 * Return: 0 if successful, -1 if an error occurs.
 */
static int check_closing_quote(const char *line, int *index, int len, char *buffer, char quote)
{
	if (*index >= len || line[*index] != quote)
	{
		ft_putstr_fd("Error: unclosed quote\n", STDERR_FILENO);
		free(buffer);
		return (-1);
	}
	(*index)++;
	return (0);
}

/**
 * process_quoted_content - Extracts and processes content enclosed within quotes.
 *
 * @line: The input string.
 * @index: Pointer to the current position in the string (updated during parsing).
 * @quote: The type of quote (single or double).
 * @len: The total length of the input string.
 *
 * Return: A newly allocated string containing the extracted quoted content.
 */
char *process_quoted_content(const char *line, int *index, char quote, int len)
{
	int i;
	char *result;

	i = 0;
	result = (char *)malloc(len + 1);
	if (!result)
		return (NULL);
	(*index)++; // İlk tırnağı atla
	if (extract_quoted_content(line, index, quote, &result, &i) == -1)
	{
		free(result);
		return (NULL);
	}
	if (check_closing_quote(line, index, len, result, quote) == -1)
		return (NULL);

	result = handle_nested_quotes(line, index, len, result, &i);
	if (!result)
		return NULL;

	result[i] = '\0';
	return (result);
}
