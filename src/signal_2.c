/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dakcakoc <dakcakoc@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:43:28 by dakcakoc          #+#    #+#             */
/*   Updated: 2025/04/15 15:43:31 by dakcakoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal = 0;

static void	set_signal_exit(int *exit_code, int sig)
{
	static int	*exit_code_ptr;

	if (exit_code != NULL)
		exit_code_ptr = exit_code;
	else
	{
		if (exit_code_ptr && sig > 0)
			*exit_code_ptr = 128 + sig;
		g_signal = sig;
	}
}

static void	sig_handler_parent(int signo)
{
	if (signo == SIGINT)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		if (!isatty(STDIN_FILENO) || errno != EINTR)
			return ;
		set_signal_exit(NULL, signo);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

static void	sig_handler_heredoc(int signo)
{
	if (signo == SIGINT)
	{
		rl_done = 1;
		set_signal_exit(NULL, signo);
	}
}

static int	set_parent_signals(void)
{
	struct sigaction	sa;

	rl_done = 0;
	rl_event_hook = NULL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = sig_handler_parent;
	if (sigaction(SIGINT, &sa, NULL) == -1)
		return (0);
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
		return (0);
	return (1);
}

int	set_signals(int *exit_code, int type)
{
	set_signal_exit(exit_code, 0);
	if (type == SIGNAL_PARENT)
		return (set_parent_signals());
	else if (type == SIGNAL_HEREDOC)
	{
		rl_event_hook = event;
		if (signal(SIGINT, sig_handler_heredoc) == SIG_ERR)
			return (0);
		if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
			return (0);
	}
	else if (type == SIGNAL_CHILD)
	{
		if (signal(SIGINT, SIG_DFL) == SIG_ERR)
			return (0);
		if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
			return (0);
	}
	return (1);
}
