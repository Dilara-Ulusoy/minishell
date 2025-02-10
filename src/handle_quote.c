#include "minishell.h"

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

static int extract_quoted_content(const char *line, int *index, char quote, char **buffer, int *res_index)
{
	int buffer_size = 32;

	while (*index < buffer_size && line[*index] != quote)
	{
		if (quote == '"' && line[*index] == '$')
		{
			if (expand_env_variable(line, index, buffer, res_index, &buffer_size) == -1)
				return -1;
			continue;
		}
		if (*res_index >= buffer_size - 1)
		{
			buffer_size *= 2; // Double the buffer size
			char *new_buffer = ft_realloc(*buffer, buffer_size);
			if (!new_buffer)
			{
				free(*buffer); // Prevent memory leak
				return -1;
			}
			*buffer = new_buffer;
		}
		(*buffer)[(*res_index)++] = line[(*index)++];
	}
	return 0;
}

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

char *process_quoted_content(const char *line, int *index, char quote, int len)
{
	int res_index;
	char *buffer;

	res_index = 0;
	buffer = (char *)malloc(len + 1);
	if (!buffer)
		return (NULL);
	(*index)++; // İlk tırnağı atla
	if (extract_quoted_content(line, index, quote, &buffer, &res_index) == -1)
	{
		free(buffer);
		return (NULL);
	}
	if (check_closing_quote(line, index, len, buffer, quote) == -1)
		return (NULL);

	buffer = handle_nested_quotes(line, index, len, buffer, &res_index);
	if (!buffer)
		return NULL;

	buffer[res_index] = '\0';
	return (buffer);
}
