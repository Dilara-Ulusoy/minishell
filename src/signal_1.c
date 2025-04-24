/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:42:05 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/04/24 15:35:42 by htopa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	event(void)
{
	return (0);
}

int	init_term_and_signal(int argc, int *exit_code)
{
	if (argc > 1)
		*exit_code = 127;
	set_signals(exit_code, SIGNAL_PARENT);
	return (0);
}

void	write_core_dump(int sig, int *printed_newline)
{
	if (sig == SIGINT && !printed_newline)
	{
		write(1, "\n", 1);
		*printed_newline = 1;
	}
	else if (sig == SIGQUIT)
		write(2, "Quit (core dumped)\n", 20);
}
