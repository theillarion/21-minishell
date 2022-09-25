#include "minishell.h"

void	ft_exec_command(t_environment *env, t_cmd *cmd, int is_child)
{
	char	*command;
	char	**envp;
	char	**args;

	ft_convert_token_vector_to_str_array(&args, &cmd->args);
	if (cmd->builtin)
	{
		if (is_child)
			exit(cmd->builtin->func(env, (const char *const *)(args + 1)));
		cmd->status = cmd->builtin->func(env, (const char *const *)(args + 1));
		free_command_args(args);
	}
	else
	{
		command = ft_substr(cmd->command->start, 0, cmd->command->size);
		ft_convert_vector_to_array(&envp, &env->variables_env);
		if (ft_strchr(command, '/') != NULL)
			execve(command, args, envp);
		find_cmd_in_path(args, envp);
	}
}

void	serve_redirects(t_cmd *cmd)
{
	t_redir		*redir;
	size_t		r;

	r = -1;
	while (++r < ft_size(&cmd->redirs))
	{
		redir = (t_redir *)ft_get_element(&cmd->redirs, r);
		if (redir->r_type == t_r_in)
			input_file_fd(redir);
		if (redir->r_type == t_hd)
			here_doc_child(redir);
		if (redir->r_type == t_r_out || redir->r_type == t_r_outa)
			output_file_fd(redir);
	}
}

void	proc_prep(t_environment *env, size_t ind, int pipe_fd[2][2], int is_chd)
{
	t_cmd	*cur_cmd;

	cur_cmd = (t_cmd *) ft_get_element(&env->groups, ind);
	if (ind != 0)
	{
		if (dup2(pipe_fd[!(ind % 2)][0], 0) == -1)
			ft_raise_error("dup2 error\n");
		if (close(pipe_fd[!(ind % 2)][0]) == -1)
			ft_raise_error("close error\n");
	}
	if ((ind + 1 != ft_size(&env->groups)))
	{
		if (dup2(pipe_fd[ind % 2][1], 1) == -1)
			ft_raise_error("dup2 error\n");
		if (close(pipe_fd[ind % 2][1]) == -1)
			ft_raise_error("close error\n");
	}
	serve_redirects(cur_cmd);
	ft_exec_command(env, cur_cmd, is_chd);
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
		current_i = -1;
		while (++current_i < ft_size(&env->groups))
		{
			cu_cmd = ft_get_element(&env->groups, current_i);
			if (cu_cmd && cu_cmd->pid == pid)
				cu_cmd->status = (status);
		}
	}
}

void	close_fds_parent(const t_environment *env, int pipe_fd[2][2], size_t curr)
{
	if (curr && close(pipe_fd[(!(curr % 2))][0]) == -1)
		ft_print_error(env, NULL, "close fd error\n");
	if (close(pipe_fd[curr % 2][1]) == -1)
		ft_print_error(env, NULL, "close fd error\n");
}

pid_t	go_throw_groups(t_environment *env, pid_t pid, int pipe_fd[2][2])
{
	size_t		current;
	t_cmd	*cur_cmd;

	current = -1;
	while (++current < ft_size(&env->groups))
	{
		cur_cmd = (t_cmd *) ft_get_element(&env->groups, current);
		pipe(pipe_fd[current % 2]);
		pid = fork();
		if (pid == -1)
			ft_print_error(env, NULL, "fork error");
		else if (pid == 0)
			proc_prep(env, current, pipe_fd, 1);
		else
		{
			cur_cmd->pid = pid;
			close_fds_parent(env, pipe_fd, current);
		}
	}
	if (pipe_fd[(current % 2)][0])
		close(pipe_fd[(current % 2)][0]);
	wait_write_statuses(env);
	return (cur_cmd->status);
}

int	executor(t_environment *env)
{
	int			status;
	int			pipe_fd[2][2];
	t_cmd	*cur_cmd;

	status = 0;
	cur_cmd = (t_cmd *) ft_get_element(&env->groups, 0);
	if (ft_size(&env->groups) == 1 && cur_cmd->builtin)
	{
		pipe(pipe_fd[0]);
		proc_prep(env, 0, pipe_fd, 0);
		status = cur_cmd->status;
	}
	else
		status = go_throw_groups(env, status, pipe_fd);
	return (status);
}
