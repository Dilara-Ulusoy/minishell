#include "minishell.h"

//* Çevresel değişkeni çözümleyen fonksiyon */
char *expand_env_var(const char *line, int *index)
{
    int start = *index + 1; // '$' işaretinden sonraki pozisyon
    int word_length = 0;
	char *var_name;
	char *result;
	const char *env_value;

    // Boşluk veya geçersiz bir karakter görünene kadar uzunluğu hesapla
    while (line[start + word_length] && (ft_isalnum(line[start + word_length]) || line[start + word_length] == '_'))
        word_length++;

    if (word_length == 0)
    {
        (*index)++;
        return ft_strdup("$"); // Sadece '$' karakterini döndür
    }
    var_name = malloc(word_length + 1);
    if (!var_name)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
	ft_memcpy(var_name, &line[start], word_length);
    var_name[word_length] = '\0';
    *index += word_length + 1;
	printf("var_name: %s\n", var_name);

    env_value = getenv(var_name); // Çevresel değişkenin değerini al
    if (env_value)
		result = ft_strdup(env_value);
    else
        result = ft_strdup("");
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