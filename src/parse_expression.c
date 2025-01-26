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

static	t_ast_node *parse_right_node(t_parser *p, t_ast_node *left_node, int op_prec)
{
	t_ast_node	*right_node;
	int			next_prec;

	next_prec = op_prec + 1;
	right_node = parse_expression(p, next_prec);
	if (!right_node)
	{
		handle_parse_error(p, left_node, NULL, "Expected expression after operator");
		return NULL;
	}
	return right_node;
}

t_ast_node	*parse_expression(t_parser *p, int min_prec)
{
	t_token_type	operator_type;
	t_ast_node		*left_node;
	t_ast_node		*right_node;
	int				op_prec;

	if (!p->current_token || p->error_status != PARSE_OK) /* If no current token or parse error is set, we can't proceed. */
		return NULL;
	left_node = parse_term(p);
	if (!left_node)
		return NULL;
	while (p->current_token && is_binary_operator(p->current_token->type))
	{
		op_prec = get_precedence(p->current_token->type);
		if (op_prec < min_prec) /* If operator's precedence is less than min_prec, we stop. */
			break;
		operator_type = p->current_token->type;				  /* Remember the operator type (e.g., TOKEN_AND, TOKEN_OR, etc.). */
		get_next_token(p);									  /* consume the operator token */
		right_node = parse_right_node(p, left_node, op_prec);
		if (!right_node)
			return NULL;
		left_node = built_operator_node(left_node, right_node, operator_type, p); /* Built an operator AST node */
		if (!left_node)
			return NULL;
	}
	return left_node; /* 6) Once we see an operator with lower precedence or no more tokens, we stop. */
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
t_ast_node	*parse_term(t_parser *p)
{
	t_ast_node *sub_expr;
	/* If there's no token or an error is already flagged, we cannot proceed. */
	if (!p->current_token || p->error_status != PARSE_OK)
		return NULL;

	/* CASE 1: Parenthesized expression */
	if (p->current_token->type == TOKEN_PAREN_OPEN)
	{
		get_next_token(p); /* consume '(' */
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
		get_next_token(p); /* consume the ')' token */
		return sub_expr;   /* The entire parenthesized expression is one "term." */
	}
	else
		return parse_command(p); /* CASE 2: If not '(', we parse a command (like "echo hello" or "ls -l") */
}

// Built operator node function is used to create an operator node with left and right nodes.
t_ast_node	*built_operator_node(t_ast_node *left_node, t_ast_node *right_node, t_token_type operator_type, t_parser *p)
{
	t_ast_node_type	ast_type;
	t_ast_node		*op_node;

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

/*
   get_precedence:
   - a simple numeric precedence system
	 OR (||) -> 10
	 AND (&&) -> 20
	 PIPE (|) -> 30
   - higher => parse first
*/
int	get_precedence(t_token_type type)
{
	if (type == TOKEN_OR)
		return 10;
	else if (type == TOKEN_AND)
		return 20;
	else if (type == TOKEN_PIPE)
		return 30;
	else
		return 0;
}
