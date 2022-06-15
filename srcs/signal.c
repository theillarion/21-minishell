#include "minishell.h"


void    ft_handler(int sig, siginfo_t	*info, void	*ucontext)
{
    (void)info;
    (void)ucontext;
    rl_replace_line("", 0);
    if (sig == SIGINT)
    {
        printf("\n");
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
    else if (sig == SIGQUIT)
    {
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
