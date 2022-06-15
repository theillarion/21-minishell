#include "minishell.h"

int	main(void)
{
	char	prompt[12] = "minishell: ";
	char	*in;

	in = readline(prompt);
	while (in)
	{
		printf("%s\n", in);
		in = readline(prompt);
	}
}
