#include "minishell.h"

void	ft_exec_command(t_environment *env, t_command *cmd, int is_child)
{
	char	*command;
	char	**envp;
	char	**args;

	command = ft_substr(cmd->command->start, 0, cmd->command->size);
	ft_convert_vector_to_array(&envp, &env->variables_env);
	ft_convert_token_vector_to_str_array(&args, &cmd->args);
	if (cmd->builtin)
	{
		if (is_child)
			exit(cmd->builtin->func(env, *(++args)));
		cmd->builtin->func(env, *(++args));
	}
	else
	{
		if (ft_strchr(command, '/') != NULL)
			execve(command, args, envp);
		find_cmd_in_path(args, envp);
	}
}

void	serve_redirects(size_t i, int pipe_fd[2][2], t_command *cmd)
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
			here_doc(redir, pipe_fd[i % 2]);
		if (redir->r_type == t_r_out || redir->r_type == t_r_outa)
			output_file_fd(redir);
	}
}

void	proc_prep(t_environment *env, size_t i, int pipe_fd[2][2], int is_chld)
{
	t_command	*cur_cmd;

	cur_cmd = (t_command *) ft_get_element(&env->groups, i);
	if (i != 0)
	{
		if (dup2(pipe_fd[!(i % 2)][0], 0) == -1)
			ft_raise_error("dup2 error\n");
	}
	if ((i + 1 != ft_size(&env->groups)))
	{
		if (dup2(pipe_fd[i % 2][1], 1) == -1)
			ft_raise_error("dup2 error\n");
	}
	serve_redirects(i, pipe_fd, cur_cmd);
	ft_exec_command(env, cur_cmd, is_chld);
}

pid_t	go_throw_groups(t_environment *env, pid_t pid, int pipe_fd[2][2])
{
	size_t		current;

	current = -1;
	while (++current < ft_size(&env->groups))
	{
		pipe(pipe_fd[current % 2]);
		pid = fork();
		if (pid == -1)
			ft_raise_error("fork error\n");
		else if (pid == 0)
			proc_prep(env, current, pipe_fd, 1);
		else
		{
			if (close(pipe_fd[current % 2][1]) == -1)
				ft_raise_error("close fd error\n");
		}
	}
	return (pid);
}

int	executor(t_environment *env)
{
	pid_t		pid;
	int			pipe_fd[2][2];
	t_command	*cur_cmd;

	pid = 0;
	cur_cmd = (t_command *) ft_get_element(&env->groups, 0);
	if (ft_size(&env->groups) == 1 && cur_cmd->builtin)
	{
		pipe(pipe_fd[0]);
		proc_prep(env, 0, pipe_fd, 0);
	}
	else
		pid = go_throw_groups(env, pid, pipe_fd);
	return (pid);
}