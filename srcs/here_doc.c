/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: illarion <glashli@student.21-school.ru>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/29 15:58:20 by ltowelie          #+#    #+#             */
/*   Updated: 2022/09/29 16:28:01 by illarion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static volatile sig_atomic_t	g_my_signal_flag = 0;

static int	event(void)
{
	return (1);
}

static void	stop_readline(int sig)
{
	if (sig == SIGINT)
	{
		g_my_signal_flag = 1;
		rl_done = 1;
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	finalize_heredoc(const int *pipe_fd, char *str, char *delimit)
{
	close(pipe_fd[1]);
	if (str)
		free(str);
	if (delimit)
		free(delimit);
}

int	handle_line(char *delimit, char **str, int pipe_fd[2])
{
	if (g_my_signal_flag
		|| !str
		|| (ft_strlen(*str)
			&& ft_strlen(*str) == ft_strlen(delimit)
			&& ! ft_strncmp(delimit, *str, ft_strlen(*str))))
	{
		finalize_heredoc(pipe_fd, *str, delimit);
		return (0);
	}
	ft_putstr_fd(*str, pipe_fd[1]);
	ft_putstr_fd("\n", pipe_fd[1]);
	free(*str);
	*str = readline("> ");
	return (1);
}

void	here_doc(t_redir *token, t_cmd *cmd)
{
	char	*str;
	char	*token_delimiter;
	int		pipe_fd[2];

	rl_event_hook = event;
	signal(SIGINT, stop_readline);
	if (pipe(pipe_fd) == -1)
		ft_raise_error("pipe error\n");
	cmd->fd_in = pipe_fd[0];
	token_delimiter = ft_substr(token->arg->start, 0, token->arg->size);
	if (! token_delimiter)
		ft_raise_error("malloc delimiter error\n");
	if (!token_delimiter)
		ft_raise_error("malloc delimiter error\n");
	g_my_signal_flag = 0;
	str = readline("> ");
	while (str)
	{
		if (! handle_line(token_delimiter, &str, pipe_fd))
			return ;
	}
	finalize_heredoc(pipe_fd, str, token_delimiter);
}
