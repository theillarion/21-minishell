#include "minishell.h"

void	ft_handle_signal(int signal)
{
    if (signal == SIGINT)
    {
        write(STDOUT_FILENO, "\n", 1);
        rl_replace_line("", 0);
        rl_on_new_line();
        rl_redisplay();
    }
}

void	ft_handle_signal_child(int signal)
{
    if (signal == SIGINT)
        write(STDOUT_FILENO, "\n", 1);
    else if (signal == SIGQUIT)
        write(STDOUT_FILENO, "Quit: 3\n", 8);
    rl_replace_line("", 0);
    rl_on_new_line();
}
