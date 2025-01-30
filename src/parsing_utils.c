
#include "minishell.h"

int	is_operator(t_token_type type)
{
	if(type == TOKEN_AND || type == TOKEN_OR || type == TOKEN_PIPE ||
		type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT ||
		type == TOKEN_REDIR_APPEND || type == TOKEN_REDIR_HERE)
		return (1);
	return (0);
}

/*
   is_binary_operator:
   - returns 1 if it's &&, ||, or |,
   - 0 otherwise
*/
int	is_binary_operator(t_token_type ttype)
{
	if (ttype == TOKEN_AND || ttype == TOKEN_OR || ttype == TOKEN_PIPE)
		return (1);
	return (0);
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
		return (10);
	else if (type == TOKEN_AND)
		return (20);
	else if (type == TOKEN_PIPE)
		return (30);
	else
		return (0);
}
