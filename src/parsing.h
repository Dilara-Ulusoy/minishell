
#include "minishell.h"
#include "token.h"

#ifndef PARSING_H
# define PARSING_H

/*
    e_ast_node_type: Types of AST nodes we handle:
      - AST_COMMAND: a node that represents a command + arguments + redirections
      - AST_AND:     represents `&&`
      - AST_OR:      represents `||`
      - AST_PIPE:    represents `|`
*/
typedef enum e_ast_node_type
{
    AST_COMMAND,
    AST_AND,
    AST_OR,
    AST_PIPE,
	AST_INVALID,
} t_ast_node_type;

/*
    e_io_type: For classifying redirection tokens:
      - IO_IN  : `<`
      - IO_OUT : `>`
      - IO_APPEND : `>>`
      - IO_HEREDOC: `<<`
*/
typedef enum e_io_type
{
    IO_IN,       /* <  */
    IO_OUT,      /* >  */
    IO_APPEND,   /* >> */
    IO_HEREDOC   /* << */
} t_io_type;

/*
    s_io_node: linked list for storing redirection info in a command node.
      - io_kind:  e.g., IO_IN, IO_OUT, etc.
      - filename: typically the word after `<`, `>`, etc.
*/
typedef struct s_io_node
{
    t_io_type          io_kind;
    char              *filename;
    struct s_io_node  *next;
} t_io_node;

/*
    s_ast_node: an AST node can be:
      - AST_COMMAND (with cmd_args and io_redirects set)
      - AST_AND / AST_OR / AST_PIPE (operator nodes with left/right children)
*/
typedef struct s_ast_node
{
    t_ast_node_type       node_type;     /* e.g., AST_COMMAND, AST_AND, AST_OR */

    /* If it's a command node: */
    char                 *cmd_args;      /* combined string e.g. "echo hello" */
    t_io_node            *io_redirects;  /* linked list of redirections */

    /* If it's an operator node (AND, OR, PIPE): */
    struct s_ast_node    *left;          /* left subtree */
    struct s_ast_node    *right;         /* right subtree */
} t_ast_node;

/*****************************************************************************/
/*              PARSER STATE (IF YOU WANT TO TRACK TOKENS GLOBALLY)          */
/*****************************************************************************/
typedef enum e_parse_error
{
    PARSE_OK,
    PARSE_SYNTAX_ERROR,
    PARSE_MEMORY_ERROR
} e_parse_error;

/*
    t_parser: holds the tokens, a pointer to the current token, and error status.
*/
typedef struct s_parser
{
    t_token         *tokens;        /* head of the token list */
    t_token         *current_token; /* pointer to the token being parsed now */
    e_parse_error    error_status;  /* ok, syntax error, memory error, etc. */
} t_parser;


/*****************************************************************************/
/*             FUNCTION DECLARATIONS FOR AST BUILDING / REDIRECTIONS         */
/*****************************************************************************/

/* The main entry: build_ast. Takes a token list => returns an AST root. */
t_ast_node    *build_ast(t_token *token_list);

/* The core recursive-descent parser functions */
t_ast_node *parse_expression(t_parser *p, int min_prec);
t_ast_node *parse_term(t_parser *p);
t_ast_node *parse_command(t_parser *p);
char *build_command_string(t_parser *p);
void attach_io_node(t_io_node **io_list, t_io_node *new_io);
t_io_node *create_io_node(t_io_type kind, const char *filename, t_parser *p);

/* Redirection parsing: gather <, >, >>, << and attach them to a command. */
void         parse_redirections(t_parser *p, t_io_node **io_list);

/* AST node creation helpers */
t_ast_node  *create_ast_command_node(const char *cmd_text, t_io_node *io_list);
t_ast_node  *create_ast_operator_node(t_ast_node_type type, t_ast_node *left, t_ast_node *right);

/* Utility to free the AST recursively */
void free_ast(t_ast_node *root);

/* Utilities: next token, operator precedence checks, etc. */
void         get_next_token(t_parser *p);
int          get_precedence(t_token_type ttype);
int          is_binary_operator(t_token_type ttype);
t_io_type    map_token_to_io_type(t_token_type ttype);

void free_io_list(t_io_node *io_list);

void check_syntax_errors(t_parser *parser);
t_ast_node_type get_ast_node_type(t_token_type operator_type);
void *handle_parse_error(t_parser *p, t_ast_node *left_node, t_ast_node *right_node, const char *error_message);
char *handle_allocation_error(t_parser *parser, char *var_name, const char *error_message);

t_ast_node *built_operator_node(t_ast_node *left_node, t_ast_node *right_node, t_token_type operator_type, t_parser *p);


int is_operator(t_token_type type);
int is_redirection(t_token_type type);
void set_syntax_error(t_parser *parser, const char *token_value);
void check_syntax_errors(t_parser *parser);



#endif