/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:45:23 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/04/24 15:36:04 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNAL_H
# define SIGNAL_H

# include <signal.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <errno.h>

# define SIGNAL_PARENT 1
# define SIGNAL_HEREDOC 2
# define SIGNAL_CHILD 3

extern int	g_signal;

int		set_signals(int *exit_code, int type);
void	write_core_dump(int sig, int *printed_newline);

#endif
