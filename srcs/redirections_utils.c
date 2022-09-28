#include "minishell.h"

void	input_file_fd(t_redir *token, t_cmd *cmd)
{
	char	*path;
	int		read_fd;

	path = ft_substr(token->arg->start, 0, token->arg->size);
	read_fd = open(path, O_RDONLY);
	if (read_fd == -1)
		ft_raise_perror(path, 0);
	cmd->fd_in = read_fd;
}

void	output_file_fd(t_redir *token, t_cmd *cmd)
{
	char	*path;
	int		flags;
	int		write_fd;

	path = ft_substr(token->arg->start, 0, token->arg->size);
	flags = O_CREAT | O_RDWR;
	if (token->r_type == t_r_outa)
		flags = flags | O_APPEND;
	else
		flags = flags | O_TRUNC;
	write_fd = open(path, flags, 0644);
	if (write_fd == -1)
		ft_raise_perror(path, 0);
	cmd->fd_out = write_fd;
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
			input_file_fd(redir, cmd);
		if (redir->r_type == t_hd)
			here_doc(redir, cmd);
		if (redir->r_type == t_r_out || redir->r_type == t_r_outa)
			output_file_fd(redir, cmd);
	}
}
