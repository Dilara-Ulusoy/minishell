#include "minishell.h"

// Operatör mü?
bool	is_operator_token(t_token_type type)
{
	if (type == T_PIPE || type == T_REDIRECT_OUT || type == T_REDIRECT_IN
		|| type == T_REDIRECT_APPEND || type == T_REDIRECT_HEREDOC
		|| type == T_AND || type == T_OR || type == T_OPEN_P
		|| type == T_CLOSE_P || type == T_NL)
		return (true);
	return (false);
}

bool is_operator(char c)
{
    return (c == '|' || c == '>' || c == '<' || c == '&' || c == '(' || c == ')' || c == '\n');
}
bool is_group(t_token *token)
{
    if (!token || !token->value)
        return false;

    char first_char = token->value[0];
    return (first_char == '(' || first_char == ')');
}
bool contains_wildcard(const char *token) {
    while (*token) {
        if (*token == '*' || *token == '?' || *token == '[' || *token == ']')
            return true; // Joker karakter bulundu
        token++;
    }
    return false; // Joker karakter yok
}


// Yeni bir token oluştur
t_token *create_token(char *value, t_token_type type) {
    t_token *new_token = malloc(sizeof(t_token));
    new_token->value = ft_strdup(value);
    new_token->type = type;
    new_token->is_operator = is_operator_token(type);
    new_token->next = NULL;
    return new_token;
}

// Tokeni listeye ekle
void add_token(t_token **head, char *value, t_token_type type) {
    t_token *new_token = create_token(value, type);
    if (*head == NULL) // Eger liste bos ise yeni tokeni basa ekle
        *head = new_token;
    else // Listenin sonuna ekle
    {
        t_token *current = *head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = new_token;
    }
}

// Token listesini yazdır
void print_tokens(t_token *head) {
    t_token *current = head;
    while (current != NULL) {
        printf("Token: %s, Type: %u\n", current->value, current->type);
        current = current->next;
    }
}

// Listeyi temizle
void free_tokens(t_token *head) {
    t_token *current = head;
    t_token *next;
    while (current != NULL) {
        next = current->next;
        free(current->value);
        free(current);
        current = next;
    }
}