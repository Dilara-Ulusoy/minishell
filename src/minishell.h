#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>


#include "../Libft/libft.h"

typedef struct s_shell
{
	char *line;
	char *tokens;

} t_shell;

typedef enum s_token_type
{
	T_COMMAND, // command; ls, echo, cat Type: 0
	T_ARG, // argument Type: 1
	T_PIPE,  		// | Type: 2
	T_REDIRECT_OUT, // > Type: 3
	T_REDIRECT_IN, // < Type: 4
	T_REDIRECT_APPEND, // >> Type: 5
	T_REDIRECT_HEREDOC, // << Type: 6
	T_OPEN_P, // ( Type: 7
	T_CLOSE_P, // ) Type: 8
	T_AND, // && Type: 9
	T_OR, // || Type: 10
	T_NL, // new line Type: 11
}
t_token_type;


typedef struct s_token
{
	t_token_type type;
	char *value;
	bool is_operator;
	bool expect_command;
	struct s_token *next;
	struct s_token *prev;
} t_token;

typedef struct s_parser_state {
    bool expect_command;
} t_parser_state;



char *get_input(void);


//Tokenization

void tokenize(char *line, t_token **tokens);
t_token *create_token(char *value, t_token_type type);
void add_token(t_token **head, char *value, t_token_type type);
void print_tokens(t_token *head);
void free_tokens(t_token *head);
t_token_type get_token_type(char *token);
bool is_operator_token(t_token_type type);
void determine_token_types(t_token *tokens, t_parser_state *state);


// Utils
bool is_quote(char c);


