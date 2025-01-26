#include "minishell.h"

/* Allocate memory for the result buffer */
static char *allocate_quote_buffer(int length)
{
	char *buffer;

	buffer = (char *)malloc(length + 1);
	if (!buffer)
	{
		printf("Error: Memory allocation failed\n");
		return (NULL);
	}
	return (buffer);
}

/* Expands an environment variable within a quoted string */
static int handle_env_variable(const char *line, int *i, char *result, int *res_index)
{
	char *env_value;

	env_value = expand_env_var(line, i);
	if (!env_value)
		return (0);
	ft_memcpy(&result[*res_index], env_value, ft_strlen(env_value));
	*res_index += ft_strlen(env_value);
	free(env_value);
	return (1);
}

/* Processes quoted string content */
static int process_quoted_content(const char *line, int *i, char quote, char *result)
{
	int res_index = 0;
	int length = (int)ft_strlen(line);
	while (*i < length)
	{
		if (line[*i] == quote) // Closing quote found
		{
			(*i)++;
			result[res_index] = '\0';
			return (1); // Successful parsing
		}
		else if (quote == '"' && line[*i] == '$') // Handle environment variables
		{
			if (!handle_env_variable(line, i, result, &res_index))
				return (0);
		}
		else // Normal character inside quotes
		{
			result[res_index++] = line[*i];
			(*i)++;
		}
	}
	return (0); // Unclosed quote error
}

/* Main function to handle quoted strings */
char *handle_quotes(const char *line, int *i, char quote)
{
	int		length = (int)ft_strlen(line);
	char	*result;

	result = allocate_quote_buffer(length);
	if (!result)
		return (NULL);

	(*i)++; // Skip the opening quote
	if (!process_quoted_content(line, i, quote, result))
	{
		printf("Syntax error: Unclosed %c quote\n", quote);
		free(result);
		return (NULL);
	}
	return (result);
}