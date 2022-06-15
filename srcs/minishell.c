#include "minishell.h"

int main(int argc, char **argv, char    **envp)
{
	t_sigaction	action;
	char		prompt[12] = "minishell: ";
	char		*in;

	(void)argc;
	(void)argv;
	(void)envp;
	ft_initial_action(&action);
	if (sigaction(SIGQUIT, &action, NULL) == -1
		|| sigaction(SIGINT, &action, NULL) == -1)
	in = readline(prompt);
	while (in)
	{
		printf("%s\n", in);
		in = readline(prompt);
	}
}
