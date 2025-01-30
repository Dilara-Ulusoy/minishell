#include "minishell.h"

/*
   match_two_char_operator:
   - Looks at line[index] and line[index+1].
   - Returns a token_type if it matches "&&", "||", "<<" or ">>".
   - Otherwise returns TOKEN_UNKNOWN.
*/
t_token_type match_two_char_operator(const char *line, int index)
{
    if (line[index] == '&' && line[index + 1] == '&')
        return TOKEN_AND;
    if (line[index] == '|' && line[index + 1] == '|')
        return TOKEN_OR;
    if (line[index] == '<' && line[index + 1] == '<')
        return TOKEN_REDIR_HERE;
    if (line[index] == '>' && line[index + 1] == '>')
        return TOKEN_REDIR_APPEND;
    return TOKEN_UNKNOWN;
}

/*
   match_single_char_operator:
   - Checks if 'c' is one of the known single-character operators.
   - If it is, return the corresponding token_type.
   - If not, return TOKEN_UNKNOWN.
*/
t_token_type match_single_char_operator(char c)
{
    if (c == '<')
        return TOKEN_REDIR_IN;
    if (c == '>')
        return TOKEN_REDIR_OUT;
    if (c == '|')
        return TOKEN_PIPE;
    if (c == '(')
        return TOKEN_PAREN_OPEN;
    if (c == ')')
        return TOKEN_PAREN_CLOSE;
    return TOKEN_UNKNOWN;
}