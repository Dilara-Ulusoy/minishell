#include "minishell.h"

/* Allocate memory for the result buffer */
static char *allocate_quote_buffer(const char *line)
{
	char *buffer;
	int len;
	int env_var_len;

	buffer = NULL;
	len = ft_strlen(line);
	env_var_len = get_env_var_length(line);
	if (env_var_len != 0)
	{
		buffer = (char *)malloc(len + env_var_len + 1);
		if (!buffer)
		{
			ft_putstr_fd("Error: Memory allocation failed\n", STDERR_FILENO);
			return (NULL);
		}
	}
	else
		buffer = (char *)malloc(len + 1);
	if (!buffer)
	{
		ft_putstr_fd("Error: Memory allocation failed\n", STDERR_FILENO);
		return (NULL);
	}
	return (buffer);
}

/* Expands an environment variable within a quoted string */
int handle_env_variable(const char *line, int *i, char *result, int *res_index)
{
	char	*var_name;
	char	*env_value;
	int		len;

	var_name = extract_env_var_name(line, i);
	if (!var_name)
		return (0);
	env_value = getenv(var_name);
	free(var_name);
	if (!env_value)
		return (1);
	len = ft_strlen(env_value);
	ft_memcpy(&result[*res_index], env_value, len);
	*res_index += len;
	return (1);
}

/* Processes quoted string content */
static int process_quoted_content(const char *line, int *i, char quote, char *result)
{
	int res_index = 0;
	int len = (int)ft_strlen(line);
	while (*i < len)
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
	char *result;

	result = allocate_quote_buffer(line);
	if (!result)
		return (NULL);

	(*i)++; // Skip the opening quote
	if (!process_quoted_content(line, i, quote, result))
	{
		printf("Syntax error: Unclosed %c quote\n", quote);
		free(result);
		exit(1);
	}
	return (result);
}