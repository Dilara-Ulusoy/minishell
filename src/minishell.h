#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <string.h>
#include "../Libft/libft.h"

typedef struct s_shell
{
	char *line;
	char *tokens;

} t_shell;

typedef enum s_token_type
{
	T_COMMAND, // command; ls, echo, cat
	T_ARG, // argument
	T_PIPE,  		// |
	T_REDIRECT_OUT, // >
	T_REDIRECT_IN, // <
	T_REDIRECT_APPEND, // >>
	T_REDIRECT_HEREDOC, // <<
	T_OPEN_P, // (
	T_CLOSE_P, // )
	T_AND, // &&
	T_OR, // ||
	T_NL, // new line
}
t_token_type;


typedef struct s_token
{
	t_token_type type;
	char *value;
	struct s_token *next;
	struct s_token *prev;
} t_token;

char *get_input(void);
char *trim_whitespace(char *str);