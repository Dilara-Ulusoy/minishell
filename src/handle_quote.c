#include "minishell.h"

char *process_quoted_content(const char *line, int *index, char quote, int len)
{
    int res_index = 0;
    char *env_value;
    char temp[1024];

    temp[res_index++] = quote;
    (*index)++;

    while (*index < len && line[*index] != quote)
    {
        if (quote == '"' && line[*index] == '$')
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
    if (*index >= len)
    {
        fprintf(stderr, "Error: unclosed quote\n");
        return NULL;
    }
    temp[res_index++] = quote;
    (*index)++;
    temp[res_index] = '\0';
    return ft_strdup(temp);
}
