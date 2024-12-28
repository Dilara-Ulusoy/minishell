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
	T_WORD, // argument Type: 0
	T_PIPE,  		// | Type: 1
	T_REDIRECT_OUT, // > Type: 2
	T_REDIRECT_IN, // < Type: 3
	T_REDIRECT_APPEND, // >> Type: 4
	T_REDIRECT_HEREDOC, // << Type: 5
	T_OPEN_P, // ( Type: 6
	T_CLOSE_P, // ) Type: 7
	T_AND, // && Type: 8
	T_OR, // || Type: 9
	T_NL, // new line Type: 10
	T_GROUP,
}
t_token_type;


typedef struct s_token
{
	t_token_type type;
	char *value;
	bool is_operator;
	struct s_token *next;
	struct s_token *prev;
} t_token;



char *get_input(void);


//Tokenization

void tokenize(char *line, t_token **tokens);
t_token *create_token(char *value, t_token_type type);
void add_token(t_token **head, char *value, t_token_type type);
void print_tokens(t_token *head);
void free_tokens(t_token *head);
t_token_type get_token_type(char *token);
bool is_operator_token(t_token_type type);
void determine_token_types(t_token *tokens);


// Utils
char **split_tokens(const char *line);


