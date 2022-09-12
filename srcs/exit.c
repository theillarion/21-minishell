#include "minishell.h"

int	ft_exit(t_environment	*env, int status)
{
	ft_destroy(env);
	exit(status);
	return (status);
}

void	ft_exit_with_message(t_environment	*env, int status,
			const char	*command, const char	*msg)
{
	ft_print_error(env, command, msg);
	ft_exit(env, status);
}
