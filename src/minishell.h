#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "parsing.h"
#include "token.h"
#include "../Libft/libft.h"


typedef struct s_shell
{
	char *line;
	t_token *tokens;
	t_token *current_token;
	t_ast_node *ast;
	t_parser *parser;
	int exit_code;

} t_shell;


/*****************************************************************************/
/*                         FUNCTION DECLARATIONS                             */
/*****************************************************************************/

/* The main function to convert a user input line into a linked list of tokens. */
t_token *tokenize(const char *line);

/* Two-char operator checks */
int         is_two_char_operator(char c);
t_token_type match_two_char_operator(const char *line, int index);

/* Single-char operator checks */
t_token_type match_single_char_operator(char c);

/* Creating / appending tokens */
t_token     *create_new_token_range(t_token_type type,
                                    const char *line,
                                    int startIndex,
                                    int length);
void        append_token(t_token **head, t_token *new_token);

/* Whitespace handling */
int         skip_whitespace(const char *line, int i);
int         is_space(char c);


/* Read a non-operator word */
char *read_word_range(const char *line, int *index);
int handle_quotes(const char *line, int *index, char quote);

char *allocate_word(const char *line, int start, int length);

/*****************************************************************************/
/*                        PARSE STEPS (LOGICALLY SPLIT)                      */
/*****************************************************************************/

/* Step 2: parse two-char operator */
int  parse_two_char_operator(t_token **head, const char *line, int *pos);
/* Step 3: parse single-char operator */
int  parse_single_char_operator(t_token **head, const char *line, int *pos);
/* Step 4: parse word */
int  parse_word(t_token **head, const char *line, int *pos);
int handle_newline(t_token **head, const char *line, int *pos);

void print_tokens(const t_token *head);


void free_tokens(t_token **head);

char *get_input(const char *prompt);

#endif