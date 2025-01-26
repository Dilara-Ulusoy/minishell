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

t_token *create_new_token_range(t_token_type type,
                                const char *line,
                                int startIndex,
                                int length)
{
    t_token *new_token = (t_token *)malloc(sizeof(t_token)); // Allocate memory for the new token
    if (!new_token)
        return NULL;
    new_token->type = type;
    new_token->value = (char *)malloc((length + 1) * sizeof(char));    // 2. Allocate memory for the token's value string
    if (!new_token->value)
    {
        free_tokens(&new_token);
        return NULL;
    }
    // 3. Copy the substring from line into new_token->value
    ft_memcpy(new_token->value, &line[startIndex], length + 1); // Ensure length + 1
    new_token->value[length] = '\0';                             // Null-terminate for safety

    // 4. Initialize next pointer
    new_token->next = NULL;
    return new_token;
}

