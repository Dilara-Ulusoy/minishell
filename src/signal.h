#ifndef SIGNALS_H
# define SIGNALS_H

# include <signal.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <errno.h>

# define SIGNAL_PARENT 1
# define SIGNAL_HEREDOC 2
# define SIGNAL_CHILD 3

extern int g_signal;

int		set_signals(int *exit_code, int type);

#endif
