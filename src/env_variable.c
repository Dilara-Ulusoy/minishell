#include "minishell.h"

//* Çevresel değişkeni çözümleyen fonksiyon */
char *expand_env_var(const char *line, int *index)
{
    t_parser parser; // Varsayılan olarak parser.error_status = PARSE_OK olmalı
    int start = *index + 1;
    int word_length = 0;
    char *var_name = NULL;
    char *result = NULL;
    const char *env_value;

    // Değişken uzunluğu hesaplama
    while (line[start + word_length] && (ft_isalnum(line[start + word_length]) || line[start + word_length] == '_'))
        word_length++;

    if (word_length == 0)
	{
        (*index)++;
        return ft_strdup("$");
    }
    var_name = malloc(word_length + 1);     // Bellek ayırma
    if (!var_name)
        return handle_allocation_error(&parser, NULL, "malloc");

    ft_memcpy(var_name, &line[start], word_length);
    var_name[word_length] = '\0';
    *index += word_length + 1;

    env_value = getenv(var_name);
    if (env_value)
        result = ft_strdup(env_value);
    else
        result = ft_strdup("");

    if (!result)
        return handle_allocation_error(&parser, var_name, "ft_strdup");

    free(var_name);
    return result;
}


// New function definition
void handle_env_var(const char *line, int *i, char *result, int *res_index)
{
    char *env = expand_env_var(line, i);
    if (!env)
        return;
    ft_memcpy(&result[*res_index], env, ft_strlen(env));
    *res_index += ft_strlen(env);
    free(env);
}