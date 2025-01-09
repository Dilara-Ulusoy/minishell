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

    /* Move past the opening quote. */
    (*index)++;

    /* Traverse until a matching closing quote is found. */
    while (*index < length)
    {
        if (line[*index] == quote) /* Matching closing quote found */
        {
            (*index)++;
            return 1; /* Successfully handled the quote */
        }
        (*index)++;
    }
    /* No matching closing quote found */
    printf("Syntax error: Unclosed quote\n");
    return 0;
}
