#include "minishell.h"

void ft_print_error(t_environment	*env, const char *command, const char *msg)
{
	if (env == NULL || command == NULL || msg == NULL)
		return ;
	ft_putstr_fd(env->info.name_shell, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd((char *)command, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd((char *)msg, STDERR_FILENO);
}

void ft_print_errno(t_environment	*env, const char *command)
{
	if (errno != 0)
	{
		env->last_code = errno;
		ft_print_error(env, command, strerror(errno));
		errno = 0;
	}
}
