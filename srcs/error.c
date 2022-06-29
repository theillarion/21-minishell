#include "minishell.h"

void	ft_error(const char	*name_shell, const char	*err_msg)
{
	if (errno != 0)
	{
		ft_putstr_fd((char *)name_shell, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		perror(err_msg);
		errno = 0;
	}
}
