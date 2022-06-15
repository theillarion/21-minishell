#include "minishell.h"

//static void	int_handler(int status)
//{
//	printf("\n"); // Move to a new line
//	rl_on_new_line(); // Regenerate the prompt on a newline
//	rl_replace_line("", 0); // Clear the previous text
//	rl_redisplay();
//}

int	main(void)
{
	char	prompt[12] = "minishell: ";
	char	*in;
	char	**tokens;
	pid_t	pid;
	int 	st;

	in = readline(prompt);
	tokens = ft_split_with_tokens(in);
	while (*tokens)
	{
		printf("%s\n", *tokens);
//		in = readline(prompt);
		tokens += 1;
		pid = fork();
		if (pid == 0)
			execve("cat");
		else
		{
			waitpid(pid, &st, 0);
			printf("%d", st);
		}
	}
}
