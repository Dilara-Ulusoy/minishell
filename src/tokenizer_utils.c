#include "minishell.h"


int skip_whitespace(const char *line, int i)
{
    while (line[i] && is_space(line[i]))
        i++;
    return i;
}

int is_space(char c)
{
    if(c == ' ' || c == '\t' || c == '\r' || c == '\v' || c == '\f')
        return 1;
    return 0;
}

/*
   is_two_char_operator:
   Returns non-zero (true) if 'c' can start a two-char operator like:
    - '&' for "&&"
    - '|' for "||"
    - '<' for "<<"
    - '>' for ">>"
*/
int is_two_char_operator(char c)
{
    return (c == '&' || c == '|' || c == '<' || c == '>');
}

char *allocate_word(const char *line, int start, int length)
{
    char *word;

    word = (char *)malloc(length + 1);
    if (!word)
        return NULL;
    ft_memcpy(word, &line[start], length);
    word[length] = '\0';
    return word;
}

/* Handle quotes and return the processed string */
char *handle_quotes(const char *line, int *i, char quote)
{
    int length = (int)ft_strlen(line);
    char *result = (char *)malloc(length + 1);
    char *env_value;

    if (!result)
    {
        perror("malloc");
        return NULL;
    }
    int res_index = 0;

    // Tırnağı atla
    (*i)++;

    while (*i < length)
    {
        if (line[*i] == quote) // Kapatıcı tırnak bulundu
        {
            (*i)++;
            result[res_index] = '\0';
            return result; // İşlenmiş stringi döndür
        }
        else if (quote == '"' && line[*i] == '$') // Çift tırnak içinde $ işareti EG: echo "hello $USER"
        {
            env_value = expand_env_var(line, i);
            ft_memcpy(&result[res_index], env_value, ft_strlen(env_value));
            res_index += ft_strlen(env_value);
            free(env_value);
        }
        else // Normal bir karakter ise EG: echo "hello"
        {
            result[res_index++] = line[*i];
            (*i)++;
        }
    }
    // Eğer tırnak kapatılmadan sona ulaşılırsa
    printf("Syntax error: Unclosed %c quote\n", quote);
    free(result);
    return NULL; // Hata
}

