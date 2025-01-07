#include "minishell.h"

/*****************************************************************************/
/*                           IMPLEMENTATION                                  */
/*****************************************************************************/

static int is_operator(t_token_type type)
{
    return (type == TOKEN_AND || type == TOKEN_OR || type == TOKEN_PIPE ||
            type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT ||
            type == TOKEN_REDIR_APPEND || type == TOKEN_REDIR_HERE);
}

static int is_redirection(t_token_type type)
{
    return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT ||
            type == TOKEN_REDIR_APPEND || type == TOKEN_REDIR_HERE);
}

static void set_syntax_error(t_parser *parser, const char *token_value)
{
    parser->error_status = PARSE_SYNTAX_ERROR;
    printf("Syntax error near unexpected token '%s'\n", token_value ? token_value : "newline");
}


void check_syntax_errors(t_parser *parser)
{
    t_token *current = parser->tokens;

	if (!parser || !parser->tokens) /* Early return if no tokens */
        return;

    /* Check if the first token is invalid */
    if (is_operator(current->type))
    {
        set_syntax_error(parser, current->value);
        return;
    }
   while (current && current->next)
{
    if ((is_operator(current->type) && is_operator(current->next->type)) ||
        (is_redirection(current->type) && (!current->next || current->next->type != TOKEN_WORD)))
    {
        set_syntax_error(parser, current->next ? current->next->value : "newline");
        return;
    }
    current = current->next;
}
    if (current && is_operator(current->type))     /* Check if the last token is an invalid operator */
        set_syntax_error(parser, current->value);
}

/**
 * @brief build_ast
 * Initializes a parser with the token list and calls parse_expression(0).
 * If leftover tokens or errors are found, sets parser.error_status accordingly.
 */
t_ast_node *build_ast(t_token *token_list)
{
    t_parser parser;

    parser.tokens = token_list;
    parser.current_token = token_list;
    parser.error_status = PARSE_OK;
	check_syntax_errors(&parser);
	if (parser.error_status == PARSE_SYNTAX_ERROR)
        return NULL;
    /* parse the whole line as an expression of min precedence = 0 */
    t_ast_node *root = parse_expression(&parser, 0);
    /* check if we used all tokens or if there's an error */
    if (parser.error_status == PARSE_OK && parser.current_token != NULL)
    {
        /* leftover tokens => syntax error */
        parser.error_status = PARSE_SYNTAX_ERROR;
    }
    if (parser.error_status != PARSE_OK)
    {
        /* free partial AST and return NULL */
        free_ast(root);
        return NULL;
    }
    return root;
}

