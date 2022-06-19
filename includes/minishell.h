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
# include <stdbool.h>

typedef struct sigaction	t_sigaction;

/*
typedef	struct s_environment
{

}		t_environment;
*/

//		utilities_readline.c
void	ft_readline_insert(const char	*str);

//		utilities.c
void	ft_smart_free(void	**address);

//		signal.c
void	ft_initial_action(t_sigaction *action);

//		main.c
int		main(int argc, char **argv, char    **envp);

#endif
