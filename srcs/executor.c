#include "minishell.h"

void	ft_exec_command(t_environment *env, t_command *cmd, int is_child)
{
	char	*command;
	char	**envp;
	char	**args;

	command = ft_substr(cmd->command->start, 0, cmd->command->size);
	if (ft_strlen(command) == 0)
		exit(0);
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

void	process_prepare(t_environment *env, size_t i, int pipe_fd[2][2], int is_child)
{
	t_command	*cur_cmd;
	size_t		r;
	t_redir		*cur_redir;

	cur_cmd = (t_command *) ft_get_element(&env->groups, i);
	r = -1;
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
	while (++r < ft_size(&cur_cmd->redirs))
	{
		cur_redir = (t_redir *)ft_get_element(&cur_cmd->redirs, r);
		if (cur_redir->r_type == t_r_in)
			input_file_fd(cur_redir);
		if (cur_redir->r_type == t_hd)
			here_doc(cur_redir, pipe_fd[i % 2]);
		if (cur_redir->r_type == t_r_out || cur_redir->r_type == t_r_outa)
			output_file_fd(cur_redir);
	}
	ft_exec_command(env, cur_cmd, is_child);
}

int	executor(t_environment *env)
{
	size_t	current;
	pid_t	pid;
	int		pipe_fd[2][2];
	t_command	*cur_cmd;

	pid = 0;
	current = 0;
	cur_cmd = (t_command *) ft_get_element(&env->groups, current);
	if (ft_size(&env->groups) == 1 && cur_cmd->builtin)
	{
		pipe(pipe_fd[current % 2]);
		process_prepare(env, current, pipe_fd, 0);
	}
	else
	{
		current = -1;

		while (++current < ft_size(&env->groups)) {
			pipe(pipe_fd[current % 2]);
			pid = fork();
			if (pid == -1)
				ft_raise_error("fork error\n");
			else if (pid == 0)
				process_prepare(env, current, pipe_fd, 1);
			else {
				if (close(pipe_fd[current % 2][1]) == -1)
					ft_raise_error("close fd error\n");
			}
		}
	}
	return (pid);
}