/*****************************************************************************/
/*                             PARSE EXPRESSION                               */
/*****************************************************************************/
/*
    parse_expression(min_prec):
    ---------------------------
    This function is part of a **recursive-descent** parser that constructs
    an AST (Abstract Syntax Tree) by looking at the current tokens and deciding
    how to group commands with operators (like &&, ||, |).

    BASIC IDEA:
      1) We first parse a single "term" (could be a command or a parenthesized expression).
      2) Then, as long as we see a **binary operator** (e.g., &&, ||, |) whose
         **precedence** is greater or equal to 'min_prec', we:
         - consume (advance past) that operator,
         - parse the **right** side with a higher minimum precedence (op_prec + 1),
         - combine the **left** and **right** with the operator into a new node.

    EXAMPLE (Beginner-Friendly):
    ----------------------------
    Let's pretend we have:
      - term() that parses either '(' expression ')' or a simple command (like "ls -l").
      - expression(min_prec) that:
         * calls term() to get a left-hand side node,
         * while we see an operator with precedence >= min_prec, we parse the right side
           with expression(op_prec + 1), and build an operator node.

    Example Input:   ls && echo done || echo fallback
    Precedence:
        - '||' -> 10
        - '&&' -> 20
      (A higher number means higher precedence.)

    1) expression(0) starts:
       - parse_term => let's say we get "ls" (an AST_COMMAND node).
       - Current token is '&&' with precedence 20. Since 20 >= 0, we handle it.
         * We consume '&&'.
         * We call expression(20 + 1 = 21) for the right side.
             ~ In that sub-call, parse_term => "echo done" (a command node).
             ~ Next token might be '||' with precedence 10, but 10 < 21,
               so we don't handle it here. We return "echo done" from that sub-call.
         * We combine "ls" (left) and "echo done" (right) into an AND node => left_node.

       - We return to the while loop in the first expression(0):
         * Current token is now '||' with precedence 10. 10 >= 0 => handle it.
         * Consume '||'.
         * parse expression(10 + 1 = 11) => parse_term => "echo fallback".
           No more tokens of precedence >= 11, so we return "echo fallback".
         * Combine (ls && echo done) with (echo fallback) into an OR node.

    2) Final AST might look like:
                (OR)
             /  	  \
    	(AND)  	    (COMMAND: "echo fallback")
          /   	  \
    (COMMAND: "ls")  (COMMAND: "echo done")

    CODE LOGIC (Simplified):
      while current_token is an operator with precedence >= min_prec:
         operator_type = current_token->type
         consume_token
         right_node = expression(precedence_of(operator_type) + 1)
         left_node = new_operator_node(operator_type, left_node, right_node)
      return left_node

    This allows us to handle **nesting** of operators and ensure that
    higher-precedence operators (like &&) group their operands before
    lower-precedence ones (like ||). By adjusting the 'min_prec' parameter,
    we effectively enforce the correct grouping rules.

*/
t_ast_node *parse_expression(t_parser *p, int min_prec)
{
    /* If no current token or parse error is set, we can't proceed. */
    if (!p->current_token || p->error_status != PARSE_OK)
        return NULL;

    /*
       1) Parse one "term" (could be parenthesized expression or a command).
          This will be our 'left' side of the expression.
    */
    t_ast_node *left_node = parse_term(p);
    if (!left_node)
        return NULL; /* parse_term might set an error */

    /*
       2) As long as the current token is a binary operator with enough precedence,
          we handle it. We'll repeatedly combine 'left_node' with a newly parsed
          'right_node' to build a tree from left to right.
    */
    while (p->current_token && is_binary_operator(p->current_token->type))
    {
        int op_prec = get_precedence(p->current_token->type);
        /* If operator's precedence is less than min_prec, we stop. */
        if (op_prec < min_prec)
            break;

        /* Remember the operator type (e.g., TOKEN_AND, TOKEN_OR, etc.). */
        t_token_type operator_type = p->current_token->type;
        advance_token(p); /* consume the operator token */

        /*
           3) Parse the right side of the expression with a higher precedence
              threshold (op_prec + 1). This ensures that equally high or higher
              precedence operators on the right get parsed in this sub-call.
        */
        int next_prec = op_prec + 1;
        t_ast_node *right_node = parse_expression(p, next_prec);
        if (!right_node)
        {
            /* If we fail to parse the right side, free left_node and bail out. */
            free_ast(left_node);
            return NULL;
        }

        /* 4) Create an operator node to combine left_node and right_node. */
        t_ast_node_type ast_type;
        if (operator_type == TOKEN_AND)
            ast_type = AST_AND;
        else if (operator_type == TOKEN_OR)
            ast_type = AST_OR;
        else if (operator_type == TOKEN_PIPE)
            ast_type = AST_PIPE;
        else
        {
            /* Unexpected operator => syntax error. */
            p->error_status = PARSE_SYNTAX_ERROR;
            free_ast(left_node);
            free_ast(right_node);
            return NULL;
        }

        t_ast_node *op_node = create_ast_operator_node(ast_type, left_node, right_node);
        if (!op_node)
        {
            p->error_status = PARSE_MEMORY_ERROR;
            free_ast(left_node);
            free_ast(right_node);
            return NULL;
        }

        /*
           5) The newly created operator node becomes our 'left_node' for
              subsequent iterations. If there's another operator of
              precedence >= min_prec, we'll combine again.
        */
        left_node = op_node;
    }

    /* 6) Once we see an operator with lower precedence or no more tokens, we stop. */
    return left_node;
}

