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


int handle_quotes(const char *line, int *index, char quote)
{
    int length = (int)ft_strlen(line);
    char *new_line;

    /* Açılış tırnağını atla */
    (*index)++;

    while (*index < length)
    {
        if (line[*index] == quote) /* Kapanış tırnağı bulundu */
        {
            (*index)++;
            return 1; /* Başarıyla tamamlandı */
        }
        (*index)++;
    }
    /* Quote kapanmadıysa, kullanıcıdan daha fazla girdi iste */
    if(quote == '\'')
        new_line = get_input("quote> ");
    else
        new_line = get_input("dquote> ");
    if (!new_line)
    {
        printf("Syntax error: Unclosed quote\n");
        return 0; /* Kullanıcı daha fazla girdi vermezse hata */
    }

    /* Yeni satırı birleştir */
    char *temp = ft_strjoin(line, "\n");
    if(!temp)
    {
        free(new_line);
        return 0;
    }
    char *joined_line = ft_strjoin(temp, new_line);
    if(!joined_line)
    {
        free(new_line);
        return 0;
    }
    free(new_line);
    free(temp);

    /* Yeni satırı recursive olarak işleme devam et */
    return handle_quotes(joined_line, index, quote);
}
