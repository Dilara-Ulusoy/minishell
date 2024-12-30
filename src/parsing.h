#ifndef PARSING_H
# define PARSING_H

typedef enum e_node_type
{
	N_PIPE, // |
	N_AND, // &&
	N_OR, // ||
	N_CMD // Command etc ls, echo
}
t_node_type;

typedef enum e_io_type
{
	IO_IN, // <
	IO_OUT, // >
	IO_HEREDOC, // <<
	IO_APPEND // >>
}
t_io_type;

typedef struct s_io_node
{
	t_io_type			type;
	char				*value;
	struct s_io_node	*prev;
	struct s_io_node	*next;
}	t_io_node;

typedef struct s_node
{
	t_node_type			type;
	t_io_node			*io_list;
	char				*args;
	struct s_node		*left;
	struct s_node		*right;
}
t_node;

#endif