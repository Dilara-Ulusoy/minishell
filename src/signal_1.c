/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoc@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:42:05 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/04/23 09:53:34 by dakcakoc         ###   ########.fr       */
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
