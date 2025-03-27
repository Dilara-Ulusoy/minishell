/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_expression.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoc@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 10:49:26 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/03/27 16:40:25 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	t_ast_node	*parse_right_node(t_parser *p, int op_prec)
{
	t_ast_node	*right_node;
	int			next_prec;

	next_prec = op_prec + 1;
	right_node = parse_expression(p, next_prec);
	if (!right_node)
	{
		handle_parse_error(p, NULL, NULL, "Expected expression after operator");
		return (NULL);
	}
	return (right_node);
}

/*
	parse_term():
	-------------
	A "term" is the smallest self-contained unit at the
	current precedence level.
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
	  - That sub-expression is now one single node
	  (the AND node with left=ls, right=echo done)
	  - parse_term returns that node.
	  - Then, in parse_expression,
	  we see '||' and proceed to parse the rest ("cat").

	CODE FLOW:
	  1) If we see '(' => parse sub-expression => expect ')' => return sub-expr
	  2) Else => parse_command
*/
t_ast_node	*parse_term(t_parser *p)
{
	if (!p->current_token || p->error_status != PARSE_OK)
		return (NULL);
	else
		return (parse_command(p));
}

t_ast_node	*built_operator_node(t_ast_node *left_node, t_ast_node *right_node,
		t_token_type operator_type, t_parser *p)
{
	t_ast_node_type	ast_type;
	t_ast_node		*op_node;

	ast_type = get_ast_node_type(operator_type);
	if (ast_type == AST_INVALID)
	{
		return (handle_parse_error(p, left_node,
				right_node, "unexpected operator"));
	}
	op_node = create_ast_operator_node(ast_type, left_node, right_node);
	if (!op_node)
	{
		p->error_status = PARSE_MEMORY_ERROR;
		return (handle_parse_error(p, left_node,
				right_node, "memory allocation failed"));
	}
	return (op_node);
}

/*
📌 parse_expression(min_prec)
---------------------------
This function is part of a **recursive-descent parser** that constructs an
AST (Abstract Syntax Tree) by analyzing tokens and determining how to group
commands with **binary operators** (`|`, `&&`, `||`).

--Basic Idea:---
1) **Parse a single command** (or a parenthesized expression).
2) While encountering a binary operator (`|`, `&&`, `||`) whose precedence
   is **≥ min_prec**, we:
   - Consume that operator.
   - Recursively parse the right-hand side with a
   higher precedence (`op_prec + 1`).
   - Combine the left and right into an AST node representing the operation.

---Example:---
ls | grep .c | wc -l

| → 5 (higher precedence operator than '&&' or '||' )

1) First Call (`expression(0)`)
   - `parse_term()` processes `ls` → AST_COMMAND("ls").
   - Current token is `|` with precedence 5 ≥ 0, so we handle it.
   - We consume `|` and call `expression(6)` for the right side.

2) Recursive Call (`expression(6)`)
   - `parse_term()` processes `grep .c` → AST_COMMAND("grep .c").
   - Next token is `|` with precedence 5 ≥ 6, but 5 is not ≥ 6, so we stop.
   - Returns AST_COMMAND("grep .c").

3) Combining Nodes
   - AST_PIPE("ls", "grep .c") is created.

4) Return to First Call
   - Current token is `|` again with precedence 5 ≥ 0, so we handle it.
   - We consume `|` and call `expression(6)` for the right side.

5) Final Recursive Call (`expression(6)`)
   - `parse_term()` processes `wc -l` → AST_COMMAND("wc -l").
   - No more operators → Return AST_COMMAND("wc -l").

6) Final Combination
   - AST_PIPE(PIPE("ls", "grep .c"), "wc -l") is created.

Final AST:

         AST_PIPE
        /       \
     AST_PIPE    AST_COMMAND("wc -l")
    /    \
AST_COMMAND("ls") AST_COMMAND("grep .c")
*/
t_ast_node	*parse_expression(t_parser *p, int min_prec)
{
	t_token_type	operator_type;
	t_ast_node		*left;
	t_ast_node		*right;
	int				op_prec;

	if (!p->current_token || p->error_status != PARSE_OK)
		return (NULL);
	left = parse_term(p);
	if (!left)
		return (NULL);
	while (p->current_token && p->current_token->type == TOKEN_PIPE)
	{
		op_prec = get_precedence(p->current_token->type);
		if (op_prec < min_prec)
			break ;
		operator_type = p->current_token->type;
		get_next_token(p);
		right = parse_right_node(p, op_prec);
		if (!right)
			return (NULL);
		left = built_operator_node(left, right, operator_type, p);
		if (!left)
			return (NULL);
	}
	return (left);
}