/*****************************************************************************/
/*                                parse_term                                 */
/*****************************************************************************/
/*
    parse_term():
    -------------
    A "term" is the smallest self-contained unit at the current precedence level.
    In most shells, a term can be:
       1) A **parenthesized** expression, e.g. "(echo hi || echo fail)"
       2) A **command** (like "ls -l", or "echo done").

    HOW IT WORKS:
      - If the current token is '(', we:
         1) consume '(',
         2) parse an entire sub-expression (with min_prec = 0),
         3) expect a closing ')' token,
         4) return that sub-expression node as the result of parse_term.
      - Otherwise, we parse a "command", which might contain multiple words
        and possibly redirections.

    WHY DO THIS?
      - Parentheses allow users to group commands and operators.
        For instance: (echo hello || echo fail) && ls
        means "echo hello || echo fail" is treated as a single grouped expression.
      - If we didn't handle parentheses specially, we couldn't override normal
        precedence rules in the shell grammar.

    SIMPLE EXAMPLE:
      Suppose the user typed: "(ls && echo done) || cat"
      - parse_term sees '(' => calls parse_expression(0)
        * Inside parse_expression, we parse "ls && echo done" => AND node
      - We expect a closing ')' => we consume it
      - That sub-expression is now one single node (the AND node with left=ls, right=echo done)
      - parse_term returns that node.
      - Then, in parse_expression, we see '||' and proceed to parse the rest ("cat").

    CODE FLOW:
      1) If we see '(' => parse sub-expression => expect ')' => return sub-expr
      2) Else => parse_command
*/
t_ast_node *parse_term(t_parser *p)
{
    /* If there's no token or an error is already flagged, we cannot proceed. */
    if (!p->current_token || p->error_status != PARSE_OK)
        return NULL;

    /* CASE 1: Parenthesized expression */
    if (p->current_token->type == TOKEN_PAREN_OPEN)
    {
        /* consume '(' */
        advance_token(p);

        /* parse an expression with min_prec=0 (like a fresh expression) */
        t_ast_node *sub_expr = parse_expression(p, 0);
        if (!sub_expr)
            return NULL; /* sub-expression parse failed */

        /*
           Now we expect a closing ')' token. Why?
           ---------------------------------------
           We started this 'term' by seeing a '(' token and calling
           parse_expression(p, 0) to parse everything inside the parentheses.
           That sub-expression should be completely enclosed, so a matching ')'
           must appear to indicate the end of that sub-expression.

           In typical shell or language syntax, if '(' is opened, the user
           must close it with ')'. If it's missing (the user forgot to type it),
           we have an unmatched parenthesis => syntax error.

           For example, if the input was:
               (ls && echo done
           and never ended with ')',
           parse_expression would parse "ls && echo done" successfully,
           but now we must see a ')' token to confirm the parentheses are closed.
           If it's not here, we flag a syntax error because the user
           effectively wrote an incomplete (unbalanced) expression.
        */
        if (!p->current_token || p->current_token->type != TOKEN_PAREN_CLOSE)
        {
            p->error_status = PARSE_SYNTAX_ERROR;
            free_ast(sub_expr);
            return NULL;
        }

        /* consume the ')' token */
        advance_token(p);

        /* The entire parenthesized expression is one "term." */
        return sub_expr;
    }
    else
    {
        /* CASE 2: If not '(', we parse a command (like "echo hello" or "ls -l") */
        return parse_command(p);
    }
}
/*****************************************************************************/
/*                              parse_command                                 */
/*****************************************************************************/
/*
    parse_command():
    ---------------
    This function parses a simple command node. A "command" in this context
    is one or more WORD tokens (like "echo", "hello") that get combined into
    a single string, plus any optional redirections (<, >, >>, <<) that follow.

    STEP-BY-STEP (Beginner-Friendly):
      1) If there's a WORD token, we capture it (and possibly more WORD tokens)
         into one command string (e.g., "echo hello world").
      2) After we finish reading WORD tokens, we look for redirection tokens
         like TOKEN_REDIR_IN (<), TOKEN_REDIR_OUT (>), etc.
      3) For each redirection, we consume the operator token, then expect a
         WORD token for the filename. We store these in a simple linked list
         (io_redirects).
      4) Finally, we create and return a command node (AST_COMMAND) with
         cmd_args = the combined command string, and io_redirects = the
         redirection list. If we have no words at all but we do have
         redirections, we can still form a command node (though a real shell
         might treat that as an error or do something special).

    EXAMPLE:
      Tokens: [ WORD("echo"), WORD("hello"), TOKEN_REDIR_OUT(">"),
                WORD("file.txt") ]
      - Combine "echo" + "hello" => "echo hello" for cmd_args
      - Redirection: > file.txt => store in io_redirects => IO_OUT("file.txt")
      - Build a node => AST_COMMAND with:
           node->cmd_args = "echo hello"
           node->io_redirects => [IO_OUT("file.txt")]
      - Return that node.

    IMPORTANT:
      - In a real shell, you might handle special cases like 'redirection
        with no command' differently. For simplicity, we allow an empty
        command string if no WORD tokens appear but do parse redirections.
      - If memory fails, we set p->error_status = PARSE_MEMORY_ERROR.
      - If we find a redirection operator but no WORD after it for the filename,
        we set p->error_status = PARSE_SYNTAX_ERROR.

    CODE FLOW (Simplified):
      1) Gather all WORD tokens into a single string 'cmd_args'.
      2) parse_redirections => attach them to a linked list for io_redirects
      3) create an AST_COMMAND node and return it.
*/

