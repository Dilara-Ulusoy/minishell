#include "minishell.h"


/*
   append_token:
   - Adds 'new_token' to the end of the linked list whose head is '*head'.
   - If *head is NULL, then new_token becomes the first and only element.
*/
void append_token(t_token **head, t_token *new_token)
{
    if (!new_token)
        return;

    if (!*head)
    {
        /* If the list is empty, new_token is the new head. */
        *head = new_token;
        return;
    }
    t_token *cursor = *head;
    while (cursor->next)
        cursor = cursor->next; /* move to the end of the list */

    cursor->next = new_token; /* append at the end */
}

int skip_whitespace(const char *line, int i)
{
    while (line[i] && is_space(line[i]))
        i++;
    return i;
}

int is_space(char c)
{
    if(c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f')
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
