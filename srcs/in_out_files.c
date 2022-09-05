#include "../includes/minishell.h"

void	input_file_fd(t_redir *token)
{
	char	*path;
	int		read_fd;

	path = ft_substr(token->arg->start, 0, token->arg->size);
	read_fd = open(path, O_RDONLY);
	if (read_fd == -1)
		ft_raise_perror(path, 0);
	if (dup2(read_fd, 0) == -1)
		ft_raise_perror(path, 0);
	if (close(read_fd) == -1)
		ft_raise_perror(path, 0);
}

void	output_file_fd(t_redir *token)
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
	if (dup2(write_fd, 1) == -1)
		ft_raise_perror(path, 0);
	if (close(write_fd) == -1)
		ft_raise_perror(path, 0);
}