t_ast_node *parse_command(t_parser *p)
{
    if (!p->current_token || p->error_status != PARSE_OK)
        return NULL;

    /* 1) BUILD A COMMAND STRING FROM CONSECUTIVE WORD TOKENS. */
    char *cmd_args = NULL;  /* We'll build this dynamically. */
    {
        /* As long as we see WORD tokens, we append them with a space in between. */
        char buffer[256];         /* A simple buffer for demonstration. */
        int  buf_pos = 0;         /* Current position in buffer. */

        buffer[0] = '\0';         /* Start empty. */

        /* Read while the next token is a WORD. */
        while (p->current_token && p->current_token->type == TOKEN_WORD)
        {
            const char *word_value = p->current_token->value;
            int word_len = (int)ft_strlen(word_value);

            /* If buffer + word won't fit, we do a simple check (not robust). */
            /* In a real shell, you'd do dynamic resizing or a similar approach. */
            if (buf_pos + word_len + 2 >= (int)sizeof(buffer))
            {
                /* Buffer too small => for demonstration, set memory error. */
				// TO DO......TO DO.........TO DO.........TO DO.........
                p->error_status = PARSE_MEMORY_ERROR;
                return NULL;
            }

            /* Add a space if not the first word. */
            if (buf_pos > 0)
            {
                buffer[buf_pos] = ' ';
                buf_pos++;
            }

            /* Copy the word into the buffer. */
            ft_memcpy(&buffer[buf_pos], word_value, word_len);
            buf_pos += word_len;
            buffer[buf_pos] = '\0';

            /* Advance to next token. */
            advance_token(p);
        }

        /* If we read zero words, buffer is still "". That's okay for now. */
        /* We'll create a copy of buffer as cmd_args. */
        cmd_args = strdup(buffer);
        if (!cmd_args)
        {
            p->error_status = PARSE_MEMORY_ERROR;
            return NULL;
        }
    }

    /* 2) PARSE REDIRECTIONS (like <, >, >>, <<). */
    t_io_node *io_list = NULL;
    parse_redirections(p, &io_list);
    /* parse_redirections will attach any valid redirection tokens
       and update p->error_status if there's an error. */

    if (p->error_status != PARSE_OK)
    {
        free(cmd_args);
        free_io_list(io_list);
        return NULL;
    }

    /* 3) CREATE THE AST COMMAND NODE. */
    t_ast_node *cmd_node = create_ast_command_node(cmd_args, io_list);
    free(cmd_args); /* We can free our local copy after building the node. */

    if (!cmd_node)
    {
        /* Memory error building the command node. */
        free_io_list(io_list);
        p->error_status = PARSE_MEMORY_ERROR;
        return NULL;
    }

    return cmd_node;
}


/*****************************************************************************/
/*                        parse_redirections (for reference)                 */
/*****************************************************************************/
/*
   parse_redirections:
   - While the current token is one of TOKEN_REDIR_IN (<), TOKEN_REDIR_OUT (>),
     TOKEN_REDIR_APPEND (>>), TOKEN_REDIR_HERE (<<),
     we consume it, map it to an i/o type, then expect the next token to be WORD
     for the filename/limit string.
   - We add these redirection nodes to the provided 'io_list' pointer.

   Example:
     Tokens: [ TOKEN_REDIR_OUT(">"), WORD("output.txt"), TOKEN_REDIR_IN("<"), WORD("input.txt"), ... ]
     We'll attach IO_OUT("output.txt") then IO_IN("input.txt") to io_list in order.

   In a real shell, you might handle more sophisticated cases or expansions.
   For simplicity, we just handle <filename> as is.
*/

