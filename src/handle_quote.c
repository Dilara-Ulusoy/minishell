#include "minishell.h"

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
            (*index)++; // Tırnağı atla
            if (*index < len && line[*index] == quote) // Arka arkaya iki tırnak var mı?
            {
                (*index)++; // İkinci tırnağı atla
                continue;
            }
            else
                break; // Eğer sadece tek tırnak kapandıysa çık
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
        fprintf(stderr, "Error: unclosed quote\n");
        return NULL;
    }

    temp[res_index] = '\0';
    return ft_strdup(temp);
}

