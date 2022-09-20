#include "../includes/minishell.h"

void	finalize_heredoc(const int *pipe_fd, char *str, char *delimit, int stat)
{
	close(pipe_fd[1]);
	if (str)
		free(str);
	if (delimit)
		free(delimit);
	exit(stat);
}

void	here_doc(t_redir *token, int pipe_fd[2])
{
	char	*str;
	char	*token_delimiter;
	char	*delimit;

	token_delimiter = ft_substr(token->arg->start, 0, token->arg->size);
	if (!token_delimiter)
		ft_raise_error("malloc delimiter error\n");
	delimit = ft_strjoin(token_delimiter, "\n");
	free(token_delimiter);
	if (!delimit)
		ft_raise_error("malloc delimiter error\n");
	str = readline("> ");
	while (str)
	{
		if (!ft_strncmp(delimit, str, ft_strlen(str)))
			finalize_heredoc(pipe_fd, str, delimit, EXIT_SUCCESS);
		ft_putstr_fd(str, pipe_fd[1]);
		ft_putstr_fd("\n", pipe_fd[1]);
		free(str);
		str = readline("> ");
	}
	finalize_heredoc(pipe_fd, str, delimit, EXIT_FAILURE);
}
