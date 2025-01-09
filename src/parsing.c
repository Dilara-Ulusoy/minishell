#include "minishell.h"
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
          If left side consist of only command (eg. ls -l), it will be returned as AST_COMMAND node.
          AST_COMMAND
            ├── Command: ls
            └── Args: -l
          If it is a paranthesized expression (eg. echo test && echo done), it will be returned as sub-expression.
          AST_AND (&&)
            ├── Left: AST_COMMAND (echo test)
            └── Right: AST_COMMAND (echo done)
    */
    t_ast_node *left_node = parse_term(p);
    if (!left_node)
        return NULL;

    /*
       2) As long as the current token is a binary operator with enough precedence,
          we handle it. We'll repeatedly combine 'left_node' with a newly parsed
          'right_node' to build a tree from left to right.
    */
    while (p->current_token && is_binary_operator(p->current_token->type))
    {
        int op_prec = get_precedence(p->current_token->type);
        if (op_prec < min_prec)        /* If operator's precedence is less than min_prec, we stop. */
            break;

        /* Remember the operator type (e.g., TOKEN_AND, TOKEN_OR, etc.). */
        t_token_type operator_type = p->current_token->type;
        get_next_token(p); /* consume the operator token */

        /*
           3) Parse the right side of the expression with a higher precedence
              threshold (op_prec + 1). This ensures that equally high or higher
              precedence operators on the right get parsed in this sub-call.
        */
        int next_prec = op_prec + 1;
        t_ast_node *right_node = parse_expression(p, next_prec);
        if (!right_node)
            handle_parse_error(p, left_node, NULL, "expected expression after operator");
        /* Built an operator AST node */
        left_node = built_operator_node(left_node, right_node, operator_type, p);
        if (!left_node)
            return NULL;
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
	t_ast_node *sub_expr;
	/* If there's no token or an error is already flagged, we cannot proceed. */
    if (!p->current_token || p->error_status != PARSE_OK)
        return NULL;

    /* CASE 1: Parenthesized expression */
    if (p->current_token->type == TOKEN_PAREN_OPEN)
    {
        get_next_token(p);        /* consume '(' */
        /* parse an expression with min_prec=0 (like a fresh expression) */
        sub_expr = parse_expression(p, 0);
        if (!sub_expr)
            return NULL; /* sub-expression parse failed */
        if (!p->current_token || p->current_token->type != TOKEN_PAREN_CLOSE)
        {
            p->error_status = PARSE_SYNTAX_ERROR;
            free_ast(sub_expr);
            return NULL;
        }
        get_next_token(p);         /* consume the ')' token */
        return sub_expr;    /* The entire parenthesized expression is one "term." */
    }
    else
        return parse_command(p);  /* CASE 2: If not '(', we parse a command (like "echo hello" or "ls -l") */
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

   // 1) Build Command String
    char *cmd_args = build_command_string(p);
	//printf("cmd_args: %s\n", cmd_args);
    if (!cmd_args || p->error_status != PARSE_OK)
        return NULL;

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
    return cmd_node; // Return AST_COMMAND node
}


static char *resize_buffer(char *buffer, size_t *buffer_size)
{
    size_t new_size = *buffer_size * 2;
    char *new_buffer = ft_calloc(new_size, 1);
    if (!new_buffer)
    {
        free(buffer);
        return NULL;
    }
    ft_memcpy(new_buffer, buffer, *buffer_size);
    free(buffer);
    *buffer_size = new_size;
    return new_buffer;
}


static int append_to_buffer(char **buffer, size_t *buffer_size, size_t *buf_pos, const char *word_value, size_t word_len)
{
    // Tamponda yeterli alan yoksa genişlet
    while (*buf_pos + word_len + 2 >= *buffer_size)
    {
        *buffer = resize_buffer(*buffer, buffer_size);
        if (!*buffer)
            return 0; // Hata durumu
    }

    // Argümanları boşluk ile ayır
    if (*buf_pos > 0)
        (*buffer)[(*buf_pos)++] = ' ';

    // Yeni kelimeyi tampona kopyala
    ft_memcpy(&(*buffer)[*buf_pos], word_value, word_len);
    *buf_pos += word_len;
    (*buffer)[*buf_pos] = '\0'; // Sonlandırıcı karakter ekle

    return 1; // Başarı
}


char *build_command_string(t_parser *p)
{
    size_t buffer_size = 256;
    size_t buf_pos = 0;
	size_t word_len = 0;
    char *buffer;
	const char *word_value;

	buffer = ft_calloc(buffer_size, 1);
    if (!buffer)
    {
        p->error_status = PARSE_MEMORY_ERROR;
        return NULL;
    }
    while (p->current_token && p->current_token->type == TOKEN_WORD)
    {
        word_value = p->current_token->value;
        word_len = ft_strlen(word_value);

        if (!append_to_buffer(&buffer, &buffer_size, &buf_pos, word_value, word_len))
        {
            p->error_status = PARSE_MEMORY_ERROR;
            return NULL;
        }
        get_next_token(p); // Sonraki token'a geç
    }
    return buffer;
}

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
    while (p->current_token && is_redirection(p->current_token->type))
    {
		t_io_type kind;
		t_io_node *new_io;
		/* map token -> io type */
		kind = map_token_to_io_type(p->current_token->type);
		get_next_token(p); /* consume the redirection token */

        /* now we expect a WORD for the filename (or here-doc delimiter) */
        if (!p->current_token || p->current_token->type != TOKEN_WORD)
        {
            p->error_status = PARSE_SYNTAX_ERROR;
            return;
        }
        new_io = create_io_node(kind, p->current_token->value, p);
        if (!new_io) // Memory error is already set in create_io_node
            return;

        attach_io_node(io_list, new_io);
        get_next_token(p); // Consume the filename/WORD token
    }
}

/*
   map_token_to_io_type:
   - Convert a token type for redirection (<, >, >>, <<)
     into an i/o type (IO_IN, IO_OUT, IO_APPEND, IO_HEREDOC).
*/
t_io_type map_token_to_io_type(t_token_type type)
{
   if(type == TOKEN_REDIR_IN)
		return IO_IN;
	else if(type == TOKEN_REDIR_OUT)
		return IO_OUT;
	else if(type == TOKEN_REDIR_APPEND)
		return IO_APPEND;
	else if(type == TOKEN_REDIR_HERE)
		return IO_HEREDOC;
	else
		return IO_IN;
}

t_ast_node *built_operator_node(t_ast_node *left_node, t_ast_node *right_node, t_token_type operator_type, t_parser *p)
{
    t_ast_node_type ast_type;
	t_ast_node *op_node;

	ast_type = get_ast_node_type(operator_type);
    if (ast_type == AST_INVALID)
    {
        return handle_parse_error(p, left_node, right_node, "unexpected operator");
    }
    op_node = create_ast_operator_node(ast_type, left_node, right_node);
    if (!op_node)
    {
        p->error_status = PARSE_MEMORY_ERROR;
        return handle_parse_error(p, left_node, right_node, "memory allocation failed");
    }
    return op_node;
}
