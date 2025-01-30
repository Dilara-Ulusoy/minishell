
#include "minishell.h"

int is_operator(t_token_type type)
{
    return (type == TOKEN_AND || type == TOKEN_OR || type == TOKEN_PIPE ||
            type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT ||
            type == TOKEN_REDIR_APPEND || type == TOKEN_REDIR_HERE);
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


