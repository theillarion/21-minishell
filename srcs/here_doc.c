#include "../includes/minishell.h"

void	finalize_heredoc(const int *pipe_fd, char *str, char *delimit)
{
	close(pipe_fd[1]);
	if (str)
		free(str);
	if (delimit)
		free(delimit);
}

void	here_doc(t_redir *token)
{
	char	*str;
	char	*token_delimiter;
	char	*delimit;
	int 	pipe_fd[2];

	if (pipe(pipe_fd) == -1)
		ft_raise_error("pipe error\n");
	token->here_doc_fd = pipe_fd[0];
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
			return(finalize_heredoc(pipe_fd, str, delimit));
		ft_putstr_fd(str, pipe_fd[1]);
		ft_putstr_fd("\n", pipe_fd[1]);
		free(str);
		str = readline("> ");
	}
	finalize_heredoc(pipe_fd, str, delimit);
}

void	read_heredocs(t_environment *env)
{
	size_t		current_cmd;
	t_redir		*redir;
	size_t		current_redir;
	t_command 	*cmd;

	current_cmd = -1;
	while (++current_cmd < ft_size(&env->groups))
	{
		cmd = ft_get_element(&env->groups, current_cmd);
		current_redir = -1;
		while (++current_redir < ft_size(&cmd->redirs))
		{
			redir = (t_redir *)ft_get_element(&cmd->redirs, current_redir);
			if (redir->r_type == t_hd)
				here_doc(redir);
		}
	}
}