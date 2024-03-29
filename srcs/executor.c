/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: illarion <glashli@student.21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/29 15:57:49 by ltowelie          #+#    #+#             */
/*   Updated: 2022/09/29 16:29:36 by illarion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	prepare_cmd(t_environment *env, size_t ind, int is_chd)
{
	t_cmd	*cur_cmd;

	cur_cmd = (t_cmd *) ft_get_element(&env->groups, ind);
	if (cur_cmd->fd_in > 0)
	{
		if (dup2(cur_cmd->fd_in, 0) == -1)
			ft_raise_error("dup2 error\n");
	}
	if (cur_cmd->fd_out > 1)
	{
		if (dup2(cur_cmd->fd_out, 1) == -1)
			ft_raise_error("dup2 error\n");
	}
	close_fds(env);
	ft_exec_command(env, cur_cmd, is_chd);
}

void	prepare_fork(t_environment *env, pid_t pid, size_t cur, t_cmd *cur_cmd)
{
	serve_redirects(env, cur_cmd);
	serve_pipes(env, cur, cur_cmd);
	if (! cur_cmd->status)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		pid = fork();
		if (pid == -1)
			ft_print_error(env, NULL, "fork error");
		else if (pid == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			prepare_cmd(env, cur, 1);
		}
		else
			cur_cmd->pid = pid;
	}
}

pid_t	go_commands(t_environment *env, pid_t pid)
{
	size_t	current;
	t_cmd	*cur_cmd;

	current = -1;
	while (++current < ft_size(&env->groups))
	{
		cur_cmd = (t_cmd *) ft_get_element(&env->groups, current);
		prepare_fork(env, pid, current, cur_cmd);
	}
	close_fds(env);
	wait_write_statuses(env);
	return (cur_cmd->status);
}

int	single_builtin(t_environment *env, t_cmd *cur_cmd)
{
	int	status;
	int	old_stdin;
	int	old_stdout;

	old_stdin = dup(0);
	old_stdout = dup(1);
	if (old_stdin == -1 || old_stdout == -1)
		return (ft_print_error(env, NULL, "dup error\n"));
	serve_redirects(env, cur_cmd);
	prepare_cmd(env, 0, 0);
	if (dup2(old_stdin, 0) == -1)
		return (ft_print_error(env, NULL, "dup2 error\n"));
	if (close(old_stdin) == -1)
		return (ft_print_error(env, NULL, "close error\n"));
	if (dup2(old_stdout, 1) == -1)
		return (ft_print_error(env, NULL, "dup2 error\n"));
	if (close(old_stdout) == -1)
		return (ft_print_error(env, NULL, "dup2 error\n"));
	status = cur_cmd->status;
	return (status);
}

int	executor(t_environment *env)
{
	int		status;
	t_cmd	*cur_cmd;

	status = 0;
	cur_cmd = (t_cmd *) ft_get_element(&env->groups, 0);
	if (ft_size(&env->groups) == 1 && cur_cmd->builtin)
		status = single_builtin(env, cur_cmd);
	else
		status = go_commands(env, status);
	return (status);
}
