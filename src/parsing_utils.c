
#include "minishell.h"

bool is_redirection(t_token_type type)
{
	if(type == T_REDIRECT_OUT || type == T_REDIRECT_IN || type == T_REDIRECT_APPEND || type == T_REDIRECT_HEREDOC)
		return true;
	return false;
}

bool is_binary_operator_token(t_token *token)
{
	t_token *current = token;
	if(!current)
		return false;
	if(current->type == T_AND || current->type == T_OR || current->type == T_PIPE)
		return true;
	return false;
}

int is_precedence_token(t_token_type type)
{
	if(type == T_AND)
		return 1;
	if(type == T_OR)
		return 0;
	return -1;
}

void get_next_token(t_shell *shell)
{
	if(shell->current_token)
		shell->current_token = shell->current_token->next;
}
