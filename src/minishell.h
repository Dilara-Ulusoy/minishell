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
int handle_env_variable(const char *line, int *i, char *result, int *res_index);
int get_env_var_length(const char *line);
char *extract_env_var_name(const char *line, int *index);


/* Word Parsing */
char *read_word_range(const char *line, int *index, int length);
char *handle_quotes(const char *line, int *i, char quote);
char *allocate_word(const char *line, int start, int length);


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

// Builtin functions
int ft_cd(char *new_path);
void ft_pwd();
void ft_echo(char *str);
int execute_pwd(char *cmd);

#endif