#include "minishell.h"

static int expand_env_variable(const char *line, int *index, char **temp, int *res_index, int *temp_size)
{
	char	*env_value;
	int		env_len;

	env_value = get_env_var_value(line, index);
	if (!env_value)
		return (0);
	env_len = strlen(env_value);
	if (*res_index + env_len >= *temp_size)
	{
		*temp_size += env_len;
		*temp = ft_realloc(*temp, *temp_size);
		if (!*temp)
		{
			free(env_value);
			return (-1);
		}
	}
	ft_memcpy(*temp + *res_index, env_value, env_len);
	*res_index += env_len;
	free(env_value);
	return (1);
}

static int extract_quoted_content(const char *line, int *index, char quote, char **temp, int *res_index)
{
    int temp_size = ft_strlen(line) + 1;

    while (*index < temp_size && line[*index] != quote)
    {
        if (quote == '"' && line[*index] == '$') // Çevre değişkeni genişletme
        {
            if (expand_env_variable(line, index, temp, res_index, &temp_size) == -1)
                return -1;
            continue;
        }
        if (*res_index >= temp_size - 1) // Ensure buffer has enough space
        {
            temp_size *= 2;
            char *new_temp = ft_realloc(*temp, *res_index);
            if (!new_temp)
                return -1;
            *temp = new_temp;
        }
        (*temp)[(*res_index)++] = line[(*index)++];
    }
    return 0;
}

static char *handle_nested_quotes(const char *line, int *index, int len, char *temp, int *res_index)
{
    char inner_quote;

    while (*index < len && (line[*index] == '\'' || line[*index] == '"'))
    {
        inner_quote = line[*index];
        (*index)++;

        while (*index < len && line[*index] != inner_quote)
        {
            temp[*res_index] = line[*index];
            (*res_index)++;
            (*index)++;
        }
        if (*index >= len || line[*index] != inner_quote) // Error: unclosed inner quote
        {
            ft_putstr_fd("Error: unclosed inner quote\n", STDERR_FILENO);
            free(temp);
            return NULL;
        }
        (*index)++; // Move past the closing inner quote
    }
    return temp;
}

static int check_closing_quote(const char *line, int *index, int len, char *temp, char quote)
{
	if (*index >= len || line[*index] != quote)
	{
		ft_putstr_fd("Error: unclosed quote\n", STDERR_FILENO);
		free(temp);
		return (-1);
	}
	(*index)++;
	return (0);
}

char *process_quoted_content(const char *line, int *index, char quote, int len)
{
	int res_index = 0;
	char *temp;

	temp = (char *)malloc(len + 1);
	if (!temp)
		return (NULL);
	(*index)++; // İlk tırnağı atla
	if (extract_quoted_content(line, index, quote, &temp, &res_index) == -1)
	{
		free(temp);
		return (NULL);
	}
	if (check_closing_quote(line, index, len, temp, quote) == -1)
		return (NULL);

    temp = handle_nested_quotes(line, index, len, temp, &res_index);
    if (!temp)
        return NULL;

	temp[res_index] = '\0';
	return (temp);
}

/*
char *process_quoted_content(const char *line, int *index, char quote, int len)
{
    int res_index = 0;
    char *env_value;
    char temp[1024];

    (*index)++; // İlk tırnağı atla

    while (*index < len)
    {
        if (line[*index] == quote) // Eğer aynı tırnak tekrar geldiyse kontrol et
        {
            //(*index)++; // Tırnağı atla
            if (*index < len && line[*index] == quote) // Arka arkaya iki tırnak var mı?
            {
                (*index)++;
                continue;
            }
            else
                break;
        }
        if (quote == '"' && line[*index] == '$') // Çift tırnak içinde değişken varsa
        {
            env_value = get_env_var_value(line, index);
            if (env_value)
            {
                ft_memcpy(temp + res_index, env_value, ft_strlen(env_value));
                res_index += ft_strlen(env_value);
                free(env_value);
            }
        }
        else
        {
            temp[res_index++] = line[*index];
            (*index)++;
        }
    }
    if (*index > len)
    {
        ft_putstr_fd("Error: unclosed quote", STDERR_FILENO);
        return NULL;
    }
    temp[res_index] = 0;
    return ft_strdup(temp);
}

*/