/*****************************************************************************/
/*             create_ast_command_node & free_io_list (for reference)        */
/*****************************************************************************/
/*
   create_ast_command_node(cmd_args, io_list):
   - Allocates an AST node of type AST_COMMAND.
   - Duplicates 'cmd_args' if non-empty, assigns 'io_list' to node->io_redirects.
   - left/right remain NULL because a command node doesn't have children.

   free_io_list:
   - Iterate over the t_io_node linked list, free each filename, and free the node.
*/

/*
    CONCLUSION:
    The parse_command function is simplified:
    - Collect words in a small local buffer (or a dynamic approach in real code).
    - parse_redirections handles <, >, >>, << and updates the i/o list.
    - We build an AST_COMMAND node with the final strings.

    For advanced shells, you'd handle expansions, environment variables,
    aliasing, etc. But this is enough to demonstrate a beginner-level approach
    to building a command node from tokens.
*/


/*****************************************************************************/
/*                            parse_redirections                             */
/*****************************************************************************/
/*
   parse_redirections:
   - While the current token is one of TOKEN_REDIR_IN (<),
     TOKEN_REDIR_OUT (>), TOKEN_REDIR_APPEND (>>), TOKEN_REDIR_HERE (<<),
     consume that token => map it to IO_IN, IO_OUT, IO_APPEND, IO_HEREDOC.
   - Then expect a WORD token for the filename (or limit string).
   - Attach it to the io_list.
*/
void parse_redirections(t_parser *p, t_io_node **io_list)
{
    while (p->current_token &&
           (p->current_token->type == TOKEN_REDIR_IN ||
            p->current_token->type == TOKEN_REDIR_OUT ||
            p->current_token->type == TOKEN_REDIR_APPEND ||
            p->current_token->type == TOKEN_REDIR_HERE))
    {
        /* map token -> io type */
        t_io_type kind = map_token_to_io_type(p->current_token->type);

        /* consume the redirection token */
        advance_token(p);

        /* now we expect a WORD for the filename (or here-doc delimiter) */
        if (!p->current_token || p->current_token->type != TOKEN_WORD)
        {
            /* syntax error: e.g., `>` with no filename */
            p->error_status = PARSE_SYNTAX_ERROR;
            return;
        }

        /* create a new io_node */
        t_io_node *new_io = (t_io_node *)malloc(sizeof(t_io_node));
        if (!new_io)
        {
            p->error_status = PARSE_MEMORY_ERROR;
            return;
        }
        new_io->io_kind = kind;
        /* duplicate the filename from the token's value */
        new_io->filename = strdup(p->current_token->value);
		new_io->next = NULL;

        if (!new_io->filename)
        {
            free(new_io);
            p->error_status = PARSE_MEMORY_ERROR;
            return;
        }
        /* consume the WORD token */
        advance_token(p);

        /* attach new_io to the io_list */
        if (!*io_list)
        {
            /* first in the list */
            *io_list = new_io;
        }
        else
        {
            /* append at the end */
            t_io_node *tmp = *io_list;
            while (tmp->next)
                tmp = tmp->next;
            tmp->next = new_io;
        }
    }
}


/*****************************************************************************/
/*                         AST NODE CREATION HELPERS                         */
/*****************************************************************************/

