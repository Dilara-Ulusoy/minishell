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
#include <errno.h>
#include <signal.h>
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
	int line_length;

} t_shell;

/*****************************************************************************/
/*                         FUNCTION DECLARATIONS                             */
/*****************************************************************************/

/* Input Handling and initialization */
char *get_input(const char *prompt);
void init_shell(t_shell *shell);


/* Environment variable expansion */
char *get_env_var_value(const char *line, int *index);
char *handle_env_variable_without_space(const char *line, int *index, int start);
char *handle_dollar_sign(const char *line, int *index, int start);
void *ft_realloc(void *ptr, size_t size);
char *join_string_with_quoted_if_no_space(const char *line, int *index, int start);
char *parse_quotes(const char *line, int *index);
void *free_this(char *s1, char *s2, char *s3, char *message);


/* Word Parsing */
char *read_word_range(const char *line, int *index, int length);


/* Whitespace handling */
int         skip_whitespace(const char *line, int i);
int         is_space(char c);

/*****************************************************************************/
/*                        PARSE STEPS                                        */
/*****************************************************************************/

/* Step 2: Parse two-char operator */
int parse_two_char_operator(t_token **head, const char *line, int *pos, int length);

/* Step 3: Parse single-char operator */
int parse_single_char_operator(t_token **head, const char *line, int *pos, int length);

/* Step 4: Parse word */
int parse_word(t_token **head, const char *line, int *pos, int length);
int  handle_newline(t_token **head, const char *line, int *pos);


// Cleanup
void cleanup_shell(t_shell *shell);


#endif