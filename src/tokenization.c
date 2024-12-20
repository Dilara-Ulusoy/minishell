#include "minishell.h"

// Yeni bir token oluştur
t_token *create_token(char *value, t_token_type type) {
    t_token *new_token = malloc(sizeof(t_token));
    new_token->value = ft_strdup(value);
    new_token->type = type;
    new_token->next = NULL;
    return new_token;
}

// Tokeni listeye ekle
void add_token(t_token **head, char *value, t_token_type type) {
    t_token *new_token = create_token(value, type);
    if (*head == NULL) {
        *head = new_token;
    } else {
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
        printf("Token: %s, Type: %d\n", current->value, current->type);
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
/*
int main() {
    t_token *tokens = NULL;

    // Token ekleme
    add_token(&tokens, "echo", TOK_WORD);
    add_token(&tokens, "|", TOK_PIPE);
    add_token(&tokens, "grep", TOK_WORD);
    add_token(&tokens, "output.txt", TOK_REDIRECT_OUT);

    // Tokenları yazdır
    print_tokens(tokens);

    // Bellek temizleme
    free_tokens(tokens);

    return 0;
}
*/