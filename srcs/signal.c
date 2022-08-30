#include "minishell.h"

void    ft_handler(int sig, siginfo_t	*info, void	*ucontext)
{
    (void)info;
    (void)ucontext;

	if (sig != SIGINT && sig != SIGQUIT)
		return;
	rl_on_new_line();
	ft_readline_insert("  ");
/*	rl_on_new_line();
	rl_replace_line("", 0);
	//rl_redisplay();*/
    if (sig == SIGINT)
    {
		write(STDOUT_FILENO, "\n", 1);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
}

void	ft_initial_action(t_sigaction *action)
{
	ft_memset(&(*action), 0, sizeof(*action));
	action->sa_flags = SA_SIGINFO;
	action->sa_sigaction = ft_handler;
	sigemptyset(&action->sa_mask);
	sigaddset(&action->sa_mask, SIGQUIT);
	sigaddset(&action->sa_mask, SIGINT);
}
