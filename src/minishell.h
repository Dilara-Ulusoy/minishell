#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <dirent.h>
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

/* Input Handling and initialization */
char *get_input(const char *prompt);
void init_shell(t_shell *shell);


/* Environment variable expansion */
char  *expand_env_var(const char *line, int *index);
void   handle_env_var(const char *line, int *i, char *result, int *res_index);

/* Word Parsing */
char *read_word_range(const char *line, int *index);
char *handle_quotes(const char *line, int *i, char quote);
char *allocate_word(const char *line, int start, int length);


/* Whitespace handling */
int         skip_whitespace(const char *line, int i);
int         is_space(char c);

/*****************************************************************************/
/*                        PARSE STEPS                                        */
/*****************************************************************************/

/* Step 2: Parse two-char operator */
int  parse_two_char_operator(t_token **head, const char *line, int *pos);

/* Step 3: Parse single-char operator */
int  parse_single_char_operator(t_token **head, const char *line, int *pos);

/* Step 4: Parse word */
int  parse_word(t_token **head, const char *line, int *pos);
int  handle_newline(t_token **head, const char *line, int *pos);


// Cleanup
void cleanup_shell(t_shell *shell);


#endif