#ifndef TOKEN_H
# define TOKEN_H

/*
    token_type:
    - Each enumerator corresponds to a possible category of token.
    - Requirements:
        1) We should handle word tokens (like "ls", "echo").
        2) We should handle single-character operators (<, >, |, (, )).
        3) We should handle multi-character operators (&&, ||, <<, >>).
        4) If needed, we can include TOKEN_UNKNOWN for unrecognized sequences.
*/
typedef enum e_token_type
{
    TOKEN_WORD,         /* Represents a generic word (e.g. "echo", "hello123") */
    TOKEN_AND,          /* '&&' logical AND operator */
    TOKEN_OR,           /* '||' logical OR operator */
    TOKEN_PIPE,         /* '|' pipeline operator */
    TOKEN_REDIR_IN,     /* '<' input redirection */
    TOKEN_REDIR_OUT,    /* '>' output redirection */
    TOKEN_REDIR_APPEND, /* '>>' append output redirection */
    TOKEN_REDIR_HERE,   /* '<<' here-document operator */
    TOKEN_PAREN_OPEN,   /* '(' left parenthesis */
    TOKEN_PAREN_CLOSE,  /* ')' right parenthesis */
    TOKEN_NL,           /* newline */
    TOKEN_UNKNOWN       /* fallback if something doesn't match known operators */
} t_token_type;

/*
    s_token:
    - This structure is used to build a linked list of tokens.
    - Each node (s_token) has:
        1) 'type'  -> Which category the token falls under (from token_type).
        2) 'value' -> The actual string text (e.g., "echo", "&&").
        3) 'next'  -> Pointer to the next token in the linked list.

    Requirements for a linked-list based token representation:
        - We can easily insert tokens in the order we find them while scanning
          the input string.
        - After the tokenize method finishes, this linked list can be
          passed to the parser to build an Abstract Syntax Tree (AST).
*/
typedef struct s_token
{
    t_token_type      type;       /* Which category this token belongs to */
    char            *value;     /* The actual string content of the token */
    struct s_token  *next;      /* Pointer to the next token in the list */
} t_token;


/* Token Creation and Management */
t_token *tokenize(t_token *head, const char *line);
t_token *create_new_token_range(t_token_type type, const char *line, int startIndex, int length);
void     append_token(t_token **head, t_token *new_token);
void     free_tokens(t_token **head);


/* Token Utilities */
int      is_operator(t_token_type type);
int      is_binary_operator(t_token_type type);
int      is_redirection(t_token_type type);
int      get_precedence(t_token_type type);
int      is_two_char_operator(char c);
t_token_type match_two_char_operator(const char *line, int index);
t_token_type match_single_char_operator(char c);

/* Debugging */
void print_tokens(const t_token *head);


#endif


