#include "minishell.h"

int main(int argc, char **argv, char    **envp)
{
	t_sigaction	action;
	char		prompt[] = "\033[92mminishell\033[0m->";
	char		*in;

	(void)argc;
	(void)argv;
	(void)envp;
	ft_initial_action(&action);
	if (sigaction(SIGQUIT, &action, NULL) == -1
		|| sigaction(SIGINT, &action, NULL) == -1)
	{
		//error
		printf("Error!\n");
	}
	in = readline(prompt);
	while (in)
	{
		//printf("%s\n", in);
		in = readline(prompt);
	}
}