/*
   create_ast_command_node:
   - Allocates an AST_COMMAND node
   - cmd_text is duplicated into node->cmd_args
   - io_list is attached
*/
t_ast_node *create_ast_command_node(const char *cmd_text, t_io_node *io_list)
{
    t_ast_node *node = (t_ast_node *)malloc(sizeof(t_ast_node));
    if (!node)
        return NULL;

    node->node_type = AST_COMMAND;
    if (cmd_text && *cmd_text)
    {
        node->cmd_args = strdup(cmd_text);
        if (!node->cmd_args)
        {
            free(node);
            return NULL;
        }
    }
    else
    {
        /* no command text => empty string or possible "commandless" redirection */
        node->cmd_args = NULL;
    }

    node->io_redirects = io_list;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/*
   create_ast_operator_node:
   - For operators like &&, ||, |
   - left and right subtrees are assigned
   - cmd_args = NULL, io_redirects = NULL
*/
t_ast_node *create_ast_operator_node(t_ast_node_type type, t_ast_node *left, t_ast_node *right)
{
    t_ast_node *node = (t_ast_node *)malloc(sizeof(t_ast_node));
    if (!node)
        return NULL;

    node->node_type = type;
    node->cmd_args = NULL;
    node->io_redirects = NULL;
    node->left = left;
    node->right = right;
    return node;
}

/*****************************************************************************/
/*                         FREE THE AST RECURSIVELY                          */
/*****************************************************************************/
void free_ast(t_ast_node *root)
{
    if (!root)
        return;

    /* free left and right subtrees */
    free_ast(root->left);
    free_ast(root->right);

    /* free command text if command node */
    if (root->cmd_args)
        free(root->cmd_args);

    /* free i/o redirection list */
    if (root->io_redirects)
    {
        t_io_node *curr = root->io_redirects;
        while (curr)
        {
            t_io_node *next = curr->next;
            if (curr->filename)
                free(curr->filename);
            free(curr);
            curr = next;
        }
    }

    /* finally free the node itself */
    free(root);
}

/*****************************************************************************/
/*                          UTILITY FUNCTIONS                                */
/*****************************************************************************/

/*
   advance_token:
   - moves parser->current_token to the next in the list
*/
void advance_token(t_parser *p)
{
    if (p->current_token)
        p->current_token = p->current_token->next;
}

/*
   is_binary_operator:
   - returns 1 if it's &&, ||, or |,
   - 0 otherwise
*/
int is_binary_operator(t_token_type ttype)
{
    if (ttype == TOKEN_AND || ttype == TOKEN_OR || ttype == TOKEN_PIPE)
        return 1;
    return 0;
}

/*
   get_precedence:
   - a simple numeric precedence system
     OR (||) -> 10
     AND (&&) -> 20
     PIPE (|) -> 30
   - higher => parse first
*/
int get_precedence(t_token_type ttype)
{
    switch (ttype)
    {
    case TOKEN_OR:
        return 10;
    case TOKEN_AND:
        return 20;
    case TOKEN_PIPE:
        return 30;
    default:
        return -1;
    }
}

/*
   map_token_to_io_type:
   - Convert a token type for redirection (<, >, >>, <<)
     into an i/o type (IO_IN, IO_OUT, IO_APPEND, IO_HEREDOC).
*/
t_io_type map_token_to_io_type(t_token_type ttype)
{
    switch (ttype)
    {
    case TOKEN_REDIR_IN:     return IO_IN;       /* <  */
    case TOKEN_REDIR_OUT:    return IO_OUT;      /* >  */
    case TOKEN_REDIR_APPEND: return IO_APPEND;   /* >> */
    case TOKEN_REDIR_HERE:   return IO_HEREDOC;  /* << */
    default:                 return IO_IN;       /* fallback, shouldn't happen */
    }
}

/*****************************************************************************/
/*                         HOW TO USE (EXAMPLE)                              */
/*****************************************************************************/
/*
    Example Usage in main (pseudo-code):
    --------------------------------------------------------------
    int main(void)
    {
        // 1) Tokenize input
        // e.g. user input: "cat < input.txt && echo done >> out.txt"
        t_token *tokens = tokenize("cat < input.txt && echo done >> out.txt");

        // 2) Build AST
        t_ast_node *ast_root = build_ast(tokens);
        if (!ast_root)
        {
            // handle syntax or memory error
            printf("Parsing failed.\n");
        }
        else
        {
            // We have a valid AST
            // e.g., interpret/execute it
            // For demonstration, we do nothing special
        }

        // 3) Cleanup:
        //    - free token list
        //    - free AST
        return 0;
    }
    --------------------------------------------------------------

    This approach shows how to handle redirections by attaching them
    to command nodes. For advanced shells, you'd refine how redirections
    are interpreted and maybe handle 'commandless' redirections differently.
*/

void free_io_list(t_io_node *io_list)
{
    while (io_list)
    {
        t_io_node *temp = io_list->next;

        /*
           Each t_io_node may have a dynamically allocated 'filename'.
           We must free it.
        */
        if (io_list->filename)
            free(io_list->filename);

        /*
           Finally, free the I/O node structure itself.
        */
        free(io_list);

        /*
           Move on to the next node in the list.
        */
        io_list = temp;
    }
}
