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
			return (-1); // Bellek hatası
		}
	}
	ft_memcpy(*temp + *res_index, env_value, env_len);
	*res_index += env_len;
	free(env_value);
	return (1); // Çevre değişkeni başarıyla genişletildi
}

static char *process_inner_quotes(const char *line, int *index, int len, char *temp, int *res_index)
{
	char inner_quote;

	if (*index < len && (line[*index] == '\'' || line[*index] == '"'))
	{
		inner_quote = line[*index];
		(*index)++;
		while (*index < len && line[*index] != inner_quote)
		{
			temp[*res_index] = line[*index];
			(*res_index)++;
			(*index)++;
		}
		if (*index >= len || line[*index] != inner_quote)
		{
			ft_putstr_fd("Error: unclosed inner quote\n", STDERR_FILENO);
			return (NULL);
		}
		(*index)++;
	}
	return (temp);
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
    int temp_size = len + 1; // Başlangıçta len + 1 bellek ayırıyoruz.

    temp = (char *)malloc(temp_size);
    if (!temp)
        return (NULL);
    (*index)++; // İlk tırnağı atla
    while (*index < len && line[*index] != quote)
    {
        if (quote == '"' && line[*index] == '$') // Çevre değişkeni genişletme
        {
           if (expand_env_variable(line, index, &temp, &res_index, &temp_size) == -1)
            {
                free(temp);
                return (NULL);
            }
            continue;
        }
        temp[res_index++] = line[*index];
        (*index)++;
    }
    if (check_closing_quote(line, index, len, temp, quote) == -1)
        return NULL;
    while (*index < len && (line[*index] == '\'' || line[*index] == '"'))
    {
        temp = process_inner_quotes(line, index, len, temp, &res_index);
        if (!temp)
            return (NULL);
    }
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

