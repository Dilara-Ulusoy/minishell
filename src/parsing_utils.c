
#include "minishell.h"
#include "parsing.h"

t_node_type	ft_get_node_type(t_token_type type)
{
	if (type == T_AND)
		return (N_AND);
	if (type == T_OR)
		return (N_OR);
	return (N_PIPE);
}