#include "minishell.h"

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
        if (kind == IO_HEREDOC)
        {
            // Handle here-doc redirection
        }
        new_io = create_io_node(kind, p->current_token->value, p);
        if (!new_io)
        {
            free_io_list(*io_list);
            return;
        }
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
    if (type == TOKEN_REDIR_IN)
        return IO_IN;
    else if (type == TOKEN_REDIR_OUT)
        return IO_OUT;
    else if (type == TOKEN_REDIR_APPEND)
        return IO_APPEND;
    else if (type == TOKEN_REDIR_HERE)
        return IO_HEREDOC;
    else
        return IO_IN;
}
// Helper function: Attach a new IO node to the IO list
void attach_io_node(t_io_node **io_list, t_io_node *new_io)
{
    if (!*io_list)
    {
        *io_list = new_io;
    }
    else
    {
        t_io_node *tmp = *io_list;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new_io;
    }
}

void get_next_token(t_parser *p)
{
    if (p->current_token)
        p->current_token = p->current_token->next;
}

int is_redirection(t_token_type type)
{
    return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT ||
            type == TOKEN_REDIR_APPEND || type == TOKEN_REDIR_HERE);
}
