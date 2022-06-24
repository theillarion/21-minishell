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
	char	*delimit;

	// ToDo fix leak
	delimit = ft_strjoin(ft_substr(token->arg->start, 0, token->arg->size), "\n");
	if (!delimit)
		ft_raise_error("malloc delimiter error\n");
	ft_putstr_fd("> ", 1);
	str = get_next_line(0);
	while (str)
	{
		if (!ft_strncmp(delimit, str, ft_strlen(str)))
			finalize_heredoc(pipe_fd, str, delimit, EXIT_SUCCESS);
		ft_putstr_fd(str, pipe_fd[1]);
		free(str);
		ft_putstr_fd("> ", 1);
		str = get_next_line(0);
	}
	finalize_heredoc(pipe_fd, str, delimit, EXIT_FAILURE);
}
