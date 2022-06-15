#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>

# include "../lib/libft/libft.h"

# include <unistd.h>
# include <stdlib.h>
# include <signal.h>

typedef struct sigaction	t_sigaction;

void	ft_initial_action(t_sigaction *action);

int main(int argc, char **argv, char    **envp);

#endif
