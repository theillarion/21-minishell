#include "minishell.h"

void	close_fds(const t_environment *env)
{
	size_t	current;
	t_cmd	*cur_cmd;

	current = -1;
	while (++current < ft_size(&env->groups))
	{
		cur_cmd = ft_get_element(&env->groups, current);
		if (cur_cmd->fd_in > 0)
		{
			if (close(cur_cmd->fd_in) == -1)
				ft_raise_error("close\n");
		}
		if (cur_cmd->fd_out > 1)
		{
			if (close(cur_cmd->fd_out) == -1)
				ft_raise_error("close\n");
		}
		cur_cmd->fd_in = 0;
		cur_cmd->fd_out = 1;
	}
}

void	wait_write_statuses(const t_environment *env)
{
	size_t		current;
	int			status;
	pid_t		pid;
	size_t		current_i;
	t_cmd		*cu_cmd;

	current = -1;
	while (++current < ft_size(&env->groups))
	{
		pid = waitpid(-1, &status, WUNTRACED);
		if (WIFSIGNALED(status) != 0)
		{
			if (WTERMSIG(status) == SIGINT)
				printf("\n");
			else if (WTERMSIG(status) == SIGQUIT)
				printf("Quit: 3\n");
		}
		current_i = -1;
		while (++current_i < ft_size(&env->groups))
		{
			cu_cmd = ft_get_element(&env->groups, current_i);
			if (cu_cmd && cu_cmd->pid == pid)
				cu_cmd->status = (status);
		}
	}
}

void	serve_pipes(t_environment *env, size_t current, t_cmd *cur_cmd)
{
	int		pipe_fd[2];
	t_cmd	*next_cmd;

	next_cmd = NULL;
	if ((current + 1) < ft_size(&env->groups))
	{
		next_cmd = (t_cmd *) ft_get_element(&env->groups, current + 1);
		pipe(pipe_fd);
		if (next_cmd->fd_in == 0)
			next_cmd->fd_in = pipe_fd[0];
		else
			close(pipe_fd[0]);
		if (cur_cmd->fd_out == 1)
			cur_cmd->fd_out = pipe_fd[1];
		else
			close(pipe_fd[1]);
	}
}

char	*construct_arg(const t_vector *src, size_t *i, t_token *token)
{
	char	*str;
	char	*t_str;

	str = NULL;
	while (*i < ft_size(src) && token->type != t_sep)
	{
		t_str = ft_substr(token->start, 0, token->size);
		if (! t_str)
			return (str);
		str = ft_strjoin(str, t_str);
		free(t_str);
		(*i)++;
		token = (t_token *)ft_get_element(src, *i);
	}
	return (str);
}

bool	ft_token_vector_to_str_array(char	***dst, const t_vector	*src)
{
	size_t	i;
	t_token	*token;
	int		j;
	char	*str;

	if (dst == NULL)
		return (false);
	*dst = (char **)malloc((ft_size(src) + 1) * sizeof(**dst));
	if (*dst == NULL)
		return (false);
	i = -1;
	j = 0;
	while (++i < ft_size(src))
	{
		token = (t_token *)ft_get_element(src, i);
		if (token->type == t_sep)
			continue ;
		str = construct_arg(src, &i, token);
		if (str == NULL)
			return (false);
		(*dst)[j] = str;
		j++;
	}
	(*dst)[j] = NULL;
	return (true);
}
